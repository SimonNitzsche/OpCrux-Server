#ifndef __WEBINTERFACE__DASHBOARDAPI_HPP__
#define __WEBINTERFACE__DASHBOARDAPI_HPP__

#include <Mongoose/mongoose.h>

class DashboardAPI {
public:
	static int handleAPIs(struct mg_connection *nc, int ev, struct http_message *hm);
};

#endif // !__WEBINTERFACE__DASHBOARDAPI_HPP__
