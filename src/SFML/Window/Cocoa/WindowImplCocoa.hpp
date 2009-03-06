////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Lucas Soltic (elmerod@gmail.com) and Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_WINDOWIMPLCOCOA_HPP
#define SFML_WINDOWIMPLCOCOA_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <string>

namespace sf
{
namespace priv
{

typedef struct objc_members objc_members;

////////////////////////////////////////////////////////////
/// WindowImplCocoa is the Cocoa implementation of WindowImpl
////////////////////////////////////////////////////////////
class WindowImplCocoa : public WindowImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    /// (creates a dummy window to provide a valid OpenGL context)
    ///
    ////////////////////////////////////////////////////////////
    WindowImplCocoa();

    ////////////////////////////////////////////////////////////
    /// Construct the window implementation from an existing control
    ///
    /// \param Handle : Platform-specific handle of the control
    /// \param Params : Creation parameters
    ///
	/// Note: the NSWindow object must not be defered !
    ////////////////////////////////////////////////////////////
    WindowImplCocoa(WindowHandle Handle, WindowSettings& params);

    ////////////////////////////////////////////////////////////
    /// Create the window implementation
    ///
    /// \param Mode :           Video mode to use
    /// \param Title :          Title of the window
    /// \param WindowStyle :    Window style
    /// \param Params :			Creation parameters
    ///
    ////////////////////////////////////////////////////////////
    WindowImplCocoa(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& params);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~WindowImplCocoa();

	////////////////////////////////////////////////////////////
    /// Check if there's an active context on the current thread
    ///
    /// \return True if there's a context bound to the current thread
    ///
    ////////////////////////////////////////////////////////////
    static bool IsContextActive();
	
	////////////////////////////////////////////////////////////
	/// Handle Cocoa NSEvent
	////////////////////////////////////////////////////////////
	void HandleEvent(void *eventRef);
	
	////////////////////////////////////////////////////////////
	/// Handle an event sent by the default NSNotificationCenter
	////////////////////////////////////////////////////////////
	void HandleNotifiedEvent(Event& eventRef);
	
	////////////////////////////////////////////////////////////
	/// Return a pointer to the NSWindow (objc->windowHandle) object
	////////////////////////////////////////////////////////////
	void *CocoaWindow(void);
	
	////////////////////////////////////////////////////////////
	/// Return whether the window is in full screen mode
	////////////////////////////////////////////////////////////
	bool IsFullscreen(void);
private :

    ////////////////////////////////////////////////////////////
    /// see WindowImpl::Display
    ///
    ////////////////////////////////////////////////////////////
    virtual void Display();

    ////////////////////////////////////////////////////////////
    /// see WindowImpl::ProcessEvents
    ///
    ////////////////////////////////////////////////////////////
    virtual void ProcessEvents();
	
    ////////////////////////////////////////////////////////////
    /// see WindowImpl::MakeActive
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetActive(bool Active = true) const;

    ////////////////////////////////////////////////////////////
    /// see WindowImpl::UseVerticalSync
    ///
    ////////////////////////////////////////////////////////////
    virtual void UseVerticalSync(bool Enabled);

    ////////////////////////////////////////////////////////////
    /// see WindowImpl::ShowMouseCursor
    ///
    ////////////////////////////////////////////////////////////
    virtual void ShowMouseCursor(bool Show);

    ////////////////////////////////////////////////////////////
    /// see WindowImpl::SetCursorPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetCursorPosition(unsigned int Left, unsigned int Top);

    ////////////////////////////////////////////////////////////
    /// see WindowImpl::SetPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetPosition(int Left, int Top);
	
	////////////////////////////////////////////////////////////
    /// /see WindowImpl::SetSize
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetSize(unsigned int Width, unsigned int Height);
	
    ////////////////////////////////////////////////////////////
    /// see WindowImpl::Show
    ///
    ////////////////////////////////////////////////////////////
    virtual void Show(bool State);
	
    ////////////////////////////////////////////////////////////
    /// see WindowImpl::EnableKeyRepeat
    ///
    ////////////////////////////////////////////////////////////
    virtual void EnableKeyRepeat(bool Enabled);
	
	////////////////////////////////////////////////////////////
    /// see WindowImpl::SetIcon
    ///
    ////////////////////////////////////////////////////////////
    virtual void SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels);
	
	
	
	////////////////////////////////////////////////////////////
	/// My own part starts here !
	////////////////////////////////////////////////////////////
	
	
	////////////////////////////////////////////////////////////
	/// Event handling for every event type.
	/// 'eventRef' is a NSEvent.
	////////////////////////////////////////////////////////////
	int HandleKeyDown(void *eventRef);
	int HandleKeyUp(void *eventRef);
	int HandleModifierKey(void *eventRef);
	int HandleMouseDown(void *eventRef);
	int HandleMouseUp(void *eventRef);
	int HandleMouseMove(void *eventRef);
	int HandleMouseWheel(void *eventRef);
	
	////////////////////////////////////////////////////////////
	/// Make some allocations and initializations
	////////////////////////////////////////////////////////////
	void Initialize(void);
	
	
	////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
	
	// An opaque structure that contains all obj-C objects
	objc_members *members;
	
	bool useKeyRepeat;
	bool mouseIn;
	float wheelStatus;
	
	bool fullscreen;
	VideoMode fullscreenMode;
	VideoMode desktopMode;
};

} // namespace priv

} // namespace sf

#endif // SFML_WINDOWIMPLCOCOA_HPP