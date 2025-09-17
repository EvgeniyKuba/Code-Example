#ifndef C_ENGINECONFIG_H
#define C_ENGINECONFIG_H

#include "../tinyxml2/tinyxml2.h"

namespace KGF
{
	//////////////////////////////////////////////////////////////////////////
	class CEngineConfig
	{
	public:
		CEngineConfig();
		~CEngineConfig();

	public:
		std::string getAttribute(const std::string& tagName, const std::string& attrName);
		void setAttribute(const std::string& s_tagName, const std::string& s_attrName, const std::string& value);

	private:
		void openConfig();

		tinyxml2::XMLDocument configDoc;
		tinyxml2::XMLElement* root;
	};
}

#endif