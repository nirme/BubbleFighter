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
		VAR_INT,
		VAR_FLOAT,
		VAR_BOOL,
		VAR_STRING,
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


class VariantInt : public Variant
{
private:
	int value;

public:
	VariantInt() : Variant(VAR_INT), value(0) {};
	virtual void operator = (int _value) { value = _value; };
	virtual operator int() { return value; };
	virtual int GetValueInt() { return value; };
};


class VariantFloat : public Variant
{
private:
	float value;

public:
	VariantFloat() : Variant(VAR_FLOAT), value(0.f) {};
	virtual void operator = (float _value) { value = _value; };
	virtual operator float() { return value; };
	virtual float GetValueFloat() { return value; };
};


class VariantBool : public Variant
{
private:
	bool value;

public:
	VariantBool() : Variant(VAR_BOOL), value(false) {};
	virtual void operator = (bool _value) { value = _value; };
	virtual operator bool() { return value; };
	virtual bool GetValueBool() { return value; };
};


class VariantString : public Variant
{
private:
	std::string value;

public:
	VariantString() : Variant(VAR_STRING) {};
	virtual void operator = (std::string _value) { value = _value; };
	virtual operator std::string() { return value; };
	virtual const std::string& GetValueString() { return value; };

};