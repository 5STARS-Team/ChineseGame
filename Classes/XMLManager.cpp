#include "XMLManager.h"
#include "cocos2d.h"
using namespace cocos2d;


XMLManager::XMLManager()
{
	doc = NULL;
	
}


tinyxml2::XMLDocument *XMLManager::Init(const char *filename,const char *folder)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	m_sFilePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(filename,folder);
#else
	char newfile[200];
	memset(newfile,0,200);
	strcat(newfile,folder);
	const char *s = "/";
	strcat(newfile,s);
	strcat(newfile,filename);

	m_sFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(newfile);
#endif
	if(!doc)
	{
		ssize_t nSize; 
		const char *pXmlBuffer = (const char*)CCFileUtils::sharedFileUtils()->getFileData(m_sFilePath.c_str(), "rb", &nSize);  
		doc = new tinyxml2::XMLDocument();
		tinyxml2::XMLError error = doc->Parse(pXmlBuffer,nSize);

		/* 读取配置后要手动释放,这个地方不会被自动释放的 */
		delete [] pXmlBuffer;
		pXmlBuffer = NULL;
		if(error != tinyxml2::XML_NO_ERROR)
		{
			CCLOG("load xml faild xmlfile = %s errcode = %d",m_sFilePath.c_str(),error);
			return NULL;
		}
	}
	else
	{
		delete doc;
		doc = NULL;
		return NULL;
	}
	return doc;
}


XMLManager::~XMLManager()
{
	if(doc)
		delete doc;
}