#pragma once
#ifndef _DASHBOARDRESOURCES_HPP__
#define _DASHBOARDRESOURCES_HPP__

#include <Mongoose/mongoose.h>
class DashboardResources {
public:
	static int handleResources(struct mg_connection *nc, int ev, struct http_message *hm) {
		return 0;
	}
};

#endif // !_DASHBOARDRESOURCES_HPP__