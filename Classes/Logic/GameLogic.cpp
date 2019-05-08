#include "GameLogic.h"
#include "../ClientDefine.h"
#include "tool/md5.h"
#include "tool/libuitl.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "XMLManager.h"
#include "GameScene.h"
#include "TimeLoading.h"

using namespace tinyxml2;

GameLogic::GameLogic()
{
	loginCode = "D7SBPTaHMpbD/Qm1YqYDvPDvhWGcjZBuYwfG7vG/WPI2TYGUwiyRysY7wqc17SJ6FePKy+QGqPQDEjK+z6zNwu0ZzBsfXoCyEm/4C1taIOmeJfdjTgOSpvu1OHGQ9VHzn3UjUOMjJCSxG1eta7koP+T/OLzyAjqFpNKh1eA9EFo=";
	this->md5Key = "&5734ns)+1#@2";
	this->desKey = "18(*^jh*";
	this->gameid = 2;
    this->user.id = "0";
    this->user.gamecoin = 0;

	prerand = 0;
}
GameLogic::~GameLogic()
{

}


//��ȡ�Ĳ���С



//��������ɷ�
//1-���Ե��� 2-С��Ԫ 3-С��ϲ 4-С���� 5-����Ԫ  6-������  7-����
int GameLogic::getluckPro(int totalgold)
{
	srand((int)time(0));
	int r = rand()%7;
	if(totalgold > 3000)
		return 1;
	return r;
}

//type 4 = С  3= ��
int GameLogic::getluckDx(int types,int money)
{
	srand((int)time(0));
	int r = rand()%14;
	if(types == 4)
	{
		if(r<=7 && money >1000 )
		{
			return r + 7;
		}
	}
	return r;
}



//�ϻ����㷨
int GameLogic::getRandPro(int betArr[8],int totalgold)
{
	int onrands[6] = {11,8,16,23,9,20};

	/* �����ת�㷨 */
	srand((int)time(0));
	int rd = rand()%24;
	//����2000��
	if(totalgold > 2000)
	{
		int r = rand()%5;
		return onrands[r];
	}
	if(rd == prerand)
	{
		//����ϴ�����������ָ� ����һ��
		int dd = rand()%10;
		if(dd>2)
		{
			//�����һ��
			rd = rand()%24;
		}
	}
	prerand = rd;
	if(rd <=0)rd= 1;

	return rd;
}



