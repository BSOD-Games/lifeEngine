// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Window.h"

/**
 * Constructor window
 */
le::Window::Window() :
	handle( nullptr )
{}

/**
 * Destructor window
 */
le::Window::~Window()
{}

/**
 * Open window
 */
bool le::Window::Open( const std::string& InTitle, uint32 InWidth, uint32 InHeight, EStyleWindow InStyleWindow )
{
	return true;
}

/**
 * Set window handle
 */
void le::Window::SetHandle( WindowHandle_t InWindowHandle )
{
	if ( !handle ) Close();
	handle = InWindowHandle;
}

/**
 * Close window
 */
void le::Window::Close()
{}

/**
 * Get window handle
 */
le::WindowHandle_t le::Window::GetHandle() const
{
	return handle;
}