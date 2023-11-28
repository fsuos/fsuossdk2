#ifndef PROVIDER_HPP
#define PROVIDER_HPP

class Provider{
public:
	virtual ~Provider() {}
};

////////////////////////////////////////////////////////////
// Macro that helps host applications defining
// their provider classes
////////////////////////////////////////////////////////////
#define PLUMA_PROVIDER_HEADER(TYPE)\
PLUMA_PROVIDER_HEADER_BEGIN(TYPE)\
virtual TYPE* create() const = 0;\
PLUMA_PROVIDER_HEADER_END

////////////////////////////////////////////////////////////
// Macro that generate first part of the provider definition
////////////////////////////////////////////////////////////
#define PLUMA_PROVIDER_HEADER_BEGIN(TYPE)\
class TYPE##Provider: public Provider{\
public:


////////////////////////////////////////////////////////////
// Macro that generate last part of the provider definition
////////////////////////////////////////////////////////////
#define PLUMA_PROVIDER_HEADER_END };

////////////////////////////////////////////////////////////
// Macro that generate the provider declaration
////////////////////////////////////////////////////////////
#define PLUMA_PROVIDER_SOURCE(TYPE, Version, LowestVersion) 


////////////////////////////////////////////////////////////
// Macro that helps plugins generating their provider implementations
// PRE: SPECIALIZED_TYPE must inherit from BASE_TYPE
////////////////////////////////////////////////////////////
#define PLUMA_INHERIT_PROVIDER(SPECIALIZED_TYPE, BASE_TYPE)\
class SPECIALIZED_TYPE##Provider: public BASE_TYPE##Provider{\
public:\
	BASE_TYPE * create() const{ return new SPECIALIZED_TYPE (); }\
};

#endif
