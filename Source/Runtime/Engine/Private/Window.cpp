// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Window.h"

/**
 * Constructor
 */
le::Window::Window() :
	handle( nullptr )
{}

/**
 * Destructor
 */
le::Window::~Window()
{
	Close();
}

/**
 * Show message box
 */
void le::Window::ShowMessageBox( const std::string& InMessage, EMessageBoxType InType )
{}

/**
 * Open window
 */
bool le::Window::Open( const std::string& InTitle, uint32 InWidth, uint32 InHeight, EStyleWindow InStyleWindow )
{
	return true;
}

/**
 * Initialize window instance for alrady created native window
 */
bool le::Window::Open( FWindowHandle InWindowHandle )
{
	return true;
}

/**
 * Close window
 */
void le::Window::Close()
{}
