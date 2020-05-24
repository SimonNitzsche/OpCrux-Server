#ifndef __WEBINTERFACE__DASHBOARDAPI_HPP__
#define __WEBINTERFACE__DASHBOARDAPI_HPP__

#include <Mongoose/mongoose.h>
#include <RakNet/BitStream.h>
class DashboardAPI {
public:
	static void WEBINT2(RakNet::BitStream& bs);
	static void WEBINT3(RakNet::BitStream& bs);
	static int handleAPIs(struct mg_connection *nc, int ev, struct http_message *hm);
};

#endif // !__WEBINTERFACE__DASHBOARDAPI_HPP__
