#ifndef __SERVER__UGCSERVER_HPP__
#define __SERVER__UGCSERVER_HPP__

#include "Interfaces/ILUServer.hpp"

#include "Rendering/UGCRenderer.hpp"

class UGCServer : public ILUServer {
	UGCRenderer renderer;

public:
	UGCServer();
	~UGCServer();
};

#endif // !__SERVER__UGCSERVER_HPP__
