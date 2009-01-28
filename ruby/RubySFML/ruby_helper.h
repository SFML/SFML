////////////////////////////////////////////////////////////
//
// RubySFML - Ruby extension for the SFML library
// Copyright (C) 2007 Sean O'Neil and Laurent Gomila
// (sean.p.oneil@gmail.com and laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef __ruby_helper_h__
#define __ruby_helper_h__
#define NOMINMAX
#include <string>
#include <algorithm>
#include <iostream>
#if defined(_WIN32) || defined(__WIN32__)
    #include <winsock2.h>
#endif
#include <ruby.h>

#if defined(_WIN32) || defined(__WIN32__)
    #define DECL __cdecl
#else
    #define DECL __attribute__((cdecl))
#endif

// Copies a string, makes the first character upper case, and then returns it
inline std::string fupr(const char *p) {
	std::string str = p;
	str[0] = toupper(str[0]);
	return str;
}

#define IS(v, c) rb_obj_class(v) == c
#define ISKO(v, c) rb_obj_is_kind_of(v, c)
#define ISNUM(v) ISKO(v, rb_cNumeric)
#define ISSTR(v) ISKO(v, rb_cString)

#define DECLARE_PTR_VAR(MyClass, MyVar, expr)\
	MyClass *p##MyVar = expr;\
	VALUE v##MyVar = p##MyVar == NULL ? Qnil : Data_Wrap_Struct(g_c##MyClass, 0, 0, p##MyVar)

