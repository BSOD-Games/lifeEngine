// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Logging/LogMacros.h"
#include "Rendering/BaseShader.h"
#include "Rendering/ShaderFactory.h"

/**
 * Register shader
 */
void le::ShaderFactory::Register( const std::string& InShaderName, FCreateShaderFn InCreateShaderFunction )
{
	LIFEENGINE_LOG_INFO( "Engine", "Shader [%s] registered", InShaderName.c_str() );
	shaders[ InShaderName ] = InCreateShaderFunction;
}

/**
 * Create shader
 */
le::FBaseShaderRef le::ShaderFactory::Create( const std::string& InShaderName ) const
{
	if ( InShaderName.empty() )		return nullptr;

	auto		it = shaders.find( InShaderName );
	if ( it == shaders.end() )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Shader [%s] not founded", InShaderName.c_str() );
		return nullptr;
	}

	LIFEENGINE_ASSERT( it->second );
	return it->second();
}
