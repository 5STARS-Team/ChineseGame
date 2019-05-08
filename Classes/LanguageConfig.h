#ifndef STARLIGHT_LIBGAME_INTERNAL_LANGUAGECONFIG_H
#define STARLIGHT_LIBGAME_INTERNAL_LANGUAGECONFIG_H
#include<map>
#include<string>
using namespace std;

class LanguageConfig
{
public:
    
	LanguageConfig();
public:
	bool init(std::string lang);
	bool initTalk();
	std::string FindLang(std::string name);
public:
	std::map<std::string, std::string> langList;
	std::map<unsigned int,LanguageConfig> talkList;
	static LanguageConfig * GetInstance()
	{
		static LanguageConfig s_LanguageConfig;
		return &s_LanguageConfig;
	}
};

#endif