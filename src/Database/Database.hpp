#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

#define ODBC_STATIC

// TODO: Figure out Linux solution
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <algorithm>
#include <list>
#include <odbc/Connection.h>
#include <odbc/Environment.h>
#include <odbc/Exception.h>
#include <odbc/PreparedStatement.h>
#include <odbc/ResultSet.h>

#include "Common/CrossPlatform.hpp"
#include "Encryption/sha512.hpp"
#include "GameCache/Interface/FastDatabase.hpp"
#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/MissionTasks.hpp"
#include "DataTypes/Vector3.hpp"
#include "DataTypes/LWOOBJID.hpp"
#include "Configuration/ConfDatabase.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "DataTypes/LDF.hpp"
#include "Utils/LDFUtils.hpp"

#include "Database/DatabaseModels.hpp"

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

using namespace GameCache::Interface;
using namespace DatabaseModels;
extern FDB::Connection Cache;
class Database {
public:

	static void extract_error(
		char* fn,
		SQLHANDLE handle,
		SQLSMALLINT type) {
		SQLINTEGER   i = 0;
		SQLINTEGER   native;
		SQLCHAR      state[7];
		SQLCHAR      text[256];
		SQLSMALLINT  len;
		SQLRETURN    ret;

		fprintf(stderr, "[DATABASE] While running %s the driver reported:", fn);

		do
		{
			ret = SQLGetDiagRec(type, handle, ++i, state, &native, text,
				sizeof(text), &len);

			std::string str = "";

			switch (ret) {
			case SQL_SUCCESS:
				str = "SQL_SUCCESS"; break;
			case SQL_SUCCESS_WITH_INFO:
				str = "SQL_SUCCESS_WITH_INFO"; break;
			case SQL_ERROR:
				str = "SQL_ERROR"; break;
			case SQL_INVALID_HANDLE:
				str = "SQL_INVALID_HANDLE"; break;
			case SQL_NO_DATA:
				str = "SQL_NO_DATA"; break;
			case SQL_NEED_DATA:
				str = "SQL_NEED_DATA"; break;
			case SQL_STILL_EXECUTING:
				str = "SQL_STILL_EXECUTING"; break;
			default:
				str = "Unknkown " + ret;
			}

			std::cout << " " << str.c_str() << std::endl;

			if (SQL_SUCCEEDED(ret) || ret == SQL_ERROR)
				printf("[DATABASE] %s ->", text);
		} while (ret == SQL_SUCCESS || ret == SQL_ERROR);
	}

	static void Test() {
		ListDrivers();
		Connect();
		Disconnect();
	}

