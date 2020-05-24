#include "DashboardAPI.hpp"

#include <json/json.hpp>
#include "Utils/base64.hpp"
#include "DashboardTools.hpp"
#include "Utils/ServerInfo.hpp"
#include "Server/MasterServer.hpp"
#include "Server/WorldServer.hpp"
#include <sstream>
#include <ios>

#include "GameCache/Objects.hpp"
#include "GameCache/RenderComponent.hpp"
#include <Entity/Components/RenderComponent.hpp>
#include "Entity/Components/ControllablePhysicsComponent.hpp"

extern WorldServer* viewWs;

using namespace nlohmann;
typedef json JSON;

void DashboardAPI::WEBINT2(RakNet::BitStream & bs) {
	if (viewWs == nullptr) {
		bs.Write(0);
		return;
	}

	bs.Write(viewWs->GetZoneID());
	bs.Write(viewWs->zoneControlObject->GetObjectID());

	auto object_list = viewWs->objectsManager->GetObjects();
	bs.Write<std::uint32_t>(object_list.size());

	for (auto it = object_list.begin(); it != object_list.end(); ++it) {
		Entity::GameObject* obj = *it;
		bs.Write(obj->GetObjectID());
		bs.Write(obj->GetLOT());

		std::string objName = obj->GetNameStr();
		if (objName.size() == 0) {
			objName = CacheObjects::GetName(obj->GetLOT());
		}

		StringUtils::writeStringToBitStream<uint16_t>(&bs, objName);
		bs.Write(obj->GetPosition());
		bs.Write(obj->GetRotation());

		std::string renderAsset = "";

		RenderComponent* renderComponent = obj->GetComponent<RenderComponent>();
		if (renderComponent) {
			renderAsset = renderComponent->GetRenderAsset();
		}

		StringUtils::writeStringToBitStream<uint16_t>(&bs, renderAsset);

		std::string physicsAsset = "";

		StringUtils::writeStringToBitStream<uint16_t>(&bs, physicsAsset);
	}
}

void DashboardAPI::WEBINT3(RakNet::BitStream& bs) {
	if (viewWs == nullptr) {
		bs.Write(0);
		return;
	}

	auto object_list = viewWs->objectsManager->GetObjects();
	

	std::uint32_t object_list_size_usable = 0;
	for (auto it = object_list.begin(); it != object_list.end(); ++it) {
		if ((*it)->GetComponent<ControllablePhysicsComponent>() == nullptr) continue;
		++object_list_size_usable;
	}
	bs.Write<std::uint32_t>(object_list_size_usable);
	for (auto it = object_list.begin(); it != object_list.end(); ++it) {
		Entity::GameObject* obj = *it;
		if (obj->GetComponent<ControllablePhysicsComponent>() == nullptr) continue;
		bs.Write(obj->GetObjectID());
		bs.Write(obj->GetPosition());
		bs.Write(obj->GetRotation());
	}
}

int DashboardAPI::handleAPIs(mg_connection * nc, int ev, http_message * hm) {
	JSON j = JSON();

	bool denyAccess = false;

	if (hm->uri.len >= 4 && std::string(hm->uri.p, hm->uri.len).substr(0, 4) == "/api") {

		std::vector<std::string> command = StringUtils::splitString(std::string(hm->uri.p, hm->uri.len).substr(4), '/');
		
		std::vector<std::string> cD; for (std::string c : command)
		if (c.size() > 0) cD.push_back(c);
		command = cD; cD = std::vector<std::string>();

		while (command.size() < 30) command.push_back(""); // give some free room

		bool accessGranted = DashboardTools::isLoggedIn(nc, hm);

		if (command[0] == "WEBINT2") {
			RakNet::BitStream bs;
			WEBINT2(bs);
			const char * buf = reinterpret_cast<const char*>(bs.GetData());
			auto len = bs.GetNumberOfBytesUsed();
			std::string de = base64::encode(std::vector<byte>(buf, buf + len));

			mg_send_head(nc, 200, de.size(), "Content-Type: text/plain");
			mg_printf(nc, "%.*s", (int)de.size(), de.c_str());
			return 1;
		}

		if (command[0] == "WEBINT3") {
			RakNet::BitStream bs;
			WEBINT3(bs);
			const char* buf = reinterpret_cast<const char*>(bs.GetData());
			auto len = bs.GetNumberOfBytesUsed();
			std::string de = base64::encode(std::vector<byte>(buf, buf + len));

			mg_send_head(nc, 200, de.size(), "Content-Type: text/plain");
			mg_printf(nc, "%.*s", (int)de.size(), de.c_str());
			return 1;
		}

		if (command[0] == "serve_res") {
			std::string resFileStr = "res";
			for (int i = 1; i < command.size(); ++i) {
				if(i != 0 && command[i].size() != 0) resFileStr += "/";

				// Don't leave res folder
				if(command[i] == "..") continue;
				resFileStr += command[i];
			}

			hm->uri.p = resFileStr.c_str();
			hm->uri.len = resFileStr.size();

			struct mg_serve_http_opts s_http_server_opts;
			s_http_server_opts.document_root = "res/";
			s_http_server_opts.ip_acl = NULL;

			mg_http_serve_file(nc, hm, resFileStr.c_str(), mg_mk_str("application/x-binary"), mg_mk_str(""));

			//mg_serve_http(nc, hm, s_http_server_opts);

			return 1;
		}

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
				std::vector<Machine> connected_machines = ServerInfo::masterServer->connected_machines;
				for (std::ptrdiff_t i = 0; i < connected_machines.size(); ++i) {
					//connected_machines[i].
					auto factory = j["master"]["connected_machines"][i];
					factory["dottedIP"] = connected_machines[i].dottedIP;
					factory["machineName"] = connected_machines[i].machineName;
					factory["machineOS"] = connected_machines[i].machineOS;
				}
					
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
