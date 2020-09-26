// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef BASESHADER_H
#define BASESHADER_H

#include <string>
#include <vector>

#include "Misc/Object.h"
#include "Misc/Types.h"
#include "ShaderVar.h"

namespace le
{
	class BaseShader : public Object
	{
	public:
		/* Constructor */
		BaseShader();

		/* Destructor */
		virtual ~BaseShader();

		/* Initialize shader */
		virtual bool Initialize( const std::vector< ShaderVar >* InShaderVars = nullptr ) = 0;

		/* Get name */
		virtual std::string GetName() const = 0;

		/* Get fallback shader */
		virtual std::string GetFallbackShader() const = 0;
	};
}

#endif // !BASESHADER_H
