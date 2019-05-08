#pragma once
#include "cocos2d.h"
#include <string>
#include <list>
#include <vector>
#include <map>
#include "tool/3des.h"
#include "network/HttpClient.h"
#include "json/document.h"



USING_NS_CC;

using namespace std;

struct userinfo
{
    string id;//用户ID
    string username;//用户名
    string nickname;//用户名
    int coin;//金币  
    int gamecoin;//游戏金币
    string md5sign;
};

class Gezis
{
public:
	int zhuid;
	int fenshu;
};


class GameLogic:public Ref
{

public:
	GameLogic();
	~GameLogic();
	static GameLogic* GetInstance()
	{
		static GameLogic m_loginLogic;
		return &m_loginLogic;
	}
	bool init();
    string getCode(int gameid);
    void SendRequest(const rapidjson::Value &data );
    void requestUrlResult(network::HttpClient *sender, network::HttpResponse *response);
    void CheckRequest( int code ,string ret="900",string info = "",const rapidjson::Value &data = NULL);
	bool parseLoginCode();//解析登录code

	int getRandPro(int betArr[8],int totalgold);  //获得随机概率

	int getluckPro(int totalgold);

	int getluckDx(int types,int money);

	string myServer;
    userinfo user;
    string md5Key;
    string desKey;
	string loginCode;
	int gameid;



	vector<int> list;

	//算法用到的参数
	int prerand;

	map<int,Gezis *> v_gezi;
	//Gezis gezis[24];
};


