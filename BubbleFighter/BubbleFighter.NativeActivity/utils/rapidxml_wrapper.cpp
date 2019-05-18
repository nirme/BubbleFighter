#include "rapidxml_wrapper.h"


#ifdef RAPIDXML_NO_EXCEPTIONS

namespace rapidxml
{
	namespace
	{
		std::list<const char*> l_rapidxmlErrors;
	}


	const char* rxGetError()
	{
		if (!l_rapidxmlErrors.size()) return NULL;
		const char* err = l_rapidxmlErrors.front();
		l_rapidxmlErrors.pop_front();
		return err;
	};



	/*
	Description
	When exceptions are disabled by defining RAPIDXML_NO_EXCEPTIONS,this function is called to notify user about the error. It must be defined by the user.

	This function cannot return. If it does,the results are undefined.

	A very simple definition might look like that: void rapidxml::parse_error_handler(const char *what,void *where) { std::cout << "Parse error: " << what << "\n"; std::abort(); }
	*/

	void parse_error_handler(const char *what,void *where)
	{
		l_rapidxmlErrors.push_back(what);
	};
}

#endif



int getNodeValueInt(XmlNode* _xn_node)
{
	int base = 10;
	std::string str_data;
	if(XmlAttrib* xattr = _xn_node->first_attribute("base",0,false))
	{
		str_data.assign(xattr->value(),xattr->value_size());
		base = strtol(str_data.c_str(),NULL,10);
	}

	str_data.assign(_xn_node->value(),_xn_node->value_size());
	return strtol(str_data.c_str(),NULL,base);
};


long long getNodeValueLLong(XmlNode* _xn_node)
{
	int base = 10;
	std::string str_data;
	if(XmlAttrib* xattr = _xn_node->first_attribute("base",0,false))
	{
		str_data.assign(xattr->value(),xattr->value_size());
		base = strtol(str_data.c_str(),NULL,10);
	}

	str_data.assign(_xn_node->value(),_xn_node->value_size());
	return strtoll(str_data.c_str(),NULL,base);
};



float getNodeValueFloat(XmlNode* _xn_node)
{
	std::string str_data(_xn_node->value(),_xn_node->value_size());
	return strtof(str_data.c_str(),NULL);
};


double getNodeValueDouble(XmlNode* _xn_node)
{
	std::string str_data(_xn_node->value(),_xn_node->value_size());
	return strtod(str_data.c_str(),NULL);
};


std::string getNodeValueString(XmlNode* _xn_node)
{
	return std::string(_xn_node->value(),_xn_node->value_size());
};



XmlNode* createXmlNode(const char* _c_name)
{
	XmlNode* xn_elementNode = new XmlNode(rapidxml::node_type::node_element);
	xn_elementNode->name(_c_name);
	return xn_elementNode;
};


XmlNode* createXmlStorageNode(const char* _c_name,const char* _c_value)
{
	XmlNode* xn_elementNode = new XmlNode(rapidxml::node_type::node_element);
	XmlNode* xn_dataNode = new XmlNode(rapidxml::node_type::node_data);

	xn_elementNode->name(_c_name);
	xn_dataNode->value(_c_value);

	xn_elementNode->append_node(xn_dataNode);
	return xn_elementNode;
};


XmlNode* createXmlStorageNode(const char *_c_name,double _d_value)
{
	char c_text[50] ={0};
	snprintf(c_text,50,"%f",_d_value);
	return createXmlStorageNode(_c_name,c_text);
};


XmlNode* createXmlStorageNode(const char *_c_name,float _f_value)
{
	return createXmlStorageNode(_c_name,(double)_f_value);
};


XmlNode* createXmlStorageNode(const char *_c_name,long long _ll_value)
{
	char c_text[50] ={0};
	snprintf(c_text,50,"%d",_ll_value);

	XmlAttrib* xa_attrib = new XmlAttrib();
	xa_attrib->name("base");
	xa_attrib->value("10");

	XmlNode* xn_ret = createXmlStorageNode(_c_name,c_text);
	xn_ret->append_attribute(xa_attrib);

	return xn_ret;
};


XmlNode* createXmlStorageNode(const char *_c_name,int _i_value)
{
	return createXmlStorageNode(_c_name,(long long)_i_value);
};