/*
�ϻ������ӱ��
1-������ - 15��
2-HAH    - 50��
3-BAR   - 120��
4-ƻ��   - 5��
5-Сƻ�� -3��
6-â��  -10��
7-������ -20��
8-С���� -3��
9-GOODLUCK -����
10-ƻ�� - 5��
11-���� - 3��
12-������ - 10��
13-���� - 15��
14-˫7 - 3����
15-��˫7 - 40��
16-��ƻ�� - 5��
17-Сâ�� - 3��
18-��â�� - 10����
19-˫�� - 30��
20-С˫�� -3��
21-GOODLUCK -
22-ƻ��- 5��
23- ���� - 3��
24-���� - 10��
*/
bool GameLogic::init(){


	//��ʼ����Ϸ
	if(!CCUserDefault::sharedUserDefault()->isXMLFileExist())
	{
		//�º�
		this->user.coin = 300;
	}
	else
	{
		int gold = CCUserDefault::sharedUserDefault()->getIntegerForKey("integer");
		this->user.coin = gold;
	}

	//��ʼ����Ϸ����;;ÿ��ˮ���ĸ���


	//��ʼ����������
	/* ������ */
	Gezis *zs1 =new Gezis;
	zs1->fenshu = 15;
	zs1->zhuid = 4;
	v_gezi.insert(std::make_pair(1,zs1));

	/* HAHA */
	Gezis *zs2=new Gezis;
	zs2->fenshu = 50;
	zs2->zhuid = 8;
	v_gezi.insert(std::make_pair(2,zs2));


	/* HAHA */
	Gezis *zs3=new Gezis;
	zs3->fenshu = 120;
	zs3->zhuid = 8;
	v_gezi.insert(std::make_pair(3,zs3));

	/* ƻ�� */
	Gezis *zs4=new Gezis;
	zs4->fenshu = 5;
	zs4->zhuid = 1;
	v_gezi.insert(std::make_pair(4,zs4));
	/* Сƻ�� */
	Gezis *zs5=new Gezis;
	zs5->fenshu = 3;
	zs5->zhuid = 1;
	v_gezi.insert(std::make_pair(5,zs5));
	/* â�� */
	Gezis *zs6=new Gezis;
	zs6->fenshu = 10;
	zs6->zhuid = 3;
	v_gezi.insert(std::make_pair(6,zs6));
	/* ���� */
	Gezis *zs7=new Gezis;
	zs7->fenshu = 20;
	zs7->zhuid = 5;
	v_gezi.insert(std::make_pair(7,zs7));

	/* С���� */
	Gezis *zs8=new Gezis;
	zs8->fenshu = 3;
	zs8->zhuid = 5;
	v_gezi.insert(std::make_pair(8,zs8));

	/* LUCK */
	Gezis *zs9=new Gezis;
	zs9->fenshu = 0;
	zs9->zhuid = 0;
	v_gezi.insert(std::make_pair(9,zs9));

	/* ��ƻ�� */
	Gezis *zs10=new Gezis;
	zs10->fenshu = 5;
	zs10->zhuid = 1;
	v_gezi.insert(std::make_pair(10,zs10));

	/* ���� */
	Gezis *zs11=new Gezis;
	zs11->fenshu = 3;
	zs11->zhuid = 2;
	v_gezi.insert(std::make_pair(11,zs11));

	/* ���� */
	Gezis *zs12=new Gezis;
	zs12->fenshu = 10;
	zs12->zhuid = 2;
	v_gezi.insert(std::make_pair(12,zs12));

	/* ����15 */
	Gezis *zs13=new Gezis;
	zs13->fenshu = 15;
	zs13->zhuid = 4;
	v_gezi.insert(std::make_pair(13,zs13));

	/* 77 */
	Gezis *zs14=new Gezis;
	zs14->fenshu = 3;
	zs14->zhuid = 7;
	v_gezi.insert(std::make_pair(14,zs14));

	Gezis *zs15=new Gezis;
	zs15->fenshu = 40;
	zs15->zhuid = 7;
	v_gezi.insert(std::make_pair(15,zs15));

	/* ƻ�� */
	Gezis *zs16=new Gezis;
	zs16->fenshu = 5;
	zs16->zhuid = 1;
	v_gezi.insert(std::make_pair(16,zs16));

	/* â�� */
	Gezis *zs17=new Gezis;
	zs17->fenshu = 3;
	zs17->zhuid = 3;
	v_gezi.insert(std::make_pair(17,zs17));


	/* ��â�� */
	Gezis *zs18=new Gezis;
	zs18->fenshu = 10;
	zs18->zhuid = 3;
	v_gezi.insert(std::make_pair(18,zs18));



	/* ˫�� */
	Gezis *zs19=new Gezis;
	zs19->fenshu = 30;
	zs19->zhuid = 6;
	v_gezi.insert(std::make_pair(19,zs19));
	/* С˫�� */
	Gezis *zs20=new Gezis;
	zs20->fenshu = 3;
	zs20->zhuid = 6;
	v_gezi.insert(std::make_pair(20,zs20));


	Gezis *zs21=new Gezis;
	zs21->fenshu = 0;
	zs21->zhuid = 0;
	v_gezi.insert(std::make_pair(21,zs21));

	Gezis *zs22=new Gezis;
	zs22->fenshu = 5;
	zs22->zhuid = 1;
	v_gezi.insert(std::make_pair(22,zs22));

	Gezis *zs23=new Gezis;
	zs23->fenshu = 3;
	zs23->zhuid = 4;
	v_gezi.insert(std::make_pair(23,zs23));

	Gezis *zs24=new Gezis;
	zs24->fenshu = 10;
	zs24->zhuid = 2;
	v_gezi.insert(std::make_pair(24,zs24));

	/*XMLManager xmlMgr;
	tinyxml2::XMLDocument *doc = xmlMgr.Init("Config.xml", "config");
	XMLElement *pinfo = doc->RootElement();
	if (pinfo == NULL){
		CCLOG("not find first child element, element name: 'config'");
		return false;
	}
	XMLElement* serverIdXML = pinfo->FirstChildElement("selected_serverid");
	int selectedId = atoi(serverIdXML->Attribute("serverid"));
	//��������ַ
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
			myServer = url;
			break;
		}
		pchildinfo_gateway = pchildinfo_gateway->NextSiblingElement();
	}*/
	return true;
}

