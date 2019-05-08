#pragma once

#include "cocos2d.h"
using namespace cocos2d;
#include <string>



namespace JNI
{ 
	void cppCharge(std::string payjson);
	std::string cppGetUUID();
	int cppGetCodeID();
	void cppLogin();
	void cppShare(std::string title,std::string content,std::string imgFileName);
    void openNewApp(int index);
    void cppLoginOut(std::string str="");
    void cppDownUrl(int id, std::string downurl,std::string sign,std::string version);
    int  cppGetVersionFlag(int id,std::string version);
    int  cppCheckEditText();
    void sendApkDownPercent(int id, int percent);
	void cppInfo(std::string msg);
}


