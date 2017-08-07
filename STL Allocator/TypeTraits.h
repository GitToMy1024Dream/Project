#pragma once

#pragma once

struct __TrueType
{};

struct __FalseType
{};

template<class _Tp>
struct TypeTraits
{
	typedef __FalseType has_trivial_default_constructor;
	typedef __FalseType has_trivial_copy_constructor;
	typedef __FalseType has_trivial_assigment_operator;
	typedef __FalseType has_trivial_destructor;
	typedef __FalseType __IsPODType;
};

template<>
struct TypeTraits<bool>
{
	typedef __TrueType has_trivial_default_constructor;
	typedef __TrueType has_trivial_copy_constructor;
	typedef __TrueType has_trivial_assignment_operator;
	typedef __TrueType has_trivial_destructor;
	typedef __TrueType __IsPODType;
};

template<>
struct TypeTraits<int>
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    __IsPODType;
};

template<>
struct TypeTraits<char>
{
	typedef __TrueType    has_trivial_default_constructor;
	typedef __TrueType    has_trivial_copy_constructor;
	typedef __TrueType    has_trivial_assignment_operator;
	typedef __TrueType    has_trivial_destructor;
	typedef __TrueType    __IsPODType;
};