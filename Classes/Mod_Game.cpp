#include "Mod_Game.h"
#include "XMLManager.h"
#include "Logic/GameLogic.h"
#include "json/document.h"

using namespace tinyxml2;

bool Mod_Game::init(){
	XMLManager xmlMgr;
	tinyxml2::XMLDocument *doc = xmlMgr.Init("Config.xml", "config");
	XMLElement *pinfo = doc->RootElement();
	if (pinfo == NULL)
	{
		CCLOG("not find first child element, element name: 'config'");
		return false;
	}
	XMLElement* serverIdXML = pinfo->FirstChildElement("selected_serverid");
	int selectedId = atoi(serverIdXML->Attribute("serverid"));
	//·þÎñÆ÷µØÖ·
	XMLElement *pchildinfo = pinfo->FirstChildElement("appHttps");
	if (!pchildinfo){
		CCLOG("attribute failed!, attribute name: 'appHttps'");
		return false;
	}
	XMLElement *pchildinfo_gateway = pchildinfo->FirstChildElement("httpURL");
	while (pchildinfo_gateway){
		std::string url = pchildinfo_gateway->Attribute("value");
		unsigned int serverid = atoi(pchildinfo_gateway->Attribute("serverid"));
		if (url != "" && selectedId == serverid){
			myServer.serverId = serverid;
			myServer.url = url;
		}
		pchildinfo_gateway = pchildinfo_gateway->NextSiblingElement();
	}
	return true;
}

void Mod_Game::sendMsg(rapidjson::Value &postData){
	if (postData.Empty()) return;
	/*HttpRequest* request = new HttpRequest();
	request->setUrl((myServer.url).c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(this, httpresponse_selector(Mod_Game::onHttpRequestCompleted));
	//const char* postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test1");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();*/
	GameLogic::GetInstance()->SendRequest(postData);
}

void Mod_Game::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response){
	/*if (!response){
		return;
	}
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	//_labelStatusCode->setString(statusString);
	log("response code: %d", statusCode);
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	// dump data 
	std::vector<char> *buffer = response->getResponseData();
	printf("Http Test, dump data: ");
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		printf("%c", (*buffer));
	}
	printf("\n");*/
}
