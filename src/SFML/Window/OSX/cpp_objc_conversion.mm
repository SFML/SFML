

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Utf.hpp>

#import <SFML/Window/OSX/cpp_objc_conversion.h>

////////////////////////////////////////////////////////////
NSString* stringToNSString(std::string const& string)
{
	std::string utf8; utf8.reserve(string.size() + 1);
	sf::Utf8::FromAnsi(string.begin(), string.end(), std::back_inserter(utf8));
	NSString* str = [NSString stringWithCString:utf8.c_str() encoding:NSUTF8StringEncoding];
	
	return str;
}

