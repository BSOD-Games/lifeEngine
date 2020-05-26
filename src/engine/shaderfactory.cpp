//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/shaderdescriptor.h"

#include "shaders/lightmappedgeneric.h"
#include "shaders/textgeneric.h"
#include "shaders/spritegeneric.h"
#include "shaders/unlitgeneric.h"

#include "shaderfactory.h"

// ------------------------------------------------------------------------------------ //
// Register shader
// ------------------------------------------------------------------------------------ //
void le::ShaderFactory::Register( const ShaderDescriptor& ShaderDescriptor )
{
	le::ShaderDescriptor		shaderDescriptor;
	shaderDescriptor.countParameters = ShaderDescriptor.countParameters;
	shaderDescriptor.parametersInfo = ShaderDescriptor.parametersInfo;
	shaderDescriptor.CreateShaderFn = ShaderDescriptor.CreateShaderFn;

	shaderDescriptor.name = new char[ strlen( ShaderDescriptor.name ) ];
	strcpy( shaderDescriptor.name, ShaderDescriptor.name );

	shaders.push_back( shaderDescriptor );
	shadersFind[ shaderDescriptor.name ] = shaderDescriptor;
}

// ------------------------------------------------------------------------------------ //
// Unregister shader
// ------------------------------------------------------------------------------------ //
void le::ShaderFactory::Unregister( const char* Name )
{
	auto		it = shadersFind.find( Name );
	if ( it == shadersFind.end() )		return;

	for ( UInt32_t index = 0, count = shaders.size(); index < count; ++index )
		if ( strcmp( shaders[ index ].name, Name ) == 0 )
		{
			delete[] shaders[ index ].name;
			shaders.erase( shaders.begin() + index );
			break;
		}

	shadersFind.erase( it );
}

// ------------------------------------------------------------------------------------ //
// Create shader
// ------------------------------------------------------------------------------------ //
le::IShader* le::ShaderFactory::Create( const char* Name )
{
	auto		it = shadersFind.find( Name );
	if ( it == shadersFind.end() )		return nullptr;
	return it->second.CreateShaderFn();
}

// ------------------------------------------------------------------------------------ //
// Get count shaders
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ShaderFactory::GetCountShaders() const
{
	return shaders.size();
}

// ------------------------------------------------------------------------------------ //
// GetShader
// ------------------------------------------------------------------------------------ //
le::ShaderDescriptor le::ShaderFactory::GetShader( UInt32_t Index ) const
{
	if ( Index >= shaders.size() )		return ShaderDescriptor();
	return shaders[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Register material proxy
// ------------------------------------------------------------------------------------ //
le::ShaderDescriptor* le::ShaderFactory::GetShaders() const
{
	return ( ShaderDescriptor* ) shaders.data();
}

// ------------------------------------------------------------------------------------ //
// Register material proxy
// ------------------------------------------------------------------------------------ //
le::ShaderFactory::ShaderFactory()
{
	Register( LightmappedGeneric::GetDescriptor() );
	Register( TextGeneric::GetDescriptor() );
	Register( SpriteGeneric::GetDescriptor() );
	Register( UnlitGeneric::GetDescriptor() );
}

// ------------------------------------------------------------------------------------ //
// Register material proxy
// ------------------------------------------------------------------------------------ //
le::ShaderFactory::~ShaderFactory()
{
	for ( UInt32_t index = 0, count = shaders.size(); index < count; ++index )
		delete[] shaders[ index ].name;
}
