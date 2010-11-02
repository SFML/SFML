#ifndef SFML_RUBYEXT_CLOCK_HEADER_
#define SFML_RUBYEXT_CLOCK_HEADER_

#include "ruby.h"

VALUE Clock_New( VALUE aKlass );

// Ruby initiation function
void Init_Clock( void );

#endif // SFML_RUBYEXT_CLOCK_HEADER_
