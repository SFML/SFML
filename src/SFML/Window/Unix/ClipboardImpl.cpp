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
#include <SFML/System/Clock.hpp>
#include <SFML/System/Err.hpp>
#include <X11/Xatom.h>
#include <vector>


namespace
{
    // Filter the events received by windows (only allow those matching a specific window)
    Bool checkEvent(::Display*, XEvent* event, XPointer userData)
    {
        // Just check if the event matches the window
        return event->xany.window == reinterpret_cast< ::Window >(userData);
    }
}

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
String ClipboardImpl::getString()
{
    return getInstance().getStringImpl();
}


////////////////////////////////////////////////////////////
void ClipboardImpl::setString(const String& text)
{
    getInstance().setStringImpl(text);
}


////////////////////////////////////////////////////////////
void ClipboardImpl::processEvents()
{
    getInstance().processEventsImpl();
}


////////////////////////////////////////////////////////////
ClipboardImpl::ClipboardImpl() :
m_window (0),
m_requestResponded(false)
{
    // Open a connection with the X server
    m_display = OpenDisplay();

    // Get the atoms we need to make use of the clipboard
    m_clipboard      = getAtom("CLIPBOARD",                      false);
    m_targets        = getAtom("TARGETS",                        false);
    m_text           = getAtom("TEXT",                           false);
    m_utf8String     = getAtom("UTF8_STRING",                    true );
    m_targetProperty = getAtom("SFML_CLIPBOARD_TARGET_PROPERTY", false);

    // Create a hidden window that will broker our clipboard interactions with X
    m_window = XCreateSimpleWindow(m_display, DefaultRootWindow(m_display), 0, 0, 1, 1, 0, 0, 0);

    // Register the events we are interested in
    XSelectInput(m_display, m_window, SelectionNotify | SelectionClear | SelectionRequest);
}


////////////////////////////////////////////////////////////
ClipboardImpl::~ClipboardImpl()
{
    // Destroy the window
    if (m_window)
    {
        XDestroyWindow(m_display, m_window);
        XFlush(m_display);
    }

    // Close the connection with the X server
    CloseDisplay(m_display);
}


////////////////////////////////////////////////////////////
ClipboardImpl& ClipboardImpl::getInstance()
{
    static ClipboardImpl instance;

    return instance;
}


////////////////////////////////////////////////////////////
String ClipboardImpl::getStringImpl()
{
    // Check if anybody owns the current selection
    if (XGetSelectionOwner(m_display, m_clipboard) == None)
    {
        m_clipboardContents.clear();

        return m_clipboardContents;
    }

    // Process any already pending events
    processEvents();

    m_requestResponded = false;

    // Request the current selection to be converted to UTF-8 (or STRING
    // if UTF-8 is not available) and written to our window property
    XConvertSelection(
        m_display,
        m_clipboard,
        (m_utf8String != None) ? m_utf8String : XA_STRING,
        m_targetProperty,
        m_window,
        CurrentTime
    );

    Clock clock;

    // Wait for a response for up to 1000ms
    while (!m_requestResponded && (clock.getElapsedTime().asMilliseconds() < 1000))
        processEvents();

    // If no response was received within the time period, clear our clipboard contents
    if (!m_requestResponded)
        m_clipboardContents.clear();

    return m_clipboardContents;
}


////////////////////////////////////////////////////////////
void ClipboardImpl::setStringImpl(const String& text)
{
    m_clipboardContents = text;

    // Set our window as the current owner of the selection
    XSetSelectionOwner(m_display, m_clipboard, m_window, CurrentTime);

    // Check if setting the selection owner was successful
    if (XGetSelectionOwner(m_display, m_clipboard) != m_window)
        err() << "Cannot set clipboard string: Unable to get ownership of X selection" << std::endl;
}


////////////////////////////////////////////////////////////
void ClipboardImpl::processEventsImpl()
{
    XEvent event;

    // Pick out the events that are interesting for this window
    while (XCheckIfEvent(m_display, &event, &checkEvent, reinterpret_cast<XPointer>(m_window)))
        m_events.push_back(event);

    // Handle the events for this window that we just picked out
    while (!m_events.empty())
    {
        event = m_events.front();
        m_events.pop_front();
        processEvent(event);
    }
}


