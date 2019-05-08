#include "libuitl.h"
#include <stack>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "cocostudio/TriggerBase.h"
using namespace cocostudio;

using namespace std;

const int ONEDAY = 86400;
const int ONEHOUR= 3600;
const int ONEMINUTE = 60;

int util::toInt(const string &value)
{
	return static_cast<int>(strtol(value.data(), NULL, 0));
}

unsigned int util::toUint(const string &value)
{
	return static_cast<unsigned int>(strtoul(value.data(), NULL, 0));
}

 long util::toInt64(const string &value)
{
	return strtol(value.data(), NULL, 0);
}

unsigned long  util::toUint64(const string &value)
{
	return strtoul(value.data(), NULL, 0);
}



double util::toDouble(const string &value)
{
	return strtod(value.data(), 0);
}



template<typename T>
static string toStringInternal(T value)
{
	stringstream stream;
	stream << value;
	return stream.str();
}

string util::toString(int value)
{
	return toStringInternal(value);
}

string util::toString(unsigned int value)
{
	return toStringInternal(value);
}
string util::toString(long value)
{
	return toStringInternal(value);
}
string util::toString(long long value)
{
	return toStringInternal(value);
}

string util::toString(unsigned long long value)
{
	return toStringInternal(value);
}

string util::toString(float value)
{
	return toStringInternal(value);
}

string util::toString(double value)
{
	return toStringInternal(value);
}

string util::toString(bool value)
{
	return value ? "true" : "false";
}
std::string util::toString(int value,int length,char fill)
{
	stringstream stream;
	stream << setw(length)<<setiosflags(ios::right)<<setfill(fill)<<value;
	return stream.str();
}
void util::StringSplit(string s,char splitchar,vector<string>& vec)  
{  
    if(vec.size()>0)//保证vec是空的  
        vec.clear();  
    int length = s.length();  
    int start=0;  
    for(int i=0;i<length;i++)  
    {  
        if(s[i] == splitchar && i == 0)//第一个就遇到分割符  
        {  
            start += 1;  
        }  
        else if(s[i] == splitchar)  
        {  
            vec.push_back(s.substr(start,i - start));  
            start = i+1;  
        }  
        else if(i == length-1)//到达尾部  
        {  
			//if(!vec.empty())
				vec.push_back(s.substr(start,i+1 - start));  
        }  
    }  
} 
//时间格式转换
std::string util::FormatTime(int countdown)
{
	int time = countdown;
	int day = time/ONEDAY;
	int hour = time%ONEDAY/ONEHOUR;
	int minute = time%ONEHOUR/ONEMINUTE;
	int second = time%ONEMINUTE;

	//util::toString(day,1,'0')+":"+
    std::string time_count = util::toString(hour,2,'0')+":"+util::toString(minute,2,'0')+":"+util::toString(second,2,'0');
	return time_count;
}
//时间格式(分钟)
std::string util::FormatTimeMinute(int countdown)
{
	int time = countdown;
	int minute = time%ONEHOUR/ONEMINUTE;
	int second = time%ONEMINUTE;

	//util::toString(day,1,'0')+":"+
	std::string time_count = util::toString(minute,2,'0')+":"+util::toString(second,2,'0');
	return time_count;
}
//时间差
std::string util::FormatTimeSpace(unsigned int countdown)
{
	int time = countdown;
	int day = time/ONEDAY;
	int hour = time%ONEDAY/ONEHOUR;
	int minute = time%ONEHOUR/ONEMINUTE;
	int second = time%ONEMINUTE;
	std::string time_count = util::toString(day)+"|"+util::toString(hour)+"|"+util::toString(minute);
	return time_count;
}
//文字转换
const char* util::toChinese(const char* gb2312)
{
	//int len = MultiByteToWideChar(CP_ACP,0,gb2312,-1,NULL,0);
	//wchar_t* wstr = new wchar_t[len+1];
	//memset(wstr,0,len+1);
	//MultiByteToWideChar(CP_ACP,0,gb2312,-1,wstr,len);
	//len = WideCharToMultiByte(CP_UTF8,0,wstr,-1,NULL,0,NULL,NULL);
	//char* str = new char[len+1];
	//memset(str,0,len+1);
	//WideCharToMultiByte(CP_UTF8,0,wstr,-1,str,len,NULL,NULL);
	//if (wstr)
	//{
	//	delete[] wstr;
	//}
	return gb2312;
}

