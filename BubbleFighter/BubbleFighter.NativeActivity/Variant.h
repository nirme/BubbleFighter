#pragma once

#include <string>


class VariantException : public std::exception
{
public:
	virtual const char* what() const noexcept
	{
		return "Incorrect VariantType used!";
	};

};


class Variant
{
public:
	typedef enum
	{
		INT,
		FLOAT,
		BOOL,
		STRING,
	} VariantType;

private: 
	const VariantType type;


public:
	Variant(VariantType _type)
		: type(_type)
	{};

	virtual ~Variant() = 0;

	virtual void operator= (int _value) { throw VariantException(); };
	virtual void operator= (float _value) { throw VariantException(); };
	virtual void operator= (bool _value) { throw VariantException(); };
	virtual void operator= (const std::string& _value) { throw VariantException(); };

	virtual operator int() const { throw VariantException(); };
	virtual operator float() const { throw VariantException(); };
	virtual operator bool() const { throw VariantException(); };
	virtual operator std::string() const { throw VariantException(); };


	virtual int GetValueInt() const { throw VariantException(); };
	virtual float GetValueFloat() const { throw VariantException(); };
	virtual bool GetValueBool() const { throw VariantException(); };
	virtual const std::string& GetValueString() const { throw VariantException(); };


};


class VariantInt
{
private:
	int value;

public:
	virtual void operator = (int _value) { value = _value; };
	virtual operator int() { return value; };
	virtual int GetValueInt() { return value; };
};