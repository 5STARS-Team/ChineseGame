#include "cocos/network/HttpRequest.h"
#include "cocos/network/HttpClient.h"
#include <list>
#include "json/document.h"

USING_NS_CC;
using namespace network;
using namespace std;
using namespace rapidjson;
class Mod_Game : public HttpRequest{
public:
	static Mod_Game * getInstance(){
		static Mod_Game s_Mod_Game;
		return &s_Mod_Game;
	}
	struct Server{
		unsigned int serverId;
		std::string url;
	}myServer;
	bool init();
	void sendMsg(rapidjson::Value &data);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
};
