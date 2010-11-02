#ifndef SFML_RUBYEXT_SYSTEM_HEADER_
#define SFML_RUBYEXT_SYSTEM_HEADER_

#include "ruby.h"

VALUE GetNamespace( void );

// Ruby initiation function
extern "C" void Init_system( void );

typedef VALUE ( *RubyFunctionPtr )( ... );

#define FUNCPTR( x ) ( reinterpret_cast< RubyFunctionPtr >( x ) )

#endif // SFML_RUBYEXT_SYSTEM_HEADER_