//��������
void GameLogic::SendRequest(  const rapidjson::Value &data)
{
	/*
    if(!data.HasMember("c") || !data["c"].IsInt()){
        log("no code!");
        return ;
    }
	
	
    int code = data["c"].GetInt();
	if(code != CODE_GAME_GET_COIN_RE)
		TimeLoadingLayer::StartWaiting();
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);  
    data.Accept(writer);
    string params = buffer.GetString();
    network::HttpClient::getInstance()->enableCookies(NULL);
    network::HttpRequest* request = new network::HttpRequest();
    request->setUrl(myServer.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameLogic::requestUrlResult, this));
    

    rapidjson::Document document;

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();  
    rapidjson::Value root(rapidjson::kObjectType); 
    //root.AddMember("id",user.id,allocator);
    //root.AddMember("username",user.username,allocator);
    root.AddMember("c",code,allocator);
    root.AddMember("d",params.c_str(),allocator);
    root.AddMember("id",this->user.id.c_str(),allocator);
    md5 sign(util::toString(code)+params+this->user.id+this->md5Key);
    string md5_sign = sign.toString();
    root.AddMember("s",md5_sign.c_str(),allocator);
    rapidjson::StringBuffer buffer2;  
    rapidjson::Writer<rapidjson::StringBuffer> writer2(buffer2);  
    root.Accept(writer2);  

    string des_str = util::DES_3::des_base64_encrypt(buffer2.GetString(),this->desKey.c_str()); 

    const char* postData =des_str.c_str();  // including \0, the strings after \0 should not be cut in response
    request->setRequestData(postData, strlen(postData)); 
    request->setTag((util::toString(code)).c_str());
    
    network::HttpClient::getInstance()->send(request);
    request->release();
	*/
}

void GameLogic::requestUrlResult( network::HttpClient *sender, network::HttpResponse *response )
{
	/*
	
    if (!response)
    {        
		TimeLoadingLayer::EndWaiting();
		GameScene::gamePtr->requestData();
        return;
    }
    // You can get original request type from: response->request->reqType
    string tagStr = response->getHttpRequest()->getTag();
    if (0 != strlen(tagStr.c_str())) 
    {
        CCLOG("%s completed", tagStr.c_str());
    }
    int tagCode = util::toInt(tagStr);
	if (tagCode != CODE_GAME_GET_COIN_RE){
		TimeLoadingLayer::EndWaiting();
	}
    int statusCode = response->getResponseCode();

    if (!response->isSucceed()) 
    {
        if(statusCode == 500){
            //SystemLayer::GetInstance()->MsgBox(" server error ");
        }
        CCLOG("error buffer: %s", response->getErrorBuffer());    
        this->CheckRequest(tagCode);
        return;
    }

    // dump data
    std::vector<char> *buffer = response->getResponseData();
    string data(buffer->begin(),buffer->end());
    string des_str = util::DES_3::base64_des_decrypt(data.c_str(),this->desKey.c_str());

    rapidjson::Document jsonDict;
    jsonDict.Parse<0>(des_str.c_str());
    if (jsonDict.HasParseError())
    {
        CCLOG("GetParseError %s\n",jsonDict.GetParseError());
        this->CheckRequest(tagCode);
        return;
    }

    if(jsonDict.IsObject()&&jsonDict.HasMember("s")&&jsonDict.HasMember("d")){
        string sign_str = util::toString(tagCode) + jsonDict["d"].GetString() + this->md5Key;
        string sign_server = jsonDict["s"].GetString();
        md5 sign(sign_str);
        string sign_md5 = sign.toString();
        if( sign_md5 != sign_server){
            CCLOG("SignError %s\n",des_str.c_str());
            this->CheckRequest(tagCode);
            return;
        }
    }else{
        this->CheckRequest(tagCode);
        return;
    }

    string data_str = jsonDict["d"].GetString();


    //rapidjson::Document jsonDict;
    jsonDict.Parse<0>(data_str.c_str());
    if (jsonDict.HasParseError())
    {
        CCLOG("GetParseError %s\n",jsonDict.GetParseError());
        return;
    }
    if(jsonDict.IsObject()&&jsonDict.HasMember("r")&&jsonDict.HasMember("i")){

    }else{
        this->CheckRequest(tagCode);
        return;
    }
    const rapidjson::Value &retV=jsonDict["r"];
    const rapidjson::Value &infoV=jsonDict["i"];
    string ret = retV.GetString();
    string info = infoV.GetString();
    const rapidjson::Value &codeValue=jsonDict["c"];
    int code = util::toInt(codeValue.GetString());
    const rapidjson::Value &dataValue=jsonDict["d"];

    this->CheckRequest(code,ret, info,dataValue);
	*/
}

