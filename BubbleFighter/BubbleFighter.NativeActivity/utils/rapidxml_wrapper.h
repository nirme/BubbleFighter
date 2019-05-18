#pragma once

#include "rapidxml\rapidxml.hpp"
//helpers
#include "rapidxml\rapidxml_iterators.hpp"
#include "rapidxml\rapidxml_print.hpp"
#include "rapidxml\rapidxml_utils.hpp"


//#define RAPIDXML_NO_EXCEPTIONS


#ifdef RAPIDXML_NO_EXCEPTIONS

	#include <list>

	namespace rapidxml
	{
		const char* rxGetError();
	}

#endif //_RAPIDXML_WRAPPER


#include <string>


typedef rapidxml::xml_document<char>	XmlDoc;
typedef rapidxml::xml_base<char>		XmlBase;
typedef rapidxml::xml_node<char>		XmlNode;
typedef rapidxml::xml_attribute<char>	XmlAttrib;



int	        getNodeValueInt    (XmlNode* _xn_node);
long long   getNodeValueLLong  (XmlNode* _xn_node);
float       getNodeValueFloat  (XmlNode* _xn_node);
double      getNodeValueDouble (XmlNode* _xn_node);
std::string getNodeValueString (XmlNode* _xn_node);



XmlNode* createXmlNode       (const char* _c_name);
XmlNode* createXmlStorageNode(const char* _c_name, const char* _c_value);
XmlNode* createXmlStorageNode(const char *_c_name, double _d_value);
XmlNode* createXmlStorageNode(const char *_c_name, float _f_value);
XmlNode* createXmlStorageNode(const char *_c_name, long long _ll_value);
XmlNode* createXmlStorageNode(const char *_c_name, int _i_value);

