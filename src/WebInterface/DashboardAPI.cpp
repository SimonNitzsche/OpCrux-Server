#include "DashboardAPI.hpp"
#include <json/json.hpp>
#include "DashboardTools.hpp"
#include "Utils/ServerInfo.hpp"
#include <sstream>
#include <ios>

using namespace nlohmann;
typedef json JSON;

int DashboardAPI::handleAPIs(mg_connection * nc, int ev, http_message * hm) {
	JSON j = JSON();

	bool denyAccess = false;

	if (hm->uri.len >= 4 && std::string(hm->uri.p, hm->uri.len).substr(0, 4) == "/api") {

		std::vector<std::string> command = StringUtils::splitString(std::string(hm->uri.p, hm->uri.len).substr(4), '/');
		
		std::vector<std::string> cD; for each(std::string c in command)
		if (c.size() > 0) cD.push_back(c);
		command = cD; cD = std::vector<std::string>();

		while (command.size() < 30) command.push_back(""); // give some free room

		bool accessGranted = DashboardTools::isLoggedIn(nc, hm);

		if (command[0] == "isLoggedIn") {
			j["isLoggedIn"] = accessGranted;
		}

		else if (accessGranted) {
			// All the internal commands
			if (command[0] == "general") {
				if (command[1] == "online") {
					bool answ = (command.size() >= 3 && command[2] == "answer");
					if (answ) j["online"] = "yes";
					else j["online"] = true;
				}
				else {
					denyAccess = true;
				}
			}
			else if (command[0] == "sessions") {
				if (command[1] == "online") {
					throw std::runtime_error("TODO: Implement that.");
					//j = SessionsPool::to_json();
				}
				else {
					denyAccess = true;
				}
			}
			else if (command[0] == "master") {
				j["machine"]["machine_name"] = ServerInfo::getComputerName();
				j["machine"]["machine_os"] = ServerInfo::getOsName();
				j["master"]["connected_machines"] = 0;
			}
			else if (command[0] == "machines") {

			}
			else {
				denyAccess = true;
			}

			denyAccess = denyAccess || !accessGranted;
		}

		if (denyAccess) {
			mg_printf(
				nc, "%s",
				"HTTP/1.1 403 Access denied\r\n"
				"Content-Type: text/html\r\n"
				"Transfer-Encoding: chunked\r\n"
				"\r\n"
			);

			j = JSON();
			j["msg"] = "error";
			j["error"]["code"] = 403;
			j["error"]["text"] = "Access denied.";
		}
		else {
			mg_printf(
				nc, "%s",
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"Transfer-Encoding: chunked\r\n"
				"\r\n"
			);
		}
		std::stringstream js; js << j;
		mg_printf_http_chunk(nc, "%s", js.str().c_str());
		mg_send_http_chunk(nc, "", 0);
		return 1;
	}

	return 0;
}