	static void ListDrivers() {
		{
			SQLHENV env;
			SQLCHAR driver[256];
			SQLCHAR attr[256];
			SQLSMALLINT driver_ret;
			SQLSMALLINT attr_ret;
			SQLUSMALLINT direction;
			SQLRETURN ret;

			SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
			SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

			direction = SQL_FETCH_FIRST;
			while (SQL_SUCCEEDED(ret = SQLDrivers(env, direction,
				driver, sizeof(driver), &driver_ret,
				attr, sizeof(attr), &attr_ret))) {
				direction = SQL_FETCH_NEXT;
				printf("%s - %s\n", driver, attr);
				if (ret == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
			}
		}
	}
private:
	//define handles and variables
	inline static SQLHANDLE sqlConnHandle;
	inline static SQLHANDLE sqlEnvHandle;
	inline static SQLCHAR retconstring[SQL_RETURN_CODE_LEN];
	inline static SQLHANDLE sqlStmtHandle = NULL;

	inline static odbc::EnvironmentRef env = odbc::Environment::create();
	inline static odbc::ConnectionRef conn;

public:
	static SQLHANDLE GetSqlStmtHandle() {
		return sqlStmtHandle;
	}

	static int Connect() {
		Logger::log("DATABASE", "Attempting connection to SQL Server...");

		auto dbConf = &Configuration::ConfigurationManager::dbConf;

		std::string connStrBuilder = \
			"DRIVER={"\
			+ dbConf->GetStringVal("DBConnection", "DBDRIVER")\
			+ "};SERVER="\
			+ dbConf->GetStringVal("DBConnection", "DBHOST")\
			+ ";UID="\
			+ dbConf->GetStringVal("DBConnection", "DBUSER")\
			+ ";PWD="\
			+ dbConf->GetStringVal("DBConnection", "DBPASS")\
			+ ";";

		conn = env->createConnection();
		conn->connect(connStrBuilder.c_str());
		conn->setAutoCommit(true);
	}

	static void Disconnect() {
		Logger::log("DATABASE", "WARNING!!!! DATABASE HAS BEEN UNLOADED!", LogType::ERR);

		conn->disconnect();
	}

	static bool IsLoginCorrect(char16_t* username, char16_t* password) {
		std::u16string w_username(username);
		std::string s_username(w_username.begin(), w_username.end());
		std::u16string w_password(password);
		std::string s_password(w_password.begin(), w_password.end());

		std::string h_password = sha512(s_password);

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT password FROM OPCRUX_AD.dbo.Accounts WHERE username=?");
		stmt->setCString(1, s_username.c_str());
		odbc::ResultSetRef rs = stmt->executeQuery();
		if (rs->next()) {
			std::string db_hash = *rs->getString(1);
			std::string dbhash = "DB_HASH: " + db_hash;
			std::string pkhash = "PK_HASH: " + h_password;
			Logger::log("DATABASE", dbhash);
			Logger::log("DATABASE", pkhash);

			return (db_hash == h_password);
		}

		return false;
	}

	static bool APILoginCheck(std::string s_username, std::string s_password) {

		std::u16string w_username(s_username.begin(), s_username.end());
		std::u16string w_password(s_password.begin(), s_password.end());

		return IsLoginCorrect(const_cast<char16_t*>(w_username.c_str()), const_cast<char16_t*>(w_password.c_str()));
	}

	static void SetupStatementHandle() {

		throw "NO!";
	}


	enum class DBCOUNTERID {
		STATIC,
		PLAYER,
		P_STYLE,
		P_STATS
	};

	static unsigned long long reserveCountedID(DBCOUNTERID dbCounterID) {

		const char* query;
		switch (dbCounterID) {
		case DBCOUNTERID::STATIC:
			query = "SELECT counter FROM OPCRUX_CD.dbo.IDCounter WHERE type='STATIC';UPDATE OPCRUX_CD.dbo.IDCounter SET counter=(counter+1) WHERE type='STATIC';";
			break;
		case DBCOUNTERID::PLAYER:
			query = "SELECT counter FROM OPCRUX_CD.dbo.IDCounter WHERE type='PLAYER';UPDATE OPCRUX_CD.dbo.IDCounter SET counter=(counter+1) WHERE type='PLAYER';";
			break;
		case DBCOUNTERID::P_STYLE:
			query = "SELECT counter FROM OPCRUX_CD.dbo.IDCounter WHERE type='P_STYLE';UPDATE OPCRUX_CD.dbo.IDCounter SET counter=(counter+1) WHERE type='P_STYLE';";
			break;
		case DBCOUNTERID::P_STATS:
			query = "SELECT counter FROM OPCRUX_CD.dbo.IDCounter WHERE type='P_STATS';UPDATE OPCRUX_CD.dbo.IDCounter SET counter=(counter+1) WHERE type='P_STATS';";
			break;
		default:
			return -1;
		}

		odbc::PreparedStatementRef stmt = conn->prepareStatement(query);
		odbc::ResultSetRef rs = stmt->executeQuery();
		if (rs->next()) {
			return *rs->getLong(1);
		}
		return -1;
	}

	static int GetCharCount(unsigned long accountID) {
		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT COUNT(objectID) FROM OPCRUX_GD.dbo.Characters WHERE accountID=?");
		stmt->setUInt(1, accountID);
		odbc::ResultSetRef rs = stmt->executeQuery();
		if (rs->next()) {
			return *rs->getInt(1);
		}

		return 0;
	}

	static Str_DB_CharStyle GetCharStyle(unsigned long styleID) {
		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT headColor, head, chestColor, chest, legs, hairStyle, hairColor, leftHand, rightHand, eyebrowStyle, eyesStyle, mouthStyle FROM OPCRUX_GD.dbo.CharacterStyles WHERE id=?");
		stmt->setUInt(1, styleID);
		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			Str_DB_CharStyle charStyle;
			charStyle.headColor = *rs->getInt(1);
			charStyle.head = *rs->getInt(2);
			charStyle.chestColor = *rs->getInt(3);
			charStyle.chest = *rs->getInt(4);
			charStyle.legs = *rs->getInt(5);
			charStyle.hairStyle = *rs->getInt(6);
			charStyle.hairColor = *rs->getInt(7);
			charStyle.leftHand = *rs->getInt(8);
			charStyle.rightHand = *rs->getInt(9);
			charStyle.eyebrowStyle = *rs->getInt(10);
			charStyle.eyesStyle = *rs->getInt(11);
			charStyle.mouthStyle = *rs->getInt(12);

			return charStyle;
		}

		return {};
	}

	static std::vector<Str_DB_CharInfo> GetChars(unsigned long accountID) {
		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT objectID, charIndex, name, pendingName, styleID,statsID, lastWorld, lastInstance, lastClone, lastLog, positionX, positionY, positionZ, shirtObjectID, pantsObjectID, uScore, uLevel, currency, reputation, health, imagination, armor FROM OPCRUX_GD.dbo.Characters WHERE accountID=? ORDER BY charIndex");
		stmt->setUInt(1, accountID);
		odbc::ResultSetRef rs = stmt->executeQuery();

		std::vector<Str_DB_CharInfo> charsInfo;

		while (rs->next()) {
			Str_DB_CharInfo charInfo;
			charInfo.accountID = accountID;
			charInfo.objectID = *rs->getULong(1);
			charInfo.charIndex = *rs->getInt(2);
			charInfo.name = *rs->getString(3);
			charInfo.pendingName = *rs->getString(4);
			charInfo.styleID = *rs->getInt(5);
			charInfo.statsID = *rs->getInt(6);
			charInfo.lastWorld = *rs->getShort(7) & 0xFFFF;
			charInfo.lastInstance = *rs->getShort(8) & 0xFFFF;
			charInfo.lastClone = *rs->getInt(9);
			charInfo.lastLog = *rs->getULong(10);
			charInfo.position = DataTypes::Vector3(*rs->getFloat(11), *rs->getFloat(12), *rs->getFloat(13));
			charInfo.shirtObjectID = *rs->getULong(14);
			charInfo.pantsObjectID = *rs->getULong(15);
			charInfo.uScore = *rs->getInt(16);
			charInfo.uLevel = *rs->getInt(17);
			charInfo.currency = *rs->getInt(18);
			charInfo.reputation = *rs->getInt(19);
			charInfo.health = *rs->getInt(20);
			charInfo.imagination = *rs->getInt(21);
			charInfo.armor = *rs->getInt(22);
			charsInfo.push_back(charInfo);
		}

		return charsInfo;
	}

