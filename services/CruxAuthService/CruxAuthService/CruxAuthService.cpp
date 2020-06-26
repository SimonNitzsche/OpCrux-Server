#include <iostream>

#include "../../../libs/Mongoose/mongoose.h"
#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <fstream>

#define AUTH_PORT 22390
#define SSL_PRIVATE "res/ssl/private.pem"
#define SSL_PUBLIC "res/ssl/public.pem"
#define SSL_CA "res/ssl/ca.pem"
#define REQUEST_KEYS "res/appkeys.txt"

#include "DBInterface.h"

static const char* s_http_port = "8443";

static struct mg_serve_http_opts s_http_server_opts;
static std::vector<std::string> app_keys = {};

inline std::string_view sw_str(mg_str _mstr) {
	return std::string_view(_mstr.p, _mstr.len);
}

inline bool sw_pcmp(mg_str _mstr, const char* _cstr) {
	if (_mstr.p[_mstr.len - 1] == '/') {
		return std::string_view(_mstr.p, _mstr.len-1) == _cstr;
	}
	return sw_str(_mstr) == _cstr;
}

inline bool keyvalid(const char* key, size_t keylen) {
	std::string keystr = std::string(key, keylen);
	for (int i = 0; i < app_keys.size(); ++i) {
		if (app_keys.at(i) == keystr) 
			return true;
	}
	return false;
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

static void ev_handler(struct mg_connection* c, int ev, void* p) {
	//std::cout << "ev: " << ev << std::endl;
	if (ev == MG_EV_HTTP_REQUEST) {
		struct http_message* hm = (struct http_message*) p;

		if (sw_str(hm->method) != "POST") {
			const char* msg = "405 Method Not Allowed";
			mg_send_head(c, 405, strlen(msg), "Content-Type: text/plain");
			mg_printf(c, "%.*s", strlen(msg), msg);
			return;
		}

		if (sw_pcmp(hm->uri, "/auth")) {
			
			std::unordered_map<std::string, std::string_view> form_data = {};
			mg_parse_form_data(hm, form_data);

			auto it_appkey = form_data.find("application-key");
			if (it_appkey == form_data.end() || !keyvalid(it_appkey->second.data(), it_appkey->second.size())) {
				const char* msg = "401 Unauthorized";
				mg_send_head(c, 401, strlen(msg), "Content-Type: text/plain");
				mg_printf(c, "%.*s", strlen(msg), msg);
				return;
			}

			auto it_username = form_data.find("username");
			auto it_email = form_data.find("email");
			auto it_password = form_data.find("password");

			if ((it_username == form_data.end() && it_email == form_data.end()) || it_password == form_data.end()) {
				const char* msg = "400 Bad Request";
				mg_send_head(c, 400, strlen(msg), "Content-Type: text/plain");
				mg_printf(c, "%.*s", strlen(msg), msg);
				return;
			}

			bool isLoginCorrect = (it_username == form_data.end())
				? DBInterface::IsLoginCorrectEmail(it_email->second, it_password->second)
				: DBInterface::IsLoginCorrect(it_username->second, it_password->second);

			std::string msg = isLoginCorrect ? "PASS" : "FAIL";
			if ((it_username == form_data.end())) it_username = it_email;
			std::cout <<"\nAuth for user " << std::string(it_username->second.data(), it_username->second.size()) << " " << (isLoginCorrect ? "passed" : "failed") << "." << std::endl;
			mg_send_head(c, 200, msg.length(), "Content-Type: text/plain");
			mg_printf(c, "%.*s", msg.length(), msg.c_str());
			return;
		}

		

		const char* msg = "421 Misdirected Request";
		mg_send_head(c, 421, strlen(msg), "Content-Type: text/plain");
		mg_printf(c, "%.*s", strlen(msg), msg);
		//mg_serve_http(c, (struct http_message*) p, s_http_server_opts);
	}
}

inline int loadKeys() {

	std::fstream file;
	file.open(REQUEST_KEYS, std::ios::in);

	if (!file.is_open()) {
		return -1;
	}

	std::string key;
	while (getline(file, key)) {
		if (key.length() == 0)
			continue;
		if (key[0] == ' ')
			continue;
		if (key[0] == '\t')
			continue;
		if (key[0] == '\r')
			continue;
		app_keys.push_back(key);
	}
	return 0;
}

int main() {
	struct mg_mgr mgr;
	struct mg_connection* c;
	struct mg_bind_opts bind_opts;
	const char* err;

	mg_mgr_init(&mgr, NULL);

	setlocale(LC_TIME, "en_US.UTF-8");

	int klr = loadKeys();
	if (klr == -1) {
		printf("Unable to load application keys.");
		return klr;
	}

	DBInterface::Connect();

#ifndef _DEBUG
	memset(&bind_opts, 0, sizeof(bind_opts));
	bind_opts.ssl_cert = SSL_PUBLIC;
	bind_opts.ssl_key = SSL_PRIVATE;
	//bind_opts.ssl_ca_cert = SSL_CA;
	//bind_opts.ssl_cipher_suites = "TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:DHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:DHE-RSA-CHACHA20-POLY1305:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:DHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA384:DHE-RSA-AES256-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES256-SHA:ECDHE-RSA-AES256-SHA:DHE-RSA-AES256-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES128-SHA:DHE-RSA-AES128-SHA:RSA-PSK-AES256-GCM-SHA384:DHE-PSK-AES256-GCM-SHA384:RSA-PSK-CHACHA20-POLY1305:DHE-PSK-CHACHA20-POLY1305:ECDHE-PSK-CHACHA20-POLY1305:AES256-GCM-SHA384:PSK-AES256-GCM-SHA384:PSK-CHACHA20-POLY1305:RSA-PSK-AES128-GCM-SHA256:DHE-PSK-AES128-GCM-SHA256:AES128-GCM-SHA256:PSK-AES128-GCM-SHA256:AES256-SHA256:AES128-SHA256:ECDHE-PSK-AES256-CBC-SHA384:ECDHE-PSK-AES256-CBC-SHA:SRP-RSA-AES-256-CBC-SHA:SRP-AES-256-CBC-SHA:RSA-PSK-AES256-CBC-SHA384:DHE-PSK-AES256-CBC-SHA384:RSA-PSK-AES256-CBC-SHA:DHE-PSK-AES256-CBC-SHA:AES256-SHA:PSK-AES256-CBC-SHA384:PSK-AES256-CBC-SHA:ECDHE-PSK-AES128-CBC-SHA256:ECDHE-PSK-AES128-CBC-SHA:SRP-RSA-AES-128-CBC-SHA:SRP-AES-128-CBC-SHA:RSA-PSK-AES128-CBC-SHA256:DHE-PSK-AES128-CBC-SHA256:RSA-PSK-AES128-CBC-SHA:DHE-PSK-AES128-CBC-SHA:AES128-SHA:PSK-AES128-CBC-SHA256:PSK-AES128-CBC-SHA";
	
#endif
	bind_opts.error_string = &err;
	printf("Starting SSL server on port %s, cert from %s, key from %s\n",
		s_http_port, SSL_PUBLIC, SSL_PRIVATE);

#ifdef _DEBUG
	c = mg_bind(&mgr, s_http_port, ev_handler);
#else
	c = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);

	if (c == NULL) {
		printf("Failed to create listener: %s\n", err);
		return 1;
	}

#endif
	s_http_server_opts.document_root = ".";  // Serve current directory

	mg_set_protocol_http_websocket(c);

	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);

	return 0;
}