////////////////////////////////////////////////////////////
void ClipboardImpl::processEvent(XEvent& windowEvent)
{
    switch (windowEvent.type)
    {
        case SelectionClear:
        {
            // We don't have any resources we need to clean up
            // when losing selection ownership so we don't do
            // anything when we receive SelectionClear
            // We will still respond to any future SelectionRequest
            // events since doing so doesn't really do any harm
            break;
        }
        case SelectionNotify:
        {
            // Notification that the current selection owner
            // has responded to our request

            XSelectionEvent& selectionEvent = *reinterpret_cast<XSelectionEvent*>(&windowEvent.xselection);

            m_clipboardContents.clear();

            // If retrieving the selection fails or conversion is unsuccessful
            // we leave the contents of the clipboard empty since we don't
            // own it and we don't know what it could currently be
            if ((selectionEvent.property == None) || (selectionEvent.selection != m_clipboard))
                break;

            Atom type;
            int format;
            unsigned long items;
            unsigned long remainingBytes;
            unsigned char* data = 0;

            // The selection owner should have wrote the selection
            // data to the specified window property
            int result = XGetWindowProperty(
                m_display,
                m_window,
                m_targetProperty,
                0,
                0x7fffffff,
                False,
                AnyPropertyType,
                &type,
                &format,
                &items,
                &remainingBytes,
                &data
            );

            if (result == Success)
            {
                // We don't support INCR for now
                // It is very unlikely that this will be returned
                // for purely text data transfer anyway
                if (type != getAtom("INCR", false))
                {
                    // Only copy the data if the format is what we expect
                    if ((type == m_utf8String) && (format == 8))
                    {
                        m_clipboardContents = String::fromUtf8(data, data + items);
                    }
                    else if ((type == XA_STRING) && (format == 8))
                    {
                        // Convert from ANSI std::string to sf::String
                        m_clipboardContents = std::string(data, data + items);
                    }
                }

                XFree(data);

                // The selection requestor must always delete the property themselves
                XDeleteProperty(m_display, m_window, m_targetProperty);
            }

            m_requestResponded = true;

            break;
        }
        case SelectionRequest:
        {
            // Respond to a request for our clipboard contents
            XSelectionRequestEvent& selectionRequestEvent = *reinterpret_cast<XSelectionRequestEvent*>(&windowEvent.xselectionrequest);

            // Our reply
            XSelectionEvent selectionEvent;

            selectionEvent.type      = SelectionNotify;
            selectionEvent.requestor = selectionRequestEvent.requestor;
            selectionEvent.selection = selectionRequestEvent.selection;
            selectionEvent.property  = selectionRequestEvent.property;
            selectionEvent.time      = selectionRequestEvent.time;

            if (selectionRequestEvent.selection == m_clipboard)
            {
                if (selectionRequestEvent.target == m_targets)
                {
                    // Respond to a request for our valid conversion targets
                    std::vector<Atom> targets;

                    targets.push_back(m_targets);
                    targets.push_back(m_text);
                    targets.push_back(XA_STRING);

                    if (m_utf8String != None)
                        targets.push_back(m_utf8String);

                    XChangeProperty(
                        m_display,
                        selectionRequestEvent.requestor,
                        selectionRequestEvent.property,
                        XA_ATOM,
                        32,
                        PropModeReplace,
                        reinterpret_cast<unsigned char*>(&targets[0]),
                        targets.size()
                    );

                    // Notify the requestor that they can read the targets from their window property
                    selectionEvent.target = m_targets;

                    XSendEvent(m_display, selectionRequestEvent.requestor, True, NoEventMask, reinterpret_cast<XEvent*>(&selectionEvent));

                    break;
                }
                else if ((selectionRequestEvent.target == XA_STRING) || ((m_utf8String == None) && (selectionRequestEvent.target == m_text)))
                {
                    // Respond to a request for conversion to a Latin-1 string
                    std::string data = m_clipboardContents.toAnsiString();

                    XChangeProperty(
                        m_display,
                        selectionRequestEvent.requestor,
                        selectionRequestEvent.property,
                        XA_STRING,
                        8,
                        PropModeReplace,
                        reinterpret_cast<const unsigned char*>(data.c_str()),
                        data.size()
                    );

                    // Notify the requestor that they can read the data from their window property
                    selectionEvent.target = XA_STRING;

                    XSendEvent(m_display, selectionRequestEvent.requestor, True, NoEventMask, reinterpret_cast<XEvent*>(&selectionEvent));

                    break;
                }
                else if ((m_utf8String != None) && ((selectionRequestEvent.target == m_utf8String) || (selectionRequestEvent.target == m_text)))
                {
                    // Respond to a request for conversion to a UTF-8 string
                    // or an encoding of our choosing (we always choose UTF-8)
                    std::basic_string<Uint8> data = m_clipboardContents.toUtf8();

                    XChangeProperty(
                        m_display,
                        selectionRequestEvent.requestor,
                        selectionRequestEvent.property,
                        m_utf8String,
                        8,
                        PropModeReplace,
                        reinterpret_cast<const unsigned char*>(data.c_str()),
                        data.size()
                    );

                    // Notify the requestor that they can read the data from their window property
                    selectionEvent.target = m_utf8String;

                    XSendEvent(m_display, selectionRequestEvent.requestor, True, NoEventMask, reinterpret_cast<XEvent*>(&selectionEvent));

                    break;
                }
            }

            // Notify the requestor that we could not respond to their request
            selectionEvent.target = selectionRequestEvent.target;
            selectionEvent.property = None;

            XSendEvent(m_display, selectionRequestEvent.requestor, True, NoEventMask, reinterpret_cast<XEvent*>(&selectionEvent));

            break;
        }
        default:
            break;
    }
}

} // namespace priv

} // namespace sf
