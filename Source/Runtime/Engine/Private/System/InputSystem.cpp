// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <memory>

#include "Misc/Types.h"
#include "System/InputSystem.h"
#include "System/Event.h"

/**
 * Constructor
 */
le::InputSystem::InputSystem()
{
	memset( buttonEvents, BE_None, BC_Count * sizeof( EButtonEvents ) );
}

/**
 * Destructor
 */
le::InputSystem::~InputSystem()
{}

/**
 * Apply event
 */
void le::InputSystem::ApplyEvent( const SEvent& InEvent )
{
	switch ( InEvent.type )
	{
	case SEvent::ET_KeyPressed:			buttonEvents[ InEvent.event.key.code ] = BE_Pressed;			break;
	case SEvent::ET_KeyReleased:		buttonEvents[ InEvent.event.key.code ] = BE_Released;			break;

	case SEvent::ET_MousePressed:		buttonEvents[ InEvent.event.mouseButton.code ] = BE_Pressed;	break;
	case SEvent::ET_MouseReleased:		buttonEvents[ InEvent.event.mouseButton.code ] = BE_Released;	break;

	case SEvent::ET_MouseMove:
		mouseOffset.x += static_cast< float >( InEvent.event.mouseMove.xDirection );
		mouseOffset.y += static_cast< float >( InEvent.event.mouseMove.yDirection );
		mousePosition.x = static_cast< float >( InEvent.event.mouseMove.x );
		mousePosition.y = static_cast< float >( InEvent.event.mouseMove.y );
		break;

	case SEvent::ET_MouseWheel:
		buttonEvents[ InEvent.event.mouseWheel.y > 0 ? BC_MouseWheelUp : BC_MouseWheelDown ] = BE_Scrolled;
		break;

	case SEvent::ET_TextInput:
		// TODO: Add apply text event
		break;
	}
}

/**
 * Update
 */
void le::InputSystem::Update()
{}

/**
 * Reset
 */
void le::InputSystem::Reset()
{
	for ( uint32 index = 0; index < BC_Count; ++index )
		if ( buttonEvents[ index ] == BE_Released || buttonEvents[ index ] == BE_Scrolled )
			buttonEvents[ index ] = BE_None;

	mousePosition = mouseOffset = FVector2D( 0.f, 0.f );
}

/**
 * Is key down
 */
bool le::InputSystem::IsKeyDown( EButtonCode InButtonCode ) const
{
	LIFEENGINE_ASSERT( InButtonCode > BC_KeyFirst && InButtonCode < BC_KeyLast );
	return buttonEvents[ InButtonCode ] == BE_Pressed;
}

/**
 * Is key up
 */
bool le::InputSystem::IsKeyUp( EButtonCode InButtonCode ) const
{
	LIFEENGINE_ASSERT( InButtonCode > BC_KeyFirst && InButtonCode < BC_KeyLast );
	return buttonEvents[ InButtonCode ] == BE_Released;
}

/**
 * Is mouse key down
 */
bool le::InputSystem::IsMouseKeyDown( EButtonCode InButtonCode ) const
{
	LIFEENGINE_ASSERT( InButtonCode > BC_MouseFirst && InButtonCode < BC_MouseLast );
	return buttonEvents[ InButtonCode ] == BE_Pressed;
}

/**
 * Is mouse key up
 */
bool le::InputSystem::IsMouseKeyUp( EButtonCode InButtonCode ) const
{
	LIFEENGINE_ASSERT( InButtonCode > BC_MouseFirst && InButtonCode < BC_MouseLast );
	return buttonEvents[ InButtonCode ] == BE_Released;
}

/**
 * Is mouse wheel
 */
bool le::InputSystem::IsMouseWheel( EButtonCode InButtonCode ) const
{
	LIFEENGINE_ASSERT( InButtonCode == BC_MouseWheelDown || InButtonCode == BC_MouseWheelUp );
	return buttonEvents[ InButtonCode ] == BE_Scrolled;
}

/**
 * Get mouse position
 */
const le::FVector2D& le::InputSystem::GetMousePosition() const
{
	return mousePosition;
}

/**
 * Get mouse offset
 */
const le::FVector2D& le::InputSystem::GetMouseOffset() const
{
	return mouseOffset;
}