	static Str_DB_CharInfo GetChar(unsigned long long objectID) {
		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT accountID, charIndex, name, pendingName, styleID,statsID, lastWorld, lastInstance, lastClone, lastLog, positionX, positionY, positionZ, shirtObjectID, pantsObjectID, uScore, uLevel, currency, reputation, health, imagination, armor FROM OPCRUX_GD.dbo.Characters WHERE accountID=? ORDER BY charIndex");
		stmt->setULong(1, objectID);
		odbc::ResultSetRef rs = stmt->executeQuery();

		std::vector<Str_DB_CharInfo> charsInfo;

		while (rs->next()) {
			Str_DB_CharInfo charInfo;
			charInfo.accountID = *rs->getInt(1);
			charInfo.objectID = objectID;
			charInfo.charIndex = *rs->getInt(2);
			charInfo.name = *rs->getString(3);
			charInfo.pendingName = *rs->getString(4);
			charInfo.styleID = *rs->getInt(5);
			charInfo.statsID = *rs->getInt(6);
			charInfo.lastWorld = *rs->getShort(7) & 0xFFFF;
			charInfo.lastInstance = *rs->getShort(8) & 0xFFFF;
			charInfo.lastClone = *rs->getInt(9);
			charInfo.lastLog = *rs->getULong(10);
			charInfo.position = DataTypes::Vector3(*rs->getFloat(11), *rs->getFloat(12), *rs->getFloat(13));
			charInfo.shirtObjectID = *rs->getULong(14);
			charInfo.pantsObjectID = *rs->getULong(15);
			charInfo.uScore = *rs->getInt(16);
			charInfo.uLevel = *rs->getInt(17);
			charInfo.currency = *rs->getInt(18);
			charInfo.reputation = *rs->getInt(19);
			charInfo.health = *rs->getInt(20);
			charInfo.imagination = *rs->getInt(21);
			charInfo.armor = *rs->getInt(22);
			return charInfo;
		}
	}

	static void UpdateChar(Str_DB_CharInfo charInfo) {

		odbc::PreparedStatementRef stmt = conn->prepareStatement("UPDATE OPCRUX_GD.dbo.Characters SET name=?,pendingName=?,lastWorld=?,lastInstance=?,lastClone=?,lastLog=?,positionX=?,positionY=?,positionZ=?,uScore=?,uLevel=?,currency=?,reputation=?,health=?,imagination=?,armor=? WHERE objectID=?");

		stmt->setString(1, charInfo.name);
		stmt->setString(2, charInfo.pendingName);
		stmt->setShort(3, charInfo.lastWorld);
		stmt->setShort(4, charInfo.lastInstance);
		stmt->setInt(5, charInfo.lastClone);
		stmt->setULong(6, charInfo.lastLog);
		stmt->setFloat(7, charInfo.position.x);
		stmt->setFloat(8, charInfo.position.y);
		stmt->setFloat(9, charInfo.position.z);
		stmt->setInt(10, charInfo.uScore);
		stmt->setInt(11, charInfo.uLevel);
		stmt->setInt(12, charInfo.currency);
		stmt->setInt(13, charInfo.reputation);
		stmt->setInt(14, charInfo.health);
		stmt->setInt(15, charInfo.imagination);
		stmt->setInt(16, charInfo.armor);

		stmt->setULong(17, charInfo.objectID);

		size_t nAffectedRows = stmt->executeUpdate();
	}

	static unsigned long CreateCharStyle(
		int headColor, int head, int chestColor, int chest,
		int legs, int hairStyle, int hairColor, int leftHand,
		int rightHand, int eyebrowStyle, int eyesStyle, int mouthStyle
	) {
		long id = reserveCountedID(DBCOUNTERID::P_STYLE);

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SET IDENTITY_INSERT OPCRUX_GD.dbo.CharacterStyles ON;INSERT INTO OPCRUX_GD.dbo.CharacterStyles(id,headColor,head,chestColor,chest,legs,hairStyle,hairColor,leftHand,rightHand,eyebrowStyle,eyesStyle,mouthStyle) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)");

		stmt->setInt(1, id);
		stmt->setInt(2, headColor);
		stmt->setInt(3, head);
		stmt->setInt(4, chestColor);
		stmt->setInt(5, chest);
		stmt->setInt(6, legs);
		stmt->setInt(7, hairStyle);
		stmt->setInt(8, hairColor);
		stmt->setInt(9, leftHand);
		stmt->setInt(10, rightHand);
		stmt->setInt(11, eyebrowStyle);
		stmt->setInt(12, eyesStyle);
		stmt->setInt(13, mouthStyle);

		odbc::ResultSetRef rs = stmt->executeQuery();

