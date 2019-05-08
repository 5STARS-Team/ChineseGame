#ifndef STARLIGHT_LIBUTIL_H
#define STARLIGHT_LIBUTIL_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "CCStdC.h"
#include "cocos2d.h"
USING_NS_CC;


using namespace std;

	namespace util
	{
		int toInt(const string &value);
		unsigned int toUint(const string &value);
		long toInt64(const string &value);
		unsigned long  toUint64(const std::string &value);
		double toDouble(const string &value);
		std::string toString(int value);
		std::string toString(unsigned int value);
		std::string toString(long long value);
		std::string toString(unsigned long long value);
		std::string toString(float value);
		std::string toString(double value);
		std::string toString(bool value);
		std::string toString(long value);
        void StringSplit(std::string s,char splitchar,std::vector<std::string>& vec);  
		std::string toString( int value,int length,char );
        std::string FormatTime(int time);  //时间格式转换 00:00:00

	    const char* toChinese(const char* gb2312); //转换汉字

		string   timetodate(time_t const timer);
		string subString(string &str);
		string subString(string &str,unsigned int benpos,unsigned int enpos);
	
		
		std::string FormatTimeMinute(int countdown);//时间格式转换 00:00
		std::string FormatTimeSpace(unsigned int countdown);//时间差 日时分
		long getCurrentTime();
		string strftimesnum(long timenum );
		vector<std::string> parseUTF( std::string sin );//拆分字符串为一个一个字
	}

#endif // STARLIGHT_LIBUTIL_H ///:~
