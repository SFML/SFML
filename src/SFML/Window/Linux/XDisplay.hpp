#ifndef SFML_XDISPLAY_HPP
#define SFML_XDISPLAY_HPP

#include <X11/Xlib.h>

Display *SXOpenDisplay();
void SXCloseDisplay();

#endif //SFML_XDISPLAY_HPP