		return id;
	}

	static unsigned long long CreateNewChar(
		unsigned long accountID, std::string customName, std::string genname, int headColor, int head, int chestColor, int chest,
		int legs, int hairStyle, int hairColor, int leftHand, int rightHand, int eyebrowStyle, int eyesStyle, int mouthStyle
	) {
		// Get Char Count
		int charCount = GetCharCount(accountID);

		// Check if char is creatable on char count
		if (charCount < 4) {

			// Default Valuse
			int lastWorld = 0;
			int lastInstance = 0;
			int lastClone = 0;
			unsigned long long lastLog = 0;
			DataTypes::Vector3 position = DataTypes::Vector3::zero();

			int shirtObjectLOT = 0;
			{
				FDB::RowTopHeader rth = Cache.getRows("ItemComponent");
				if (!rth.isRowHeaderValid()) throw new std::runtime_error("Invalid Row Header");
				for (int i = 0; i < rth.getRowCount(); ++i) {
					if (!rth.isValid(i)) continue;
					try {
						if (
							*reinterpret_cast<uint32_t*>(rth[i]["itemType"].getMemoryLocation()) == 15 &&
							*reinterpret_cast<uint32_t*>(rth[i]["color1"].getMemoryLocation()) == chestColor &&
							*reinterpret_cast<uint32_t*>(rth[i]["decal"].getMemoryLocation()) == chest &&
							*reinterpret_cast<uint32_t*>(rth[i]["isBOE"].getMemoryLocation()) == 1
							) {

							int componentID = *reinterpret_cast<uint32_t*>(rth[i][0].getMemoryLocation());
							shirtObjectLOT = CacheComponentsRegistry::FindID(componentID, 11);
							break;
						}
					}
					catch (std::runtime_error e) {
						Logger::log("Cache:ItemComponent", e.what(), LogType::ERR);
					}
				}
				if (shirtObjectLOT == 0) {
					Logger::log("DB-CreateNewChar", "Unable to find componentID for shirt.");
					return -1; // Fail
				}
			}

			int pantsObjectLOT = 0; {
				FDB::RowTopHeader rth = Cache.getRows("ItemComponent");
				for (int i = 0; i < rth.getRowCount(); ++i) {
					if (!rth.isValid(i)) continue;
					try {
						if (
							*reinterpret_cast<uint32_t*>(rth[i]["itemType"].getMemoryLocation()) == 7 &&
							*reinterpret_cast<uint32_t*>(rth[i]["color1"].getMemoryLocation()) == legs &&
							*reinterpret_cast<uint32_t*>(rth[i]["isBOE"].getMemoryLocation()) == 1
							) {
							int componentID = *reinterpret_cast<uint32_t*>(rth[i][0].getMemoryLocation());
							pantsObjectLOT = CacheComponentsRegistry::FindID(componentID, 11);
							break;
						}
					}
					catch (std::runtime_error e) {
						Logger::log("Cache:ItemComponent", e.what(), LogType::ERR);
					}
				}
				if (pantsObjectLOT == 0) {
					Logger::log("DB-CreateNewChar", "Unable to find componentID for pants.");
					return -1; // Fail
				}
			}
			int uScore = 0;
			int uLevel = 0;
			int currency = 0;
			int reputation = 0;
			int health = 4;
			int imagination = 0;
			int armor = 0;

			// Create style
			unsigned long styleID = CreateCharStyle(headColor, head, chestColor, chest, legs, hairStyle, hairColor, leftHand, rightHand, eyebrowStyle, eyesStyle, mouthStyle);

			// Create statistics
			unsigned long statsID = CreateCharStats(reserveCountedID(DBCOUNTERID::P_STATS));

			// Reserve objectID
			unsigned long long objectID = reserveCountedID(DBCOUNTERID::PLAYER);

			// Create default shirt/pants objects
			DatabaseModels::ItemModel shirtObject;
			shirtObject.attributes.SetBound(true);
			shirtObject.attributes.SetEquipped(true);
			shirtObject.templateID = shirtObjectLOT;
			shirtObject.slot = 0;
			shirtObject.tab = 0;
			shirtObject.count = 1;
			shirtObject.subkey = 0;
			shirtObject.ownerID = objectID;
			shirtObject.objectID = (1ULL << 60) + reserveCountedID(DBCOUNTERID::STATIC);

			DatabaseModels::ItemModel pantsObject = shirtObject;
			pantsObject.templateID = pantsObjectLOT;
			pantsObject.objectID = (1ULL << 60) + reserveCountedID(DBCOUNTERID::STATIC);
			pantsObject.slot = 1;

			AddItemToInventory(shirtObject);
			AddItemToInventory(pantsObject);

			// Create player
			odbc::PreparedStatementRef stmt = conn->prepareStatement("SET IDENTITY_INSERT OPCRUX_GD.dbo.Characters ON;INSERT INTO OPCRUX_GD.dbo.Characters(objectID,accountID,charIndex,name,pendingName,styleID,statsID,lastWorld,lastInstance,lastClone,lastLog,positionX,positionY,positionZ,shirtObjectID,pantsObjectID,uScore,uLevel,currency,reputation,health,imagination,armor) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

			stmt->setULong(1, objectID);
			stmt->setInt(2, accountID);
			stmt->setByte(3, charCount);
			stmt->setString(4, genname);
			stmt->setString(5, customName);
			stmt->setInt(6, styleID);
			stmt->setInt(7, statsID);
			stmt->setShort(8, lastWorld);
			stmt->setShort(9, lastInstance);
			stmt->setInt(10, lastClone);
			stmt->setULong(11, lastLog);
			stmt->setFloat(12, position.x);
			stmt->setFloat(13, position.y);
			stmt->setFloat(14, position.z);
			stmt->setULong(15, shirtObject.objectID);
			stmt->setULong(16, pantsObject.objectID);
			stmt->setInt(17, uScore);
			stmt->setInt(18, uLevel);
			stmt->setInt(19, currency);
			stmt->setInt(20, reputation);
			stmt->setInt(21, health);
			stmt->setInt(22, imagination);
			stmt->setInt(23, armor);

			odbc::ResultSetRef rs = stmt->executeQuery();

			return objectID;
		}

		return -1;
	}

	static unsigned long GetAccountIDByClientName(std::string clientName) {

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT id FROM OPCRUX_AD.dbo.Accounts WHERE username=?");

		stmt->setString(1, clientName);

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next())
			return *rs->getInt(1);


		return -1;
	}

	static std::uint64_t getStatByID(std::int64_t statsID, std::uint32_t statsIndex) {
		const std::string lookup[27] = {
			"currencyCollected",
			"bricksCollected",
			"objectsSmashed",
			"quickbuildsCompleted",
			"enemiesSmashed",
			"rocketsUsed",
			"missionsCompleted",
			"petsTamed",
			"imaginationCollected",
			"healthCollected",
			"armorCollected",
			"distanceTraveled",
			"smashed",
			"damageTaken",
			"damageHealed",
			"armorRepaired",
			"imaginationRestored",
			"imaginationUsed",
			"distanceDriven",
			"airborneTime",
			"racingImaginationCollected",
			"racingImaginationCratesSmashed",
			"racingBoostsActivated",
			"racingWrecks",
			"racingSmashablesSmashed",
			"racesFinished",
			"racesWon"
		};
		if (statsID >= 27) throw new std::runtime_error("Index out of range.");
		std::string statsName = lookup[statsID];
		return 0;
	}

	static bool HasMission(std::int64_t charID, std::int32_t missionID) {

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT COUNT(missionID) FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND missionID=?");

		stmt->setULong(1, charID);
		stmt->setInt(2, missionID);

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next())
			return *rs->getInt(1) != 0;


		return false;
	}

	/*
		Note: check with NOT Database::HasMission(...) before executing.
	*/
	static MissionModel AddMission(std::int64_t charID, std::int32_t missionID) {
		MissionModel model;
		model.charID = charID;
		model.missionID = missionID;
		model.state = 2;
		model.repeatCount = 0;
		model.time = time(0);
		model.chosenReward = -1;

		std::list<std::int32_t> initialProgress = {};
		auto mtr = CacheMissionTasks::getRow(missionID);
		while (mtr.isValid()) {
			initialProgress.push_back(0);
			if (!mtr.isLinkedRowInfoValid()) break;
			mtr = mtr.getLinkedRowInfo();
		}
		model.progress = StringUtils::IntListToString(initialProgress, '|');

		if (HasMission(charID, missionID)) {
			throw std::runtime_error("Mission already exists!");
		}

		odbc::PreparedStatementRef stmt = conn->prepareStatement("INSERT INTO OPCRUX_GD.dbo.Missions(charID,missionID,state,progress,repeatCount,time,chosenReward) VALUES(?,?,?,?,?,?,?)");

		stmt->setULong(1, model.charID);
		stmt->setInt(2, model.missionID);
		stmt->setInt(3, model.state);
		stmt->setString(4, model.progress);
		stmt->setInt(5, model.repeatCount);
		stmt->setULong(6, model.time);
		stmt->setInt(7, model.chosenReward);

		odbc::ResultSetRef rs = stmt->executeQuery();

		return model;
	}

	/*
		Note: check with Database::HasMission(...) before executing.
	*/
	static MissionModel GetMission(std::int64_t charID, std::int32_t missionID) {
		
		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND missionID=?");

		stmt->setULong(1, charID);
		stmt->setInt(2, missionID);

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			MissionModel model;
			model.charID = *rs->getULong(1);
			model.missionID = *rs->getInt(2);
			model.state = *rs->getInt(3);
			model.progress = *rs->getString(4);
			model.repeatCount = *rs->getInt(5);
			model.time = *rs->getULong(6);
			model.chosenReward = *rs->getInt(7);

			return model;
		}

		return MissionModel();
	}

	static std::list<MissionModel> GetAllMissions(std::int64_t charID) {
		
		std::list<MissionModel> retVal = {};
		
		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=?");

		stmt->setULong(1, charID);

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			MissionModel model;
			model.charID = *rs->getULong(1);
			model.missionID = *rs->getInt(2);
			model.state = *rs->getInt(3);
			model.progress = *rs->getString(4);
			model.repeatCount = *rs->getInt(5);
			model.time = *rs->getULong(6);
			model.chosenReward = *rs->getInt(7);
			
			retVal.push_back(model);
		}

		return retVal;
	}

	static std::list<MissionModel> GetAllMissionsByState(std::int64_t charID, std::int32_t state) {
		std::list<MissionModel> retVal = {};

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? and state=?");

		stmt->setULong(1, charID);
		stmt->setInt(2, state);

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			MissionModel model;
			model.charID = *rs->getULong(1);
			model.missionID = *rs->getInt(2);
			model.state = *rs->getInt(3);
			model.progress = *rs->getString(4);
			model.repeatCount = *rs->getInt(5);
			model.time = *rs->getULong(6);
			model.chosenReward = *rs->getInt(7);

			retVal.push_back(model);
		}

		return retVal;
	}

	static std::list<MissionModel> GetAllMissionsByStates(std::int64_t charID, std::list<std::int32_t> state) {
		std::list<MissionModel> retVal = {};

		std::string stmBuilder = "SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND state IN (";

		if (state.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < state.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ")";

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)stmBuilder.c_str(), SQL_NTS);

		odbc::PreparedStatementRef stmt = conn->prepareStatement(stmBuilder.c_str());

		stmt->setULong(1, charID);
		int pIndex = 2;
		for (auto elem : state) {
			stmt->setInt(pIndex++, elem);
		}

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			MissionModel model;
			model.charID = *rs->getULong(1);
			model.missionID = *rs->getInt(2);
			model.state = *rs->getInt(3);
			model.progress = *rs->getString(4);
			model.repeatCount = *rs->getInt(5);
			model.time = *rs->getULong(6);
			model.chosenReward = *rs->getInt(7);

			retVal.push_back(model);
		}

		return retVal;
	}

	static std::list<MissionModel> GetAllMissionsByIDs(std::int64_t charID, std::list<std::int32_t> missions) {
		std::list<MissionModel> retVal = {};

		std::string stmBuilder = "SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND missionID IN (";

		if (missions.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < missions.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ")";

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)stmBuilder.c_str(), SQL_NTS);

		odbc::PreparedStatementRef stmt = conn->prepareStatement(stmBuilder.c_str());

		stmt->setULong(1, charID);
		int pIndex = 2;
		for (auto elem : missions) {
			stmt->setInt(pIndex++, elem);
		}

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			MissionModel model;
			model.charID = *rs->getULong(1);
			model.missionID = *rs->getInt(2);
			model.state = *rs->getInt(3);
			model.progress = *rs->getString(4);
			model.repeatCount = *rs->getInt(5);
			model.time = *rs->getULong(6);
			model.chosenReward = *rs->getInt(7);

			retVal.push_back(model);
		}

		return retVal;
	}

	static std::list<MissionModel> GetAllMissionsByIDsAndState(std::int64_t charID, std::list<std::int32_t> missions, std::int32_t state) {
		std::list<MissionModel> retVal = {};

		std::string stmBuilder = "SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND state=? AND missionID IN (";

		if (missions.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < missions.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ")";

		SQLRETURN ret = SQLPrepare(sqlStmtHandle, (SQLCHAR*)stmBuilder.c_str(), SQL_NTS);

		odbc::PreparedStatementRef stmt = conn->prepareStatement(stmBuilder.c_str());

		stmt->setULong(1, charID);
		stmt->setInt(2, state);
		int pIndex = 3;
		for (auto elem : missions) {
			stmt->setInt(pIndex++, elem);
		}

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			MissionModel model;
			model.charID = *rs->getULong(1);
			model.missionID = *rs->getInt(2);
			model.state = *rs->getInt(3);
			model.progress = *rs->getString(4);
			model.repeatCount = *rs->getInt(5);
			model.time = *rs->getULong(6);
			model.chosenReward = *rs->getInt(7);

			retVal.push_back(model);
		}

		return retVal;
	}

	static std::list<MissionModel> GetAllMissionsByIDsAndStates(std::int64_t charID, std::list<std::int32_t> missions, std::list<std::int32_t> states) {
		std::list<MissionModel> retVal = {};

		std::string stmBuilder = "SELECT charID, missionID, state, progress, repeatCount, time, chosenReward FROM OPCRUX_GD.dbo.Missions WHERE charID=? AND missionID IN (";

		if (missions.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < missions.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ") AND state IN (";

		if (states.size() != 0)
			stmBuilder += "?";
		for (int i = 1; i < states.size(); ++i)
			stmBuilder += ",?";
		stmBuilder += ")";

		odbc::PreparedStatementRef stmt = conn->prepareStatement(stmBuilder.c_str());

		stmt->setULong(1, charID);
		int pIndex = 2;
		for (auto elem : missions) {
			stmt->setInt(pIndex++, elem);
		}
		for (auto elem : states) {
			stmt->setInt(pIndex++, elem);
		}

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			MissionModel model;
			model.charID = *rs->getULong(1);
			model.missionID = *rs->getInt(2);
			model.state = *rs->getInt(3);
			model.progress = *rs->getString(4);
			model.repeatCount = *rs->getInt(5);
			model.time = *rs->getULong(6);
			model.chosenReward = *rs->getInt(7);

			retVal.push_back(model);
		}

		return retVal;
	}

	/*
		Note: check with Database::HasMission(...) before executing.
	*/
	static void UpdateMission(MissionModel mission) {
		mission.time = time(0);

		odbc::PreparedStatementRef stmt = conn->prepareStatement("UPDATE OPCRUX_GD.dbo.Missions SET state=?,progress=?,repeatCount=?,time=?,chosenReward=? WHERE charID=? AND missionID=?");

		stmt->setInt(1, mission.state);
		stmt->setString(2, mission.progress);
		stmt->setInt(3, mission.repeatCount);
		stmt->setULong(4, mission.time);
		stmt->setInt(5, mission.chosenReward);
		
		stmt->setULong(6, mission.charID);
		stmt->setInt(7, mission.missionID);

		size_t numUpdatedRows = stmt->executeUpdate();
	}


	// UPDATE OPCRUX_GD.dbo.FlagChunks SET playerID=0, chunkID=0, chunkData=0; IF @@ROWCOUNT=0 INSERT INTO OPCRUX_GD.dbo.FlagChunks VALUES(0,0,1);
	static void SetFlag(DataTypes::LWOOBJID playerID, std::uint32_t chunkID, std::uint64_t chunkData) {
		std::uint64_t ppid = playerID.getPureID();

		odbc::PreparedStatementRef stmt = conn->prepareStatement("UPDATE OPCRUX_GD.dbo.FlagChunks SET chunkData=? WHERE playerID=? AND chunkID=?; IF @@ROWCOUNT=0 INSERT INTO OPCRUX_GD.dbo.FlagChunks(playerID,chunkID,chunkData) VALUES(?,?,?);");

		stmt->setULong(1, chunkData);
		stmt->setULong(2, ppid);
		stmt->setInt(3, chunkID);

		stmt->setULong(4, ppid);
		stmt->setInt(5, chunkID);
		stmt->setULong(6, chunkData);

		odbc::ResultSetRef rs = stmt->executeQuery();
	}

	static std::map<std::uint32_t, std::uint64_t> GetFlagChunks(std::int64_t charID) {
		std::map<std::uint32_t, std::uint64_t> retVal = {};

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT chunkID, chunkData FROM OPCRUX_GD.dbo.FlagChunks WHERE playerID=?");
		stmt->setULong(1, charID);
		
		odbc::ResultSetRef rs = stmt->executeQuery();

		while (rs->next())
			retVal.insert({ *rs->getUInt(1), *rs->getULong(2) });

		return retVal;
	}

	static std::map<std::int32_t, std::list<ItemModel>> GetFullInventory(std::int64_t charID) {
		const std::list<std::int32_t> tabs = { 0,1,2,4,5,6,7,8,12,14 };
		std::map<std::int32_t, std::list<ItemModel>> result;
		for (auto it = tabs.begin(); it != tabs.end(); ++it) {
			result.insert({ *it, GetInventoryItemsOfTab(charID, *it) });
		}
		return result;
	}

	static std::list<ItemModel> GetInventoryItemsOfTab(std::int64_t charID, std::int32_t _tab) {
		std::list<ItemModel> retVal = {};

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT objectID, ownerID, subkey, tab, slot, template, count, attributes, metadata FROM OPCRUX_GD.dbo.Inventory WHERE ownerID=? AND tab=?");

		stmt->setULong(1, charID);
		stmt->setInt(2, _tab);

		odbc::ResultSetRef rs = stmt->executeQuery();

		while (rs->next()) {
			ItemModel model;
			model.objectID = *rs->getULong(1);
			model.ownerID = *rs->getULong(2);
			model.subkey = *rs->getULong(3);
			model.tab = *rs->getInt(4);
			model.slot = *rs->getInt(5);
			model.templateID = *rs->getInt(6);
			model.count = *rs->getInt(7);
			model.attributes.SetAttributes(*rs->getUShort(8));
			std::string metadata = *rs->getString(9);
			model.metadata = LDFUtils::ParseCollectionFromWString(std::u16string(metadata.begin(), metadata.end()));

			retVal.push_back(model);
		}

		return retVal;
	}

	static void AddItemToInventory(ItemModel item) {
		odbc::PreparedStatementRef stmt = conn->prepareStatement("INSERT INTO OPCRUX_GD.dbo.Inventory (objectID, ownerID, subkey, tab, slot, template, count, attributes, metadata) VALUES (?,?,?,?,?,?,?,?,?)");

		stmt->setULong(1, item.objectID);
		stmt->setULong(2, item.ownerID);
		stmt->setULong(3, item.subkey);
		stmt->setInt(4, item.tab);
		stmt->setInt(5, item.slot);
		stmt->setInt(6, item.templateID);
		stmt->setInt(7, item.count);
		stmt->setUByte(8, item.attributes.GetAttributes());
		std::u16string u16metadata = LDFUtils::PackCollectionToWString(item.metadata);
		std::string metadata = std::string(u16metadata.begin(), u16metadata.end());
		stmt->setString(9, metadata);

		odbc::ResultSetRef rs = stmt->executeQuery();
	}

	static void UpdateItemFromInventory(ItemModel item) {
		odbc::PreparedStatementRef stmt = conn->prepareStatement("UPDATE OPCRUX_GD.dbo.Inventory SET objectID = ?, ownerID = ?, subkey = ?, tab = ?, slot = ?, template = ?, count = ?, attributes = ?, metadata = ? WHERE objectID = ?");

		stmt->setULong(1, item.objectID);
		stmt->setULong(2, item.ownerID);
		stmt->setULong(3, item.subkey);
		stmt->setInt(4, item.tab);
		stmt->setInt(5, item.slot);
		stmt->setInt(6, item.templateID);
		stmt->setInt(7, item.count);
		stmt->setUByte(8, item.attributes.GetAttributes());
		std::u16string u16metadata = LDFUtils::PackCollectionToWString(item.metadata);
		std::string metadata = std::string(u16metadata.begin(), u16metadata.end());
		stmt->setString(9, metadata);
		
		stmt->setULong(10, item.objectID);

		size_t numRowsAffected = stmt->executeUpdate();
	}

	static int GetAccountIDFromMinifigOBJID(DataTypes::LWOOBJID objid) {
		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT accountID FROM OPCRUX_GD.dbo.Characters WHERE objectID=?");

		stmt->setULong(1, objid.getPureID());

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			return *rs->getInt(1);
		}

		return -1;
	}

	static int GetAccountGMLevel(unsigned long accountID) {

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT rank FROM OPCRUX_AD.dbo.Accounts WHERE id=?");

		stmt->setUInt(1, accountID);

		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {
			return *rs->getInt(1);
		}

		return -1;
	}

	static Str_DB_CharStats GetCharStats(long charIndex) {
		SetupStatementHandle();

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SELECT statsID, TotalCurrencyCollected, TotalBricksCollected, TotalSmashablesSmashed, TotalQuickBuildsCompleted, TotalEnemiesSmashed, TotalRocketsUsed, TotalPetsTamed, TotalImaginationPowerUpsCollected, TotalLifePowerUpsCollected, TotalArmorPowerUpsCollected, TotalDistanceTraveled, TotalSuicides, TotalDamageTaken, TotalDamageHealed, TotalArmorRepaired, TotalImaginationRestored, TotalImaginationUsed, TotalDistanceDriven, TotalTimeAirborne, TotalRacingImaginationPowerUpsCollected, TotalRacecarBoostsActivated, TotalRacecarWrecks, TotalRacingSmashablesSmashed, TotalRacesFinished, TotalFirstPlaceFinishes FROM OPCRUX_GD.dbo.CharacterStats WHERE charIndex=?");
		
		stmt->setULong(1, charIndex);
		
		odbc::ResultSetRef rs = stmt->executeQuery();

		if (rs->next()) {

			Str_DB_CharStats charStats;

			int i = 0;

			charStats.TotalCurrencyCollected = *rs->getULong(i++);
			charStats.TotalBricksCollected = *rs->getULong(i++);
			charStats.TotalSmashablesSmashed = *rs->getULong(i++);
			charStats.TotalQuickBuildsCompleted = *rs->getULong(i++);
			charStats.TotalEnemiesSmashed = *rs->getULong(i++);
			charStats.TotalRocketsUsed = *rs->getULong(i++);
			charStats.TotalPetsTamed = *rs->getULong(i++);
			charStats.TotalImaginationPowerUpsCollected = *rs->getULong(i++);
			charStats.TotalLifePowerUpsCollected = *rs->getULong(i++);
			charStats.TotalArmorPowerUpsCollected = *rs->getULong(i++);
			charStats.TotalDistanceTraveled = *rs->getULong(i++);
			charStats.TotalSuicides = *rs->getULong(i++);
			charStats.TotalDamageTaken = *rs->getULong(i++);
			charStats.TotalDamageHealed = *rs->getULong(i++);
			charStats.TotalArmorRepaired = *rs->getULong(i++);
			charStats.TotalImaginationRestored = *rs->getULong(i++);
			charStats.TotalImaginationUsed = *rs->getULong(i++);
			charStats.TotalDistanceDriven = *rs->getULong(i++);
			charStats.TotalTimeAirborne = *rs->getULong(i++);
			charStats.TotalRacingImaginationPowerUpsCollected = *rs->getULong(i++);
			charStats.TotalRacecarBoostsActivated = *rs->getULong(i++);
			charStats.TotalRacecarWrecks = *rs->getULong(i++);
			charStats.TotalRacingSmashablesSmashed = *rs->getULong(i++);
			charStats.TotalRacesFinished = *rs->getULong(i++);
			charStats.TotalFirstPlaceFinishes = *rs->getULong(i++);

			return charStats;
		}

		return {};
	}

	static unsigned long CreateCharStats(long statsID) {

		odbc::PreparedStatementRef stmt = conn->prepareStatement("SET IDENTITY_INSERT OPCRUX_GD.dbo.CharacterStats ON;INSERT INTO OPCRUX_GD.dbo.CharacterStats (statsID, TotalCurrencyCollected, TotalBricksCollected, TotalSmashablesSmashed, TotalQuickBuildsCompleted, TotalEnemiesSmashed, TotalRocketsUsed, TotalPetsTamed, TotalImaginationPowerUpsCollected, TotalLifePowerUpsCollected, TotalArmorPowerUpsCollected, TotalDistanceTraveled, TotalSuicides, TotalDamageTaken, TotalDamageHealed, TotalArmorRepaired, TotalImaginationRestored, TotalImaginationUsed, TotalDistanceDriven, TotalTimeAirborne, TotalRacingImaginationPowerUpsCollected, TotalRacecarBoostsActivated, TotalRacecarWrecks, TotalRacingSmashablesSmashed, TotalRacesFinished, TotalFirstPlaceFinishes) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

		stmt->setULong(1, statsID);

		for (int i = 2; i <= 28; ++i)
			stmt->setULong(i, 0ull);

		odbc::ResultSetRef rs = stmt->executeQuery();
	}
};
#endif // !__DATABASE_HPP__
