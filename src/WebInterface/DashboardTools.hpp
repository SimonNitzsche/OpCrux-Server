#ifndef __WEBINTERFACE__DASHBOARDTOOLS_HPP__
#define __WEBINTERFACE__DASHBOARDTOOLS_HPP__

#include <Mongoose/mongoose.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <istream>
#include <fstream>

#include "Utils/StringUtils.hpp"

class DashboardTools {
public:
	static constexpr char* resPath() { return "./res/HTTP/"; }
	static std::string getFileContent(std::string file) {
		std::string out = "";
		std::ifstream infile(std::string(resPath() + file), std::ifstream::in);
		while (!infile.eof()) {
			std::string line;
			std::getline(infile, line);
			out += line;
		}
		infile.close();
		return out;
	}

	static std::string getPageContent(std::string file, bool error = false) {
		return  (getFileContent("Framework/top.html") +
			getFileContent(file) +
			(error?getFileContent("Error/iefix.html"):"") +
			getFileContent("Framework/end.html"));
	}

	static std::string getErrorString(int code) {
		switch (code) {
		case 200:
			return "OK";
			break;
		case 403:
			return "Forbidden";
			break;
		case 404:
			return "Not found";
			break;
		default:
			return "Unregistered Error code: " + std::to_string(code);
		}
	}

	static void sendHeaderResponse200(struct mg_connection *nc, std::string extra = "") {
		mg_printf(
			nc, "%s",
			"HTTP/1.1 200 OK \r\n"
			"Content-Type: text/html\r\n"
			"Transfer-Encoding: chunked\r\n"
			"\r\n"
		);
	}

	static void sendHeaderResponse403(struct mg_connection *nc) {
		mg_printf(
			nc, "%s",
			"HTTP/1.1 403 Forbidden \r\n"
			"Content-Type: text/html\r\n"
			"Transfer-Encoding: chunked\r\n"
			"\r\n"
		);
	}

	static void sendHeaderResponse404(struct mg_connection *nc) {
		mg_printf(
			nc, "%s",
			"HTTP/1.1 404 Not Found \r\n"
			"Content-Type: text/html\r\n"
			"Transfer-Encoding: chunked\r\n"
			"\r\n"
		);
	}

	static bool isLoggedIn(mg_connection * nc, http_message * hm) {
		for (std::ptrdiff_t i = 0; i < sizeof(hm->header_names); ++i) {
			std::string headerName = std::string(hm->header_names[i].p, hm->header_names[i].len);
			if (headerName == "Cookie") {
				std::vector<std::string> cookies = StringUtils::splitString(std::string(hm->header_values[i].p, hm->header_values[i].len), (char)59);
				for (int j = 0; j < cookies.size(); ++j) {
					std::vector<std::string> cookie = StringUtils::splitString(cookies[j],(char)61);
					if (cookie[0][0] == (char)20) cookie[0] = cookie[0].substr(1);
					bool ag = (cookie[0] == "auth" || cookie[0] == " auth") && cookie[1] == "EYLUAR";
					return true;
					throw std::runtime_error("TODO: Implement Database.");
					/*
					if ((cookie[0] == "auth"||cookie[0]==" auth")&&Database::accessGrantedWI(cookie[1])) {
						return true;
					}*/
				}
				break;
			}
		}

		return false;
	}

	static void applyHTMLVariables(std::string &finalString, struct mg_connection *nc, int ev, struct http_message *hm);

	static std::string getErrHTML(int code) {
		std::string final = getPageContent("Error/" + std::to_string(code) + ".html", true);
		while(StringUtils::replace(final, "{%CONTENT-TITLE%}", getErrorString(code)));
		return final;
	}
};

#endif // !__WEBINTERFACE__DASHBOARDTOOLS_HPP__
