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
    string id;//�û�ID
    string username;//�û���
    string nickname;//�û���
    int coin;//���  
    int gamecoin;//��Ϸ���
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
	bool parseLoginCode();//������¼code

	int getRandPro(int betArr[8],int totalgold);  //����������

	int getluckPro(int totalgold);

	int getluckDx(int types,int money);

	string myServer;
    userinfo user;
    string md5Key;
    string desKey;
	string loginCode;
	int gameid;



	vector<int> list;

	//�㷨�õ��Ĳ���
	int prerand;

	map<int,Gezis *> v_gezi;
	//Gezis gezis[24];
};