void GameLogic::CheckRequest( int code ,string ret,string info /*= ""*/,const rapidjson::Value &data /*= NULL*/ )
{
	if (code == MSG_BET_SAMPLE_RSP || code == MSG_FRUIYS_DA_XIA_RSP || code == CODE_GAME_GET_COIN_RS){
		GameScene::gamePtr->getResponse(code, ret, info /*= ""*/,data /*= NULL*/);
	}
}


string GameLogic::getCode( int gameid )
{
	/*
    rapidjson::Document document;
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();  
    rapidjson::Value root(rapidjson::kObjectType); 
    rapidjson::Value user(rapidjson::kObjectType); 
    user.AddMember("id",this->user.id.c_str(),allocator);
    user.AddMember("username",this->user.username.c_str(),allocator);
    user.AddMember("nickname",this->user.nickname.c_str(),allocator);
    user.AddMember("coin",this->user.coin,allocator);
    user.AddMember("gamecoin",this->user.gamecoin,allocator);
    root.AddMember("user",user,allocator);


    md5 sign(util::toString(this->user.coin)+util::toString(this->user.gamecoin)+util::toString(gameid)+this->user.username+this->user.nickname+this->user.id+this->md5Key);
    string md5_sign = sign.toString();
    root.AddMember("sign",md5_sign.c_str(),allocator);
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);  
    root.Accept(writer);  
    string des_str = util::DES_3::des_base64_encrypt(buffer.GetString(),this->desKey.c_str());
	*/
    return "";

}



bool GameLogic::parseLoginCode()
{
	/*
	string des_str = util::DES_3::base64_des_decrypt(this->loginCode.c_str(), this->desKey.c_str());

	rapidjson::Document jsonDict;
	jsonDict.Parse<0>(des_str.c_str());
	if (jsonDict.HasParseError()){
		return false;
	}

	if (jsonDict.IsObject() && jsonDict.HasMember("user") && jsonDict.HasMember("sign")){
		user.id = jsonDict["user"]["id"].GetString();
		user.username = jsonDict["user"]["username"].GetString();
		user.nickname = jsonDict["user"]["nickname"].GetString();
		user.coin = jsonDict["user"]["coin"].GetInt();
		user.gamecoin = jsonDict["user"]["gamecoin"].GetInt();
	} else{
		return false;
	}
	string sign = jsonDict["sign"].GetString();
	md5 sign2(util::toString(this->user.coin) + util::toString(this->user.gamecoin) + util::toString(gameid) + this->user.username + this->user.nickname + this->user.id + this->md5Key);
	string md5_sign = sign2.toString();
	if (sign == md5_sign){
		return true;
	} else{
		return false;
	}
	*/

	return true;
}









