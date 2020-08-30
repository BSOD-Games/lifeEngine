// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef OPENGL4RHI_H
#define OPENGL4RHI_H

#include "Rendering/RHI/IRHI.h"

namespace le
{
	class OpenGL4RHI : public IRHI
	{
	public:
		/* Constructor */
		OpenGL4RHI();

		/* Destructor */
		~OpenGL4RHI();

		/* Create render context */
		FRHIContext CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext = nullptr ) override;

		/* Make current context */
		bool MakeCurrentContext( FRHIContext InRHIContext ) override;

		/* Delete context */
		void DeleteContext( FRHIContext InRHIContext ) override;

		/* Swap buffers */
		void SwapBuffers( FRHIContext InRHIContext ) override;

		/* Set vertical sync */
		void SetVerticalSync( bool InIsEnable = true ) override;
	};
}

#endif // !OPENGL4RHI_H
