#include "external/tinyxml2/tinyxml2.h"
#include <string>


using namespace std;

class XMLManager
{
public:
	~XMLManager();
	XMLManager();
    tinyxml2::XMLDocument * Init(const char *filename,const char *folder);

private:
	tinyxml2::XMLDocument *doc;
	string m_sFilePath;
};
