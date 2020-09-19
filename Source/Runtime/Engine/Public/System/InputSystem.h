// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "Math/Vector2D.h"
#include "System/Object.h"
#include "ButtonCode.h"

namespace le
{
	struct SEvent;

	enum EButtonEvents
	{
		BE_None,
		BE_Pressed,
		BE_Released,
		BE_Scrolled
	};

	class InputSystem : public Object
	{
	public:
		/* Constructor */
		InputSystem();

		/* Destructor */
		~InputSystem();

		/* Apply event */
		void ApplyEvent( const SEvent& InEvent );

		/* Update */
		void Update();

		/* Reset states */
		void Reset();

		/* Is key down */
		bool IsKeyDown( EButtonCode InButtonCode ) const;

		/* Is key up */
		bool IsKeyUp( EButtonCode InButtonCode ) const;

		/* Is mouse key down */
		bool IsMouseKeyDown( EButtonCode InButtonCode ) const;

		/* Is mouse key up */
		bool IsMouseKeyUp( EButtonCode InButtonCode ) const;

		/* Is mouse wheel */
		bool IsMouseWheel( EButtonCode InButtonCode ) const;

		/* Get mouse position */
		const SVector2D& GetMousePosition() const;

		/* Get mouse offset */
		const SVector2D& GetMouseOffset() const;

	private:
		EButtonEvents			buttonEvents[ BC_Count ];
		SVector2D				mousePosition;
		SVector2D				mouseOffset;
	};
}

#endif // !INPUTSYSTEM_H
