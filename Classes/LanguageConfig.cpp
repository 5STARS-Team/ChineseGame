#include "LanguageConfig.h"
#include "cocos2d.h"
#include "XMLManager.h"

using namespace cocos2d;
using namespace tinyxml2;
LanguageConfig::LanguageConfig()
{

}

bool LanguageConfig::init(std::string lang){


	XMLManager xmlMgr;
	tinyxml2::XMLDocument *doc = xmlMgr.Init("lang_zh-cn.xml","config");
	XMLElement *pinfo = doc->RootElement();   

	if (pinfo == NULL)
	{
		CCLOG("not find first child element, element name: 'config'");
		return false;
	}
	langList.clear();
	XMLElement *pchildinfo =  pinfo->FirstChildElement("lang");
	while (pchildinfo)
	{ 
        std::string name= pchildinfo->Attribute("name");
        std::string _value= pchildinfo->Attribute("value");
        if(name != ""){
            langList.insert(std::make_pair(name,_value));
        }
		pchildinfo = pchildinfo->NextSiblingElement();
	}
	
	return true;
}

std::string LanguageConfig::FindLang(std::string name)
{
	std::map<std::string, std::string>::iterator it;
	it = langList.find(name);
	if (it != langList.end())
	{
        return it->second;
	}else{   
        return name;
    }
}