#define DECLARE_OBJ_VAR(MyClass, MyVar, expr)\
	VALUE v##MyVar = MyClass##_new(0, NOARGV, g_c##MyClass);\
	MyClass *p##MyVar = NULL;\
	Data_Get_Struct(v##MyVar, MyClass, p##MyVar);\
	*p##MyVar = expr

#define GET_OBJ_VAR(MyClass, MyVar)\
	MyClass *p##MyVar = NULL;\
	Data_Get_Struct(v##MyVar, MyClass, p##MyVar)

#define DEFINE_CLASS_METHOD(MyClass, MyMethod, n)\
	rb_define_singleton_method(g_c##MyClass, #MyMethod,\
		(unsigned long (DECL *)(...))MyClass##_##MyMethod, n)

#define DEFINE_CLASS_METHOD2(MyClass, MyMethod, RubyMethod, n)\
	rb_define_singleton_method(g_c##MyClass, #RubyMethod,\
		(unsigned long (DECL *)(...))MyClass##_##MyMethod, n)

#define DEFINE_INSTANCE_METHOD(MyClass, MyMethod, n)\
	rb_define_method(g_c##MyClass, #MyMethod,\
		(unsigned long (DECL *)(...))MyClass##_##MyMethod, n)

#define DEFINE_INSTANCE_METHOD2(MyClass, MyMethod, RubyMethod, n)\
	rb_define_method(g_c##MyClass, #RubyMethod,\
		(unsigned long (DECL *)(...))MyClass##_##MyMethod, n)

// Use to define a standard getter method (for any type of member)
// (Defines both obj.variableName and obj.getVariableName)
#define DEFINE_GETTER(MyClass, MyField) \
	rb_define_method(g_c##MyClass, #MyField, \
		(unsigned long (DECL *)(...))MyClass##_get_##MyField, 0);\
	rb_define_method(g_c##MyClass, (std::string("get")+fupr(#MyField)).c_str(), \
		(unsigned long (DECL *)(...))MyClass##_get_##MyField, 0)

// Use to define a standard setter method (for any type of member)
// (Defines both obj.variableName = v and obj.setVariableName v)
#define DEFINE_SETTER(MyClass, MyField) \
	rb_define_method(g_c##MyClass, #MyField "=", \
		(unsigned long (DECL *)(...))MyClass##_set_##MyField, 1);\
	rb_define_method(g_c##MyClass, (std::string("set")+fupr(#MyField)).c_str(), \
		(unsigned long (DECL *)(...))MyClass##_set_##MyField, 1)

// Use to define standard getter and setter methods (for any type of member)
#define DEFINE_RW(MyClass, MyField) \
	DEFINE_GETTER(MyClass, MyField);\
	DEFINE_SETTER(MyClass, MyField)

// Use to define a standard getter method (for any type of member)
// (Defines both obj.variableName and obj.getVariableName)
#define DEFINE_GETTER2(MyClass, MyField, RubyField) \
	rb_define_method(g_c##MyClass, #RubyField, \
		(unsigned long (DECL *)(...))MyClass##_get_##MyField, 0);\
	rb_define_method(g_c##MyClass, (std::string("get")+fupr(#RubyField)).c_str(), \
		(unsigned long (DECL *)(...))MyClass##_get_##MyField, 0)

// Use to define a standard setter method (for any type of member)
// (Defines both obj.variableName = v and obj.setVariableName v)
#define DEFINE_SETTER2(MyClass, MyField, RubyField) \
	rb_define_method(g_c##MyClass, #RubyField "=", \
		(unsigned long (DECL *)(...))MyClass##_set_##MyField, 1);\
	rb_define_method(g_c##MyClass, (std::string("set")+fupr(#RubyField)).c_str(), \
		(unsigned long (DECL *)(...))MyClass##_set_##MyField, 1)

// Use to define standard getter and setter methods (for any type of member)
#define DEFINE_RW2(MyClass, MyField, RubyField) \
	DEFINE_GETTER2(MyClass, MyField, RubyField);\
	DEFINE_SETTER2(MyClass, MyField, RubyField)

// Use to define a static getter method (for any type of member)
// (Defines both Class.variableName and Class.getVariableName)
#define DEFINE_STATIC_GETTER(MyClass, MyField) \
	rb_define_singleton_method(g_c##MyClass, #MyField, \
		(unsigned long (DECL *)(...))MyClass##_get_##MyField, 0);\
	rb_define_singleton_method(g_c##MyClass, (std::string("get")+fupr(#MyField)).c_str(), \
		(unsigned long (DECL *)(...))MyClass##_get_##MyField, 0)

// Use to define a static setter method (for any type of member)
// (Defines both Class.variableName = v and Class.setVariableName v)
#define DEFINE_STATIC_SETTER(MyClass, MyField) \
	rb_define_singleton_method(g_c##MyClass, #MyField "=", \
		(unsigned long (DECL *)(...))MyClass##_set_##MyField, 1);\
	rb_define_singleton_method(g_c##MyClass, (std::string("set")+fupr(#MyField)).c_str(), \
		(unsigned long (DECL *)(...))MyClass##_set_##MyField, 1)

// Use to define static getter and setter methods (for any type of member)
#define DEFINE_STATIC_RW(MyClass, MyField) \
	DEFINE_STATIC_GETTER(MyClass, MyField);\
	DEFINE_STATIC_SETTER(MyClass, MyField)

#define DEFINE_MODULE_CONST(MyConst)\
	rb_define_const(g_vModule, #MyConst, INT2FIX(MyConst))

#define DEFINE_MODULE_METHOD(MyMethod, RubyMethod, n)\
	rb_define_singleton_method(g_vModule, #RubyMethod,\
		(unsigned long (DECL *)(...))SFML_##MyMethod, n)

#define DEFINE_INT_CONST(MyClass, MyConst)\
	rb_define_const(g_c##MyClass, #MyConst, INT2FIX(MyClass::MyConst))

#define DEFINE_PTR_CONST(MyClass, MyConst, expr)\
	rb_define_const(g_c##MyClass, #MyConst, Data_Wrap_Struct(g_c##MyClass, 0, 0, expr))


#define DECLARE_VOID_METHOD(MyClass, MyMethod)\
static VALUE MyClass##_##MyMethod(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyMethod();\
	return vSelf;\
}\

// Use to declare standard free, new, and init methods with 0 parameters
#define DECLARE_FREE_NEW_INIT0(MyClass)\
static void MyClass##_free(void *p)\
{\
	delete (MyClass *)p;\
}\
static VALUE MyClass##_new(VALUE vClass)\
{\
	VALUE argv[1];\
	MyClass *ptr = new MyClass;\
	VALUE tData = Data_Wrap_Struct(vClass, 0, MyClass##_free, ptr);\
	rb_obj_call_init(tData, 0, argv);\
	return tData;\
}\
static VALUE MyClass##_initialize(VALUE vSelf)\
{\
	return vSelf;\
}

// Use to declare standard free and new methods with 1 parameter
#define DECLARE_FREE_NEW1(MyClass)\
static void MyClass##_free(void *p)\
{\
	delete (MyClass *)p;\
}\
static VALUE MyClass##_new(VALUE vClass, VALUE v1)\
{\
	VALUE argv[1];\
	argv[0] = v1;\
	MyClass *ptr = new MyClass;\
	VALUE tData = Data_Wrap_Struct(vClass, 0, MyClass##_free, ptr);\
	rb_obj_call_init(tData, 1, argv);\
	return tData;\
}

// Use to define standard free, new, and init methods with n parameters
// (Also declares a free method)
#define DEFINE_CLASS_NEW_INIT(MyClass, n)\
	g_c##MyClass = rb_define_class(#MyClass, rb_cObject);\
	DEFINE_CLASS_METHOD(MyClass, new, n);\
	DEFINE_INSTANCE_METHOD(MyClass, initialize, n);


// Use to declare a standard getter method for a public int member
#define DECLARE_INT_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return INT2NUM((int)ptr->MyField);\
}

// Use to declare a static getter method for a public int member
#define DECLARE_STATIC_INT_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vClass)\
{\
	return INT2NUM((int)MyClass::MyField);\
}

// Use to declare a standard setter method for a public int member
#define DECLARE_INT_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = NUM2INT(v);\
	return v;\
}

// Use to declare a static setter method for a public int member
#define DECLARE_STATIC_INT_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vClass)\
{\
	MyClass::MyField = NUM2INT(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public int member
#define DECLARE_INT_RW(MyClass, MyField)\
DECLARE_INT_GETTER(MyClass, MyField)\
DECLARE_INT_SETTER(MyClass, MyField)

// Use to declare static getter and setter methods for a public int member
#define DECLARE_STATIC_INT_RW(MyClass, MyField)\
DECLARE_STATIC_INT_GETTER(MyClass, MyField)\
DECLARE_STATIC_INT_SETTER(MyClass, MyField)

// Use to declare a standard setter method for a public enum member
#define DECLARE_ENUM_SETTER(MyClass, MyField, EnumType)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = (EnumType)NUM2INT(v);\
	return v;\
}

// Use to declare a standard setter method for a public enum member
#define DECLARE_STATIC_ENUM_SETTER(MyClass, MyField, EnumType)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass::MyField = (EnumType)NUM2INT(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public enum member
#define DECLARE_ENUM_RW(MyClass, MyField, EnumType)\
DECLARE_INT_GETTER(MyClass, MyField)\
DECLARE_ENUM_SETTER(MyClass, MyField, EnumType)

// Use to declare standard getter and setter methods for a public enum member
#define DECLARE_STATIC_ENUM_RW(MyClass, MyField, EnumType)\
DECLARE_STATIC_INT_GETTER(MyClass, MyField)\
DECLARE_STATIC_ENUM_SETTER(MyClass, MyField, EnumType)

// Use to declare a standard getter method for a public bool member
#define DECLARE_BOOL_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return ptr->MyField ? Qtrue : Qfalse;\
}

// Use to declare a static getter method for a public bool member
#define DECLARE_STATIC_BOOL_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vClass)\
{\
	return MyClass::MyField ? Qtrue : Qfalse;\
}

// Use to declare a standard setter method for a public bool member
#define DECLARE_BOOL_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = RTEST(v) ? true : false;\
	return v;\
}

// Use to declare a static setter method for a public bool member
#define DECLARE_STATIC_BOOL_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vClass, VALUE v)\
{\
	MyClass::MyField = RTEST(v) ? true : false;\
	return v;\
}

// Use to declare standard getter and setter methods for a public int member
#define DECLARE_BOOL_RW(MyClass, MyField)\
DECLARE_BOOL_GETTER(MyClass, MyField)\
DECLARE_BOOL_SETTER(MyClass, MyField)

// Use to declare standard getter and setter methods for a public int member
#define DECLARE_STATIC_BOOL_RW(MyClass, MyField)\
DECLARE_STATIC_BOOL_GETTER(MyClass, MyField)\
DECLARE_STATIC_BOOL_SETTER(MyClass, MyField)

// Use to declare a standard getter method for a public double member
#define DECLARE_DOUBLE_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return rb_float_new((double)ptr->MyField);\
}

// Use to declare a static getter method for a public double member
#define DECLARE_STATIC_DOUBLE_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	return rb_float_new((double)MyClass::MyField);\
}

// Use to declare a standard setter method for a public double member
#define DECLARE_DOUBLE_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = (float)NUM2DBL(v);\
	return v;\
}

// Use to declare a static setter method for a public double member
#define DECLARE_STATIC_DOUBLE_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass::MyField = NUM2DBL(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public double member
#define DECLARE_DOUBLE_RW(MyClass, MyField)\
DECLARE_DOUBLE_GETTER(MyClass, MyField)\
DECLARE_DOUBLE_SETTER(MyClass, MyField)

// Use to declare standard getter and setter methods for a public double member
#define DECLARE_STATIC_DOUBLE_RW(MyClass, MyField)\
DECLARE_DOUBLE_STATIC_GETTER(MyClass, MyField)\
DECLARE_DOUBLE_STATIC_SETTER(MyClass, MyField)

// Use to declare a standard getter method for a public std::string member
#define DECLARE_STRING_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return rb_str_new2(ptr->MyField.c_str());\
}

// Use to declare a static getter method for a public std::string member
#define DECLARE_STATIC_STRING_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	return rb_str_new2(MyClass::MyField.c_str());\
}

// Use to declare a standard setter method for a public std::string member
#define DECLARE_STRING_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = STR2CSTR(v);\
	return v;\
}

// Use to declare a static setter method for a public std::string member
#define DECLARE_STATIC_STRING_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass::MyField = STR2CSTR(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public std::string member
#define DECLARE_STRING_RW(MyClass, MyField)\
DECLARE_STRING_GETTER(MyClass, MyField)\
DECLARE_STRING_SETTER(MyClass, MyField)

// Use to declare standard getter and setter methods for a public int member
#define DECLARE_STATIC_STRING_RW(MyClass, MyField)\
DECLARE_STATIC_STRING_GETTER(MyClass, MyField)\
DECLARE_STATIC_STRING_SETTER(MyClass, MyField)

// Use to declare a standard getter method for a public short member
#define DECLARE_SHORT_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return INT2FIX((int)ptr->MyField);\
}

// Use to declare a standard getter method for a public short member
#define DECLARE_STATIC_SHORT_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	return INT2FIX((int)MyClass->MyField);\
}

// Use to declare a standard setter method for a public short member
#define DECLARE_SHORT_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = (short)FIX2INT(v);\
	return v;\
}

// Use to declare a standard setter method for a public short member
#define DECLARE_STATIC_SHORT_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass::MyField = (short)FIX2INT(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public short member
#define DECLARE_SHORT_RW(MyClass, MyField)\
DECLARE_SHORT_GETTER(MyClass, MyField)\
DECLARE_SHORT_SETTER(MyClass, MyField)

// Use to declare standard getter and setter methods for a public short member
#define DECLARE_STATIC_SHORT_RW(MyClass, MyField)\
DECLARE_STATIC_SHORT_GETTER(MyClass, MyField)\
DECLARE_STATIC_SHORT_SETTER(MyClass, MyField)


// Use to declare a standard getter method for a public object member
#define DECLARE_OBJ_GETTER(MyClass, MyField, FieldType)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	VALUE vRet = FieldType##_new(0, NOARGV, g_c##FieldType);\
	FieldType *pRet;\
	Data_Get_Struct(vRet, FieldType, pRet);\
	*pRet = pSelf->MyField;\
	return vRet;\
}

// Use to declare a static getter method for a public object member
#define DECLARE_STATIC_OBJ_GETTER(MyClass, MyField, FieldType)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	VALUE vRet = FieldType##_new(0, NOARGV, g_c##FieldType);\
	FieldType *pRet;\
	Data_Get_Struct(vRet, FieldType, pRet);\
	*pRet = MyClass::MyField;\
	return vRet;\
}

// Use to declare a standard setter method for a public object member
#define DECLARE_OBJ_SETTER(MyClass, MyField, FieldType)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	FieldType *pV;\
	Data_Get_Struct(v, FieldType, pV);\
	pSelf->MyField = *pV;\
	return Qnil;\
}

// Use to declare a static setter method for a public object member
#define DECLARE_STATIC_OBJ_SETTER(MyClass, MyField, FieldType)\
static VALUE MyClass##_set_##MyField(VALUE vClass, VALUE v)\
{\
	FieldType *pV;\
	Data_Get_Struct(v, FieldType, pV);\
	MyClass::MyField = *pV;\
	return Qnil;\
}

// Use to declare standard getter and setter methods for a public object member
#define DECLARE_OBJ_RW(MyClass, MyField, FieldType)\
DECLARE_OBJ_GETTER(MyClass, MyField, FieldType)\
DECLARE_OBJ_SETTER(MyClass, MyField, FieldType)

// Use to declare static getter and setter methods for a public object member
#define DECLARE_STATIC_OBJ_RW(MyClass, MyField, FieldType)\
DECLARE_STATIC_OBJ_GETTER(MyClass, MyField, FieldType)\
DECLARE_STATIC_OBJ_SETTER(MyClass, MyField, FieldType)


// Use to declare a standard getter method for a public unsigned short member
#define DECLARE_USHORT_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return INT2FIX((int)ptr->MyField);\
}

// Use to declare a standard getter method for a public unsigned short member
#define DECLARE_STATIC_USHORT_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	return INT2FIX((int)MyClass->MyField);\
}

// Use to declare a standard setter method for a public unsigned short member
#define DECLARE_USHORT_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = (unsigned short)FIX2INT(v);\
	return v;\
}

// Use to declare a standard setter method for a public unsigned short member
#define DECLARE_STATIC_USHORT_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass::MyField = (unsigned short)FIX2INT(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public unsigned short member
#define DECLARE_USHORT_RW(MyClass, MyField)\
DECLARE_USHORT_GETTER(MyClass, MyField)\
DECLARE_USHORT_SETTER(MyClass, MyField)

// Use to declare standard getter and setter methods for a public unsigned short member
#define DECLARE_STATIC_USHORT_RW(MyClass, MyField)\
DECLARE_STATIC_USHORT_GETTER(MyClass, MyField)\
DECLARE_STATIC_USHORT_SETTER(MyClass, MyField)


// Use to declare a standard getter method for a public unsigned char member
#define DECLARE_UCHAR_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return INT2FIX((int)ptr->MyField);\
}

// Use to declare a standard getter method for a public unsigned char member
#define DECLARE_STATIC_UCHAR_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	return INT2FIX((int)MyClass->MyField);\
}

// Use to declare a standard setter method for a public unsigned char member
#define DECLARE_UCHAR_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = (unsigned char)FIX2INT(v);\
	return v;\
}

// Use to declare a standard setter method for a public unsigned char member
#define DECLARE_STATIC_UCHAR_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass::MyField = (unsigned char)FIX2INT(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public unsigned char member
#define DECLARE_UCHAR_RW(MyClass, MyField)\
DECLARE_UCHAR_GETTER(MyClass, MyField)\
DECLARE_UCHAR_SETTER(MyClass, MyField)

// Use to declare standard getter and setter methods for a public unsigned char member
#define DECLARE_STATIC_UCHAR_RW(MyClass, MyField)\
DECLARE_STATIC_UCHAR_GETTER(MyClass, MyField)\
DECLARE_STATIC_UCHAR_SETTER(MyClass, MyField)


// Use to declare a standard getter method for a public char member
#define DECLARE_CHAR_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return INT2FIX((int)ptr->MyField);\
}

// Use to declare a standard getter method for a public char member
#define DECLARE_STATIC_CHAR_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	return INT2FIX((int)MyClass->MyField);\
}

// Use to declare a standard setter method for a public char member
#define DECLARE_CHAR_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = (char)FIX2INT(v);\
	return v;\
}

// Use to declare a standard setter method for a public char member
#define DECLARE_STATIC_CHAR_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass::MyField = (char)FIX2INT(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public char member
#define DECLARE_CHAR_RW(MyClass, MyField)\
DECLARE_CHAR_GETTER(MyClass, MyField)\
DECLARE_CHAR_SETTER(MyClass, MyField)

// Use to declare standard getter and setter methods for a public char member
#define DECLARE_STATIC_CHAR_RW(MyClass, MyField)\
DECLARE_STATIC_CHAR_GETTER(MyClass, MyField)\
DECLARE_STATIC_CHAR_SETTER(MyClass, MyField)

// Use to declare a standard getter method for a public int64 member
#define DECLARE_INT64_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return LL2NUM(ptr->MyField);\
}

// Use to declare a static getter method for a public int64 member
#define DECLARE_STATIC_INT64_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vClass)\
{\
	return LL2NUM(MyClass::MyField);\
}

// Use to declare a standard setter method for a public int64 member
#define DECLARE_INT64_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = NUM2LL(v);\
	return v;\
}

// Use to declare a static setter method for a public int64 member
#define DECLARE_STATIC_INT64_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vClass)\
{\
	MyClass::MyField = NUM2LL(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public int64 member
#define DECLARE_INT64_RW(MyClass, MyField)\
DECLARE_INT64_GETTER(MyClass, MyField)\
DECLARE_INT64_SETTER(MyClass, MyField)

// Use to declare static getter and setter methods for a public int64 member
#define DECLARE_STATIC_INT64_RW(MyClass, MyField)\
DECLARE_STATIC_INT64_GETTER(MyClass, MyField)\
DECLARE_STATIC_INT64_SETTER(MyClass, MyField)


// Use to declare a standard getter method for a public uint64 member
#define DECLARE_UINT64_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	return ULL2NUM(ptr->MyField);\
}

// Use to declare a static getter method for a public uint64 member
#define DECLARE_STATIC_UINT64_GETTER(MyClass, MyField)\
static VALUE MyClass##_get_##MyField(VALUE vClass)\
{\
	return ULL2NUM(MyClass::MyField);\
}

// Use to declare a standard setter method for a public uint64 member
#define DECLARE_UINT64_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *ptr;\
	Data_Get_Struct(vSelf, MyClass, ptr);\
	ptr->MyField = NUM2ULL(v);\
	return v;\
}

// Use to declare a static setter method for a public uint64 member
#define DECLARE_USTATIC_INT64_SETTER(MyClass, MyField)\
static VALUE MyClass##_set_##MyField(VALUE vClass)\
{\
	MyClass::MyField = NUM2ULL(v);\
	return v;\
}

// Use to declare standard getter and setter methods for a public uint64 member
#define DECLARE_UINT64_RW(MyClass, MyField)\
DECLARE_UINT64_GETTER(MyClass, MyField)\
DECLARE_UINT64_SETTER(MyClass, MyField)

// Use to declare static getter and setter methods for a public uint64 member
#define DECLARE_STATIC_UINT64_RW(MyClass, MyField)\
DECLARE_STATIC_UINT64_GETTER(MyClass, MyField)\
DECLARE_STATIC_UINT64_SETTER(MyClass, MyField)


#define DECLARE_INT_ARR_GETTER(MyClass, MyField, Size)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	VALUE v = rb_ary_new();\
	for(int i=0; i<Size; i++)\
		rb_ary_push(v, INT2FIX(pSelf->MyField[i]));\
	return v;\
}

#define DECLARE_INT_ARR_SETTER(MyClass, MyField, Size)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	for(int i=0; i<Size; i++)\
		pSelf->MyField[i] = FIX2INT(rb_ary_entry(v, i));\
	return v;\
}

#define DECLARE_INT_ARR_RW(MyClass, MyField, Size)\
DECLARE_INT_ARR_GETTER(MyClass, MyField, Size);\
DECLARE_INT_ARR_SETTER(MyClass, MyField, Size)


#define DECLARE_DOUBLE_ARR_GETTER(MyClass, MyField, Size)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	VALUE v = rb_ary_new();\
	for(int i=0; i<Size; i++)\
		rb_ary_push(v, rb_float_new(pSelf->MyField[i]));\
	return v;\
}

#define DECLARE_DOUBLE_ARR_SETTER(MyClass, MyField, Size)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	for(int i=0; i<Size; i++)\
		pSelf->MyField[i] = NUM2DBL(rb_ary_entry(v, i));\
	return v;\
}

#define DECLARE_DOUBLE_ARR_RW(MyClass, MyField, Size)\
DECLARE_DOUBLE_ARR_GETTER(MyClass, MyField, Size);\
DECLARE_DOUBLE_ARR_SETTER(MyClass, MyField, Size)


#define DECLARE_ADDR_GETTER(MyClass, MyField, FieldType)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	VALUE vRet = Data_Wrap_Struct(g_c##FieldType, 0, 0, &pSelf->MyField);\
	return vRet;\
}

// Use to declare a standard getter method for a public object pointer member
#define DECLARE_PTR_GETTER(MyClass, MyField, FieldType)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	VALUE vRet = Data_Wrap_Struct(g_c##FieldType, 0, 0, pSelf->MyField);\
	return vRet;\
}


// Use to declare a standard getter method for a public object reference member
#define DECLARE_REF_GETTER(MyClass, MyField, FieldType)\
static VALUE MyClass##_get_##MyField(VALUE vSelf)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	pSelf->MyField->ReferenceCountedObject_refCount.increment();\
	VALUE vRet = Data_Wrap_Struct(g_c##FieldType, 0, FieldType##_free, pSelf->MyField.pointer());\
	return vRet;\
}

// Use to declare a standard setter method for a public object reference member
#define DECLARE_REF_SETTER(MyClass, MyField, FieldType)\
static VALUE MyClass##_set_##MyField(VALUE vSelf, VALUE v)\
{\
	MyClass *pSelf;\
	Data_Get_Struct(vSelf, MyClass, pSelf);\
	FieldType *pV;\
	Data_Get_Struct(v, FieldType, pV);\
	pSelf->MyField = pV;\
	return Qnil;\
}

// Use to declare standard getter and setter methods for a public object member
#define DECLARE_REF_RW(MyClass, MyField, FieldType)\
DECLARE_REF_GETTER(MyClass, MyField, FieldType)\
DECLARE_REF_SETTER(MyClass, MyField, FieldType)


#endif // __ruby_helper_h__

