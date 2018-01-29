////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Unix/ClipboardImpl.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Sleep.hpp>
#include <iostream>
#include <string>
#include <X11/Xlib.h>


namespace
{
////////////////////////////////////////////////////////////
void initClipboard();
void* hostSelection(void*);

sf::String string;

pthread_mutex_t mutex;
pthread_t host_thread;

bool is_fail = false;
bool is_init = false;
bool is_host = false;

Display* display = NULL;
Window window = 0;

Atom selection = 0;
Atom atom_targ = 0;
Atom atom_text = 0;
Atom utf8_text = 0;
int xa_string = 31;
int xa_atom = 4;

////////////////////////////////////////////////////////////
void initClipboard()
{
    is_init = true;

    display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen),
        0, 0, 1, 1, 0, BlackPixel(display, screen), WhitePixel(display, screen));

    selection = XInternAtom(display, "CLIPBOARD", false);
    atom_targ = XInternAtom(display, "TARGETS", false);
    atom_text = XInternAtom(display, "TEXT", false);
    utf8_text = XInternAtom(display, "UTF8_STRING", true);

    if(utf8_text == None)
    {
        std::cerr << "UTF-8 format unavailable on clipboard." << std::endl;
        utf8_text = xa_string;
    }

    if(pthread_mutex_init(&mutex, NULL))
    {
        is_fail = true;
        std::cerr << "Unable to initialize mutex. Failed to initialize clipboard." << std::endl;
        return;
    }

    if(pthread_create(&host_thread, NULL, hostSelection, NULL))
    {
        is_fail = true;
        std::cerr << "Unable to create host thread. Failed to initialize clipboard." << std::endl;
        return;
    }
}

////////////////////////////////////////////////////////////
void* hostSelection(void*)
{
    while(true)
    {
        if(XPending(display) && is_host)
        {
            XEvent event;

            pthread_mutex_lock(&mutex);
            XNextEvent(display, &event);
            pthread_mutex_unlock(&mutex);

            switch(event.type)
            {
                case SelectionClear:
                {
                    pthread_mutex_lock(&mutex);
                    is_host = false;
                    pthread_mutex_unlock(&mutex);

                    break;
                }
                case SelectionRequest:
                {
                    if(event.xselectionrequest.selection == selection)
                    {
                        XSelectionRequestEvent* sel_req_event = &event.xselectionrequest;
                        XSelectionEvent sel_event = {0};

                        int result = 0;
                        sel_event.type = SelectionNotify,
                        sel_event.display = sel_req_event->display,
                        sel_event.requestor = sel_req_event->requestor,
                        sel_event.selection = sel_req_event->selection,
                        sel_event.time = sel_req_event->time,
                        sel_event.target = sel_req_event->target,
                        sel_event.property = sel_req_event->property;

                        std::basic_string<unsigned char> str = string.toUtf8();

                        if(sel_event.target == atom_targ)
                            result = XChangeProperty(sel_event.display, sel_event.requestor,
                                sel_event.property, xa_atom, 32, PropModeReplace,
                                reinterpret_cast<unsigned char*>(&utf8_text), 1);
                        else if(sel_event.target == xa_string || sel_event.target == atom_text)
                            result = XChangeProperty(sel_event.display, sel_event.requestor,
                                sel_event.property, xa_string, 8, PropModeReplace,
                                reinterpret_cast<unsigned char*>(&str[0]), str.size());
                        else if(sel_event.target == utf8_text)
                            result = XChangeProperty(sel_event.display, sel_event.requestor,
                                sel_event.property, utf8_text, 8, PropModeReplace,
                                reinterpret_cast<unsigned char*>(&str[0]), str.size());
                        else
                            sel_event.property = None;

                        if((result & 2) == 0)
                            XSendEvent(display, sel_event.requestor, 0, 0,
                                reinterpret_cast<XEvent*>(&sel_event));
                    }
                    break;
                }
                default: break;
            }
        }
        else
            sf::sleep(sf::milliseconds(20));
    }
}
}

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
String ClipboardImpl::getString()
{
    if(!is_init)
        initClipboard();

    if(is_fail || is_host)
        return string;

    // Dangerous! Wipes all previous events!
    XSync(display, true);
    XConvertSelection(display, selection, utf8_text, atom_text, window, CurrentTime);

    XEvent event;

    pthread_mutex_lock(&mutex);
    XNextEvent(display, &event);
    pthread_mutex_unlock(&mutex);

    if(event.type == SelectionNotify)
    {
        if(event.xselection.selection != selection || event.xselection.target != utf8_text)
        {
            std::cerr << "Failed to convert selection." << std::endl;
            return string;
        }

        if(event.xselection.property)
        {
            Atom target;
            int format;
            unsigned long size;
            unsigned long byte_left;
            unsigned char* data;

            XGetWindowProperty(event.xselection.display,
                event.xselection.requestor, event.xselection.property,
                0L, (~0L), false, AnyPropertyType,
                &target, &format, &size, &byte_left, &data);

            if(target == utf8_text)
            {
                std::basic_string<unsigned char> str(data, size);
                string = sf::String::fromUtf8(str.begin(), str.end());

                XFree(data);
            }

            XDeleteProperty(event.xselection.display, event.xselection.requestor, event.xselection.property);
        }
    }

    return string;
}


////////////////////////////////////////////////////////////
void ClipboardImpl::setString(const String& text)
{
    if(!is_init)
        initClipboard();

    if(is_fail)
        return;

    if(!is_host)
    {
        XSetSelectionOwner(display, selection, window, CurrentTime);

        if(XGetSelectionOwner(display, selection) != window)
        {
            std::cerr << "Unable to get ownership of selection." << std::endl;
            return;
        }

        pthread_mutex_lock(&mutex);
        is_host = true;
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    string = text;
    pthread_mutex_unlock(&mutex);
}

} // namespace priv

} // namespace sf
