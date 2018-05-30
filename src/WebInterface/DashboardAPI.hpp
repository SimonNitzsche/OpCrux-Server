#pragma once
#ifndef _DASHBOARDAPI_HPP__
#define _DASHBOARDAPI_HPP__

#include <Mongoose/mongoose.h>
class DashboardAPI {
public:
	static int handleAPIs(struct mg_connection *nc, int ev, struct http_message *hm);
};

#endif // !_DASHBOARDAPI_HPP__