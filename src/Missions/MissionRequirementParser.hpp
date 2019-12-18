#ifndef __MISSIONS_MISSIONREQUIREMENTPARSER_HPP__
#define __MISSIONS_MISSIONREQUIREMENTPARSER_HPP__

#include "Database/Database.hpp"
#include <iostream>
#include <string_view>

class MissionRequirementParser {
private:
	std::uint32_t inputIndex = 0;
	std::uint32_t openBrackets = 0;
	static const std::uint32_t openBracketsLimit = 12;
	bool isOperatorOR = true; // 0 = AND, 1 = OR
	std::list<Database::MissionModel> m_availableMissions;
public:
	bool result = false;
	bool parseSucceeded = false;
private:

	void err(const char* msg) {
		parseSucceeded = false;
		std::cout << msg << std::endl;
	}

	inline static std::pair<std::int32_t, std::int32_t> splitMission(std::string_view sv) {
		int m, t;
		size_t it = sv.find(":");
		if (it != std::string_view::npos) {
			m=std::stoi(static_cast<std::string>(sv.substr(0, it)));
			t=std::stoi(static_cast<std::string>(sv.substr(it+1, sv.length()-it-1)));
		}
		else {
			m = std::stoi(static_cast<std::string>(sv));
			t = 8;
		}
		return std::pair<std::int32_t, std::int32_t>{m, t};
	}

	inline bool hasMissionFromBuffer(std::string_view missionAndOrTask) {
		auto mt = splitMission(missionAndOrTask);
		for (auto it = m_availableMissions.begin(); it != m_availableMissions.end(); ++it) {
			auto mm = *it;
			if (mm.missionID == mt.first) {
				return mm.state == mt.second;
			}
		}
		return false;
	}

	inline bool doBitOperation(bool leftHand, bool rightHand) {
		return isOperatorOR ? (leftHand | rightHand) : (leftHand & rightHand);
	}

	bool parse(std::string_view statement) {
		// In case we ran into an error before
		if (!parseSucceeded) return false;

		bool bufferResult = false;
		std::uint32_t bufferBegin = inputIndex;
		std::uint32_t bufferLength = 0;

		for (; inputIndex < statement.length(); ++inputIndex) {
			switch (statement.at(inputIndex)) {
			case ' ': {
				// WHITE SPACE
				++bufferLength; // We will deal with it later
				break;
			}
			case ',':
			case '&':
			case '|': {

				if (bufferLength != 0) {
					// set operator
					isOperatorOR = (statement.at(inputIndex) == '|');
					// perform operation
					bufferResult = doBitOperation(bufferResult, hasMissionFromBuffer(statement.substr(bufferBegin, bufferLength)));
					// reset buffer
					bufferBegin = inputIndex + 1;
					bufferLength = 0;
				}

				break;
			}
			case '(': {
				// Bracket open

				if (openBrackets < openBracketsLimit) {
					++openBrackets;

					// move forward
					++inputIndex;

					// perform operation
					bufferResult = doBitOperation(bufferResult, parse(statement));

					// Check if recursion caused error
					if (!parseSucceeded) return false;

					// reset buffer
					bufferBegin = inputIndex + 1;
					bufferLength = 0;
				}
				else {
					err("Too many brackets opened.");
					return false;
				}

				break;
			}
			case ')': {
				// Bracket close

				if (openBrackets != 0) {
					--openBrackets;

					// perform operation
					return doBitOperation(bufferResult, hasMissionFromBuffer(statement.substr(bufferBegin, bufferLength)));
				}
				else {
					err("Tried to close bracket without an open bracket");
					return false;
				}

				break;
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case ':': {
				// LITERAL
				++bufferLength;
				break;
			}
			default: {
				err("Unknown operator: " + statement.at(inputIndex));
				return false;
			}
			}
		}

		// End of statement
		if (parseSucceeded) {
			if (bufferLength != 0) {
				bufferResult = doBitOperation(bufferResult, hasMissionFromBuffer(statement.substr(bufferBegin, bufferLength)));
			}
			return bufferResult;
		}

		// Something went wrong
		return false;
	}

public:

	static std::list<std::string_view> sweepMissionList(std::string_view statement) {
		std::list<std::string_view> result = {};
		std::uint32_t buffer_begin = 0;
		std::uint32_t buffer_end = 0;
		for (int i = 0; i <= statement.size(); ++i) {
			switch ((i < statement.size())?statement.at(i):'\0') {

				// Accept [0-9,:]
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case ':': {
				++buffer_end;
				break;
			}

					// Ignore everything else
			default: {
				if (buffer_end != 0) {
					std::string_view newEntry = statement.substr(buffer_begin, buffer_end);

					// Check for duplicates
					bool isDuplicate = false;
					if (result.size() != 0) {
						auto resultEnd = result.end();
						auto newEntryBegin = newEntry.begin();
						for (auto it = result.begin(); it != resultEnd; ++it) {
							if (*it == newEntry) {
								isDuplicate = true;
								break;
							}
						}
					}
					if (!isDuplicate) {
						result.push_back(newEntry);
					}
					buffer_end = 0;
				}
				buffer_begin = i;
				++buffer_begin;
				break;
			}
			}
		}
		return result;
	}

	static std::list<std::int32_t> sweepMissionListNumerical(std::string_view statement) {
		std::list<std::int32_t> result = {};
		std::list<std::string_view> sRes = sweepMissionList(statement);

		for (auto it1 = sRes.begin(); it1 != sRes.end(); ++it1) {
			std::string_view svE = *it1;
			result.push_back(splitMission(svE).first);
		}

		return result;
	}

	MissionRequirementParser(std::string_view statement, std::list<Database::MissionModel> availableMissions) : m_availableMissions(availableMissions) {
		parseSucceeded = true;

		result = parse(statement);
	}
};

#endif