#pragma once
/*
#include <typeinfo>



class Variant;
template<typename ValueType> 
ValueType variant_cast(const Variant & operand);



class Variant
{
protected:

	class ContainerInterface
	{
	public:

		virtual ~ContainerInterface() {};

		virtual const std::type_info& getType() const = 0;

		virtual ContainerInterface * clone() const = 0;

		//virtual void writeToStream(std::ostream& o) = 0;
	};



	template<typename ValueType>
	class Container : public ContainerInterface
	{
	public:

		ValueType value;

		Container(const ValueType& _value) :
			value(_value)
		{};

		virtual const std::type_info & getType() const
		{
			return typeid(ValueType);
		};

		virtual ContainerInterface * clone() const
		{
			return new Container(value);
		};
	};


	template<typename ValueType>
	friend ValueType *variant_cast(Variant *);




	ContainerInterface *container;


};
*/
