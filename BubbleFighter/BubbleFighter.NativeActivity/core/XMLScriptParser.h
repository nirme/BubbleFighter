#pragma once


#include <memory>
#include <vector>

#include "ScriptParser.h"
#include "XmlScriptNode.h"
#include "Logger.h"



namespace core
{

	class XmlScriptParser : public ScriptParser
	{
	private:

		typedef rapidxml::xml_document<char>	XmlDoc;
		typedef rapidxml::xml_node<char>		XmlNode;


#ifdef NDEBUG
		static constexpr int parserSetting = rapidxml::parse_no_entity_translation | rapidxml::parse_trim_whitespace;
#else // !NDEBUG
		static constexpr int parserSetting = rapidxml::parse_no_entity_translation | rapidxml::parse_validate_closing_tags | rapidxml::parse_trim_whitespace;
#endif // NDEBUG

		typedef std::shared_ptr<XmlScriptNode> XmlScriptNodePtr;


		ScriptNodePtr populateNode(XmlDocContainerPtr _owner, XmlNode *_root) const
		{
			if (!_root) return nullptr;

			ScriptNodeListPtr childNodes;
			for (XmlNode *child = _root->first_node(); child != nullptr; child = child->next_sibling())
			{
				childNodes->push_back(populateNode(_owner, child));
			}

			return std::make_shared<XmlScriptNode>(_owner, _root, childNodes);
		};


	public:
		ScriptNodeListPtr parseScript(DataStreamPtr _stream) const
		{
			XmlDocContainerPtr xmlDocContainer;

			xmlDocContainer->xmlText.resize(_stream->getSize());
			xmlDocContainer->xmlText.shrink_to_fit();
			_stream->readData(xmlDocContainer->xmlText.data());

			xmlDocContainer->xmlDoc.parse<parserSetting>(xmlDocContainer->xmlText.data());

			// xml should have only 1 root node
			XmlNode *root = xmlDocContainer->xmlDoc.first_node();

			ScriptNodePtr rootNode = populateNode(xmlDocContainer, root);

			// return list of root children to ignore the root
			return rootNode->getChildList();
		};
	};

}
