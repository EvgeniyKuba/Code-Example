#include "CEngineConfig.h"

//////////////////////////////////////////////////////////////////////////

namespace KGF
{
	//////////////////////////////////////////////////////////////////////////
	CEngineConfig::CEngineConfig()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	CEngineConfig::~CEngineConfig()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	std::string CEngineConfig::getAttribute(const std::string& s_tagName, const std::string& s_attrName)
	{
		openConfig();

		tinyxml2::XMLElement* tagName = root->FirstChildElement(s_tagName.c_str());
		if (tagName == nullptr)
		{
			tagName = root->InsertNewChildElement(s_tagName.c_str());
			#ifdef DEBUG
				spdlog::error("Can't find tag: {} into config.xml", s_tagName);
			#endif

			return "default";
		}
		else
		{
			const char* value = tagName->Attribute(s_attrName.c_str());
			if (value == nullptr)
			{
				tagName->SetAttribute("s_attrName", "default");
				#ifdef DEBUG
					spdlog::error("Attribute '{}' not found in tag '{}'", s_attrName, s_tagName);
				#endif

				return "default";
			}
			return std::string(value);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void CEngineConfig::setAttribute(const std::string& s_tagName, const std::string& s_attrName, const std::string& s_value)
	{
		openConfig();

		tinyxml2::XMLElement* tagName = root->FirstChildElement(s_tagName.c_str());
		if (tagName == nullptr)
		{
			tagName = root->InsertNewChildElement(s_tagName.c_str());
			#ifdef DEBUG
				spdlog::error("Can't find tag: {} into config.xml", s_tagName);
			#endif
		}
		else
		{
			tagName->SetAttribute(s_attrName.c_str(), s_value.c_str());

			tinyxml2::XMLError eResult = configDoc.SaveFile("data/properties/config.xml");
			if (eResult != tinyxml2::XML_SUCCESS)
			{
				#ifdef DEBUG
					spdlog::error("Failed to save XML file: {}", (int)eResult);
				#endif
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void CEngineConfig::openConfig()
	{
		if (configDoc.LoadFile("data/properties/config.xml") != tinyxml2::XML_SUCCESS)
		{
			#ifdef DEBUG
				spdlog::error("Can't load config.xml. Creating new config.");
			#endif

			configDoc.Clear();
			root = configDoc.NewElement("Config");
			configDoc.InsertFirstChild(root);
		}
		else
		{
			#ifdef DEBUG
				spdlog::info("config.xml loaded successfully.");
			#endif

			root = configDoc.FirstChildElement("Config");
			if (root == nullptr)
			{
				root = configDoc.NewElement("Config");
				configDoc.InsertFirstChild(root);

				#ifdef DEBUG
					spdlog::error("Root element <Config> not found. Created new root.");
				#endif
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
}