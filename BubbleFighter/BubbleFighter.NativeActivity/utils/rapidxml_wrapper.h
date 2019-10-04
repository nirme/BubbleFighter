#pragma once

#include <string>

#include "rapidxml\rapidxml.hpp"
//helpers
#include "rapidxml\rapidxml_iterators.hpp"
#include "rapidxml\rapidxml_print.hpp"
#include "rapidxml\rapidxml_utils.hpp"


#undef RAPIDXML_NO_EXCEPTIONS



typedef rapidxml::xml_document<char>	XmlDoc;
typedef rapidxml::xml_node<char>		XmlNode;
typedef rapidxml::xml_attribute<char>	XmlAttrib;


std::string getAttributeOrEmpty(XmlNode* _node, const std::string _attribute)
{
	XmlAttrib* attrib = _node->first_attribute(_attribute.c_str());
	return attrib ? std::string(attrib->value, attrib->value_size) : std::string();
};

