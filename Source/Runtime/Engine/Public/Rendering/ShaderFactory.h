// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include <string>
#include <unordered_map>

#include "Misc/Object.h"

namespace le
{
	class BaseShader;
	typedef BaseShader*			( *FCreateShaderFn )( );

	class ShaderFactory : public Object
	{
	public:
		/* Register shader */
		void Register( const std::string& InShaderName, FCreateShaderFn InCreateShaderFunction );

		/* Create shader */
		BaseShader* Create( const std::string& InShaderName ) const;

	private:
		mutable std::unordered_map< std::string, FCreateShaderFn >		shaders;
	};
}

#endif // !SHADERFACTORY_H
