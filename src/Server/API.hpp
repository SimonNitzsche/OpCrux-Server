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

static std::vector<std::string> keys = {};

enum SuccessCodes {
	SUCCESS,
	FAILED,
};

using json = nlohmann::json;

class JSONWrapper {
public:
	std::string Value;

	JSONWrapper(SuccessCodes code, int arguement, void* p) {
		json j;
		struct http_message* hm = (struct http_message*)p;
		if (code == SuccessCodes::SUCCESS) {
			j["Status"] = "Success";
			j["Result"] = arguement;
		}
		else {
			j["Status"] = "Fail";
			j["Error"] = arguement;
		}
		Value = j.dump();
	}

	JSONWrapper(SuccessCodes code, std::string arguement, void* p) {
		json j;
		struct http_message* hm = (struct http_message*)p;
		if (code == SuccessCodes::SUCCESS) {
			j["Status"] = "Success";
			j["Result"] = arguement;
		}
		else {
			j["Status"] = "Fail";
			j["Error"] = arguement;
		}
		Value = j.dump();
	}

	JSONWrapper(SuccessCodes code, const char* arguement, void* p) {
		json j;
		struct http_message* hm = (struct http_message*)p;
		if (code == SuccessCodes::SUCCESS) {
			j["Status"] = "Success";
			j["Result"] = arguement;
		}
		else {
			j["Status"] = "Fail";
			j["Error"] = arguement;
		}
		Value = j.dump();
	}
};

class APIServer {
private:
	struct mg_serve_http_opts s_http_server_opts;

	const char* SSL_PRIVATE = "res/ssl/private.pem";
	const char* SSL_PUBLIC = "res/ssl/public.pem";
	const char* SSL_CA = "res/ssl/ca.pem";

	const char* Port = nullptr;

	static void Handler(struct mg_connection* c, int ev, void* p) {
		if (ev == MG_EV_HTTP_REQUEST) {
			struct http_message* hm = (struct http_message*)p;

			std::string url = std::string(*reinterpret_cast<const char**>(&hm->uri), hm->uri.len);
			std::vector<std::string> args = StringUtils::splitString(url.erase(0, 1), '/');

			if (std::string(*reinterpret_cast<const char**>(&hm->method), 3) != "GET" && std::string(*reinterpret_cast<const char**>(&hm->method), 4) != "POST") {
				std::string msg = JSONWrapper(SuccessCodes::FAILED, "405 Method Not Allowed", p).Value;
				mg_send_head(c, 405, msg.length(), "Content-Type: text/plain");
				mg_printf(c, "%.*s", msg.length(), msg.c_str());
				return;
			}

			std::unordered_map<std::string, std::string_view> form_data = {};
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

			if (args.size() == 0) { return; }
			if (args[0] == "favicon.ico") { return; }

			if (std::find(keys.begin(), keys.end(), args[0]) != keys.end()) {
				url.erase(0, args[0].length() + 1);
				Logger::log("WebAPI", std::string("Recieved request for ") + std::string("/") + std::string(args[0].length(), "*"[0]) + "/" + url);
				if (args[1] == "getprocessname") { // Sample request
					std::string msg = JSONWrapper(SuccessCodes::SUCCESS, ServerInfo::baseTitle, p).Value;
					mg_send_head(c, 200, msg.length(), "Content-Type: text/plain");
					mg_printf(c, "%.*s", msg.length(), msg.c_str());
					return;
				}
				std::string msg = JSONWrapper(SuccessCodes::FAILED, "421 Misdirected Request", p).Value;
				mg_send_head(c, 421, msg.length(), "Content-Type: text/plain");
				mg_printf(c, "%.*s", msg.length(), msg.c_str());
			}
			else {
				std::string msg = JSONWrapper(SuccessCodes::FAILED, "403 Forbidden needs correct Token", p).Value;
				mg_send_head(c, 403, msg.length(), "Content-Type: text/plain");
				mg_printf(c, "%.*s", msg.length(), msg.c_str());
			}
		}
	}

public:
	APIServer() {
#if MG_ENABLE_SSL
		Port = "8443";
#else
		Port = "8080";
#endif

		if (keys.empty()) {
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
	}

	void Start() {
#if MG_ENABLE_SSL
		struct mg_mgr mgr;
		struct mg_connection* c;
		struct mg_bind_opts bind_opts;
		const char* err;

		loadTokens();

		mg_mgr_init(&mgr, NULL);

		setlocale(LC_TIME, "en_US.UTF-8");

		memset(&bind_opts, 0, sizeof(bind_opts));
		bind_opts.ssl_cert = SSL_PUBLIC;
		bind_opts.ssl_key = SSL_PRIVATE;

		bind_opts.error_string = &err;
		printf("[WebAPI] Starting SSL server on port %s, cert from %s, key from %s\n", GetPort(), SSL_PUBLIC, SSL_PRIVATE);

		c = mg_bind_opt(&mgr, GetPort(), ev_handler, bind_opts);

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
#else
		struct mg_mgr mgr;
		struct mg_connection* c;
		struct mg_bind_opts bind_opts;
		const char* err;

		mg_mgr_init(&mgr, NULL);

		setlocale(LC_TIME, "en_US.UTF-8");

		c = mg_bind(&mgr, Port, Handler);

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
#endif
	}
};