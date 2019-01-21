#ifndef __WEBINTERFACE__WEBINTERFACE_HPP__
#define __WEBINTERFACE__WEBINTERFACE_HPP__

#include <time.h>

#include "Utils/Logger.hpp"
#include "DashboardTools.hpp"
#include "DashboardAPI.hpp"

namespace WebInterface {
	struct mg_serve_http_opts s_http_server_opts;
	void ev_handler(struct mg_connection *nc, int ev, void* p);
	void handle_main(struct mg_connection *nc, int ev, struct http_message *hm);
	void handle_404(struct mg_connection *nc, int ev, struct http_message *hm);

	void WebInterfaceLoop() {
		struct mg_mgr mgr;
		struct mg_connection *nc;

		static const char *s_http_port = "3000";

		setlocale(LC_TIME, "en_US.UTF-8");

		Logger::log("WebI", "Starting WebInterface...");
		mg_mgr_init(&mgr, nullptr);
		nc = mg_bind(&mgr, s_http_port, ev_handler);
		if (nc == nullptr) {
			throw std::runtime_error("Failed to create Webinterface listener.");
		}

		mg_set_protocol_http_websocket(nc);
		s_http_server_opts.document_root = DashboardTools::resPath();
		s_http_server_opts.enable_directory_listing = "yes";

		for (;;) {
			mg_mgr_poll(&mgr, 1000);
		}
		mg_mgr_free(&mgr);
	}

	void ev_handler(struct mg_connection *nc, int ev, void *p) {
		if (ev == MG_EV_HTTP_REQUEST) {
			struct http_message *hm = static_cast<struct http_message*>(p);

			std::string url = std::string(hm->uri.p, hm->uri.len);
			Logger::log("WebI", "Requested: " + url);

			if ((url.substr(0, 10) == "/resources" && (url.length() == 10 || url.substr(10, 1) == "/")) || url == "/favicon.ico") {
				mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
				return;
			}

			if (url == "/") {
				handle_main(nc, ev, hm); return;
			}

			if (DashboardAPI::handleAPIs(nc, ev, hm) == 1) return;

			handle_404(nc, ev, hm);
		}
	}

	void handle_main(struct mg_connection *nc, int ev, struct http_message *hm) {
		DashboardTools::sendHeaderResponse200(nc);

		std::string content = DashboardTools::getPageContent("Pages/home.html");
		mg_printf_http_chunk(nc, "%s", content.c_str());
		mg_send_http_chunk(nc, "", 0);
	}

	void handle_404(struct mg_connection *nc, int ev, struct http_message *hm) {
		Logger::log("WebI", "Triggered 404 event.");
		DashboardTools::sendHeaderResponse404(nc);

		mg_printf_http_chunk(nc, "%s", DashboardTools::getErrHTML(404).c_str());
		mg_send_http_chunk(nc, "", 0);
	}
}

#endif // !__WEBINTERFACE__WEBINTERFACE_HPP__
