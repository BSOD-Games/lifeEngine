// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include <string>
#include <unordered_map>

#include "Misc/Object.h"
#include "Misc/Types.h"

namespace le
{
	typedef FBaseShaderRef			( *FCreateShaderFn )( );

	class ShaderFactory : public Object
	{
	public:
		/* Register shader */
		void Register( const std::string& InShaderName, FCreateShaderFn InCreateShaderFunction );

		/* Create shader */
		FBaseShaderRef Create( const std::string& InShaderName ) const;

	private:
		mutable std::unordered_map< std::string, FCreateShaderFn >		shaders;
	};
}

#endif // !SHADERFACTORY_H