string   util::timetodate(time_t const timer)
{
	struct tm *l=localtime(&timer);  
	char buf[128];  

	snprintf(buf,sizeof(buf),"%04d-%02d-%02d %02d:%02d:%02d",l->tm_year+1900,l->tm_mon+1,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);  
	string s(buf);  
	return s;  

}
string util::subString(string &str,unsigned int benpos,unsigned int length)
{
	string s(str);
	if(s.size()>24)
	{
		s = s.substr(benpos,length);
	}
	return s;
}
string util::subString(string &str)
{
	string s(str);
	if(s.size()>24)
	{
		s = s.substr(0,18);
		s = s+"...";
	}
	return s;
}

//获取时间戳 毫秒级
long util::getCurrentTime()   
{    
    struct timeval tv;    
    gettimeofday(&tv,NULL);    
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;    
}

//时间戳转换
string util::strftimesnum(long timenum )
{
	time_t t;
    struct tm *p;
    t=timenum;
    p=gmtime(&t);
    char s[80];
    strftime(s, 80, "%Y-%m-%d %H:%M:%S::%Z", p);
    printf("%d: %s\n", (int)t, s);
	return s;
}


vector<std::string> util::parseUTF( std::string sin )
{
	int l = sin.length();
	vector<string> ret;
	ret.clear();
	for(int p = 0; p < l; ) 
	{
		int size, n = l - p;
		unsigned char c = sin[p], cc = sin[p + 1];
		if(c < 0x80) 
		{
			size = 1;
		}
		else if(c < 0xc2) 
		{
		}
		else if(c < 0xe0)
		{
			if(n < 2)
			{
			}
			if(!((sin[p + 1] ^ 0x80) < 0x40)) 
			{
			}
			size = 2;
		} 
		else if(c < 0xf0) 
		{
			if(n < 3) 
			{
			}
			if(!((sin[p + 1] ^ 0x80) < 0x40 && 
				(sin[p + 2] ^ 0x80) < 0x40 &&
				(c >= 0xe1 || cc >= 0xa0))) 
			{
			}
			size = 3;
		} 
		else if(c < 0xf8) 
		{
			if(n < 4) 
			{
			}
			if(!((sin[p + 1] ^ 0x80) < 0x40 &&
				(sin[p + 2] ^ 0x80) < 0x40 &&
				(sin[p + 3] ^ 0x80) < 0x40 &&
				(c >= 0xf1 || cc >= 0x90))) 
			{
			}
			size = 4;
		} 
		else if (c < 0xfc) 
		{
			if(n < 5) 
			{
			}
			if(!((sin[p + 1] ^ 0x80) < 0x40 &&
				(sin[p + 2] ^ 0x80) < 0x40 &&
				(sin[p + 3] ^ 0x80) < 0x40 && 
				(sin[p + 4] ^ 0x80) < 0x40 &&
				(c >= 0xfd || cc >= 0x88))) 
			{
			}
			size = 5;
		} 
		else if (c < 0xfe) 
		{
			if(n < 6) 
			{
			}
			if(!((sin[p + 1] ^ 0x80) < 0x40 &&
				(sin[p + 2] ^ 0x80) < 0x40 &&
				(sin[p + 3] ^ 0x80) < 0x40 && 
				(sin[p + 4] ^ 0x80) < 0x40 &&
				(sin[p + 5] ^ 0x80) < 0x40 &&
				(c >= 0xfd || cc >= 0x84))) 
			{
			}
			size = 6;
		} 
		else
		{
		}
		string temp = "";
		temp = sin.substr(p, size);
		ret.push_back(temp);
		p += size;
	}
	return ret;
}
