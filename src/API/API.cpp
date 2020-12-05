#include <iostream>
#include <Mongoose/mongoose.h>
#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <vector>

#include "Utils/ServerInfo.hpp"
#include "Utils/StringUtils.hpp"
#include "../libs/json/json.hpp"
#include "Utils/Logger.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "Database/Database.hpp"

static struct mg_serve_http_opts s_http_server_opts;
static std::vector<std::string> keys = {};
using json = nlohmann::json;

#define SSL_PRIVATE "res/ssl/private.pem"
#define SSL_PUBLIC "res/ssl/public.pem"
#define SSL_CA "res/ssl/ca.pem"

const char * GetPort() {
	if (Configuration::ConfigurationManager::dbConf.GetStringVal("API", "USESSL") == "TRUE") {
		return "8443";
	}
	else {
		return "8080";
	}
}

void loadTokens() {
	const std::string& path = "conf/keys.txt";
	struct stat buffer;
	std::ifstream file("conf/keys.txt");
	std::string str;
	while (std::getline(file, str)) {
		if (str.length() == 0)
			continue;
		if (str[0] == ' ')
			continue;
		if (str[0] == '\t')
			continue;
		if (str[0] == '\r')
			continue;
		keys.push_back(str);
	}
}

bool checkToken(std::string token) {
	if (std::find(keys.begin(), keys.end(), token) != keys.end()) {
		return true;
	}
	else {
		return false;
	}
}

std::string mg_str2string(mg_str* string) {
	return std::string(string->p, string->len);
}

inline std::string_view sw_str(mg_str _mstr) {
	return std::string_view(_mstr.p, _mstr.len);
}

inline bool sw_pcmp(mg_str _mstr, const char* _cstr) {
	if (_mstr.p[_mstr.len - 1] == '/') {
		return std::string_view(_mstr.p, _mstr.len - 1) == _cstr;
	}
	return sw_str(_mstr) == _cstr;
}

void mg_parse_form_data(http_message* hm, std::unordered_map<std::string, std::string_view>& form_data) {
	char var_name[100], file_name[100];
	const char* chunk;
	size_t chunk_len, n1, n2;

	n1 = n2 = 0;
	while ((n2 = mg_parse_multipart(hm->body.p + n1,
		hm->body.len - n1,
		var_name, sizeof(var_name),
		file_name, sizeof(file_name),
		&chunk, &chunk_len)) > 0) {
		form_data.insert({ std::string(var_name, strlen(var_name)), std::string_view(chunk, chunk_len) });
		n1 += n2;
	}
}

static std::string HandleJson(int code, int arguement, void* p) {
	json j;
	struct http_message* hm = (struct http_message*)p;
	if (code == 0) {
		Logger::log("WebAPI", std::string(hm->uri.p, hm->uri.len));
		j["Status"] = "Success";
		j["Result"] = arguement;
	}
	else if (code == 1) {
		Logger::log("WebAPI", std::string(hm->uri.p, hm->uri.len), LogType::WARN);
		j["Status"] = "Fail";
		j["Error"] = arguement;
	}
	return j.dump();
}

static std::string HandleJson(int code, std::string arguement, void* p) {
	json j;
	struct http_message* hm = (struct http_message*)p;
	if (code == 0) {
		Logger::log("WebAPI", std::string(hm->uri.p, hm->uri.len));
		j["Status"] = "Success";
		j["Result"] = arguement;
	}
	else if (code == 1) {
		Logger::log("WebAPI", std::string(hm->uri.p, hm->uri.len), LogType::WARN);
		j["Status"] = "Fail";
		j["Error"] = arguement;
	}
	return j.dump();
}

static std::string HandleJson(int code, const char* arguement, void* p) {
	json j;
	struct http_message* hm = (struct http_message*)p;
	if (code == 0) {
		Logger::log("WebAPI", std::string(hm->uri.p, hm->uri.len));
		j["Status"] = "Success";
		j["Result"] = arguement;
	}
	else if (code == 1) {
		Logger::log("WebAPI", std::string(hm->uri.p, hm->uri.len), LogType::WARN);
		j["Status"] = "Fail";
		j["Error"] = arguement;
	}
	return j.dump();
}

static std::string HandleJson(void* p) {
	json j;
	struct http_message* hm = (struct http_message*)p;
	Logger::log("WebAPI", std::string(hm->uri.p, hm->uri.len), LogType::WARN);
	j["Status"] = "Fail";
	return j.dump();
}


static void ev_handler(struct mg_connection* c, int ev, void* p) {
	if (ev == MG_EV_HTTP_REQUEST) {
		struct http_message* hm = (struct http_message*)p;

		std::string url = mg_str2string(&hm->uri);
		std::vector<std::string> args = StringUtils::splitString(url.erase(0, 1), '/');

		if (sw_str(hm->method) != "GET" && sw_str(hm->method) != "POST") {
			std::string msg = HandleJson(0, "405 Method Not Allowed", p);
			mg_send_head(c, 405, msg.length(), "Content-Type: text/plain");
			mg_printf(c, "%.*s", msg.length(), msg.c_str());
			return;
		}

		std::unordered_map<std::string, std::string_view> form_data = {};
		mg_parse_form_data(hm, form_data);
		if (args.size() == 0) { return; }
		if (args[0] == "favicon.ico") { return; }

		if (checkToken(args[0])) {
			if (args[1] == "getprocessname") {
				std::string msg = HandleJson(0, ServerInfo::baseTitle, p);
				mg_send_head(c, 200, msg.length(), "Content-Type: text/plain");
				mg_printf(c, "%.*s", msg.length(), msg.c_str());
				return;
			}
			else if (args[1] == "getBannedStatus") { // Implement this
				if (args.size() > 2) {
					bool isBanned = true;
					// bool isBanned = Database::GetAccountBannedStatus(args[1]);
					std::string msg = HandleJson(0, int(isBanned), p);
					mg_send_head(c, 200, msg.length(), "Content-Type: text/plain");
					mg_printf(c, "%.*s", msg.length(), msg.c_str());
					return;
				}
				else {
					std::string msg = HandleJson(1, "This API call requires arguements", p);
					mg_send_head(c, 400, msg.length(), "Content-Type: text/plain");
					mg_printf(c, "%.*s", msg.length(), msg.c_str());
					return;
				}
			}
			else if (args[1] == "logincheck") {
				if (args.size() > 3) {
					auto conn = Database::Connect();
					bool result = Database::APILoginCheck(conn, args[2], args[3]);
					Database::Disconnect(conn);
					std::string msg = HandleJson(0, StringUtils::IntToString(int(result)), p);
					mg_send_head(c, 200, msg.length(), "Content-Type: text/plain");
					mg_printf(c, "%.*s", msg.length(), msg.c_str());
					return;
				}
				else {
					std::string msg = HandleJson(1, "This API call requires arguements", p);
					mg_send_head(c, 400, msg.length(), "Content-Type: text/plain");
					mg_printf(c, "%.*s", msg.length(), msg.c_str());
					return;
				}
			}
			std::string msg = HandleJson(1, "421 Misdirected Request", p);
			mg_send_head(c, 421, msg.length(), "Content-Type: text/plain");
			mg_printf(c, "%.*s", msg.length(), msg.c_str());
		}
		else {
			std::string msg = HandleJson(0, "403 Forbidden needs correct Token", p);
			mg_send_head(c, 403, msg.length(), "Content-Type: text/plain");
			mg_printf(c, "%.*s", msg.length(), msg.c_str());
		}
	}
}

//void StartAPIWithSSL() {
//	struct mg_mgr mgr;
//	struct mg_connection* c;
//	struct mg_bind_opts bind_opts;
//	const char* err;
//
//	loadTokens();
//
//	mg_mgr_init(&mgr, NULL);
//
//	setlocale(LC_TIME, "en_US.UTF-8");
//
//	memset(&bind_opts, 0, sizeof(bind_opts));
//	bind_opts.ssl_cert = SSL_PUBLIC;
//	bind_opts.ssl_key = SSL_PRIVATE;
//
//	bind_opts.error_string = &err;
//	printf("[WebAPI] Starting SSL server on port %s, cert from %s, key from %s\n", GetPort(), SSL_PUBLIC, SSL_PRIVATE);
//
//	c = mg_bind_opt(&mgr, GetPort(), ev_handler, bind_opts);
//
//	if (c == nullptr) {
//		throw std::runtime_error("Failed to create WebAPI listener.");
//	}
//
//	s_http_server_opts.document_root = ".";  // Serve current directory
//
//	mg_set_protocol_http_websocket(c);
//
//	Logger::log("WebAPI", "Started Up WebAPI.....");
//
//	for (;;) {
//		mg_mgr_poll(&mgr, 1000);
//	}
//	mg_mgr_free(&mgr);
//} I still can't seem to work this out not entirely sure what the issue is.

void StartAPI() {
	struct mg_mgr mgr;
	struct mg_connection* c;
	struct mg_bind_opts bind_opts;
	const char* err;

	loadTokens();

	mg_mgr_init(&mgr, NULL);

	setlocale(LC_TIME, "en_US.UTF-8");

	c = mg_bind(&mgr, GetPort(), ev_handler);

	if (c == nullptr) {
		throw std::runtime_error("Failed to create WebAPI listener.");
	}

	s_http_server_opts.document_root = ".";  // Serve current directory

	mg_set_protocol_http_websocket(c);

	Logger::log("WebAPI", "Started Up WebAPI.....");

	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);
}
