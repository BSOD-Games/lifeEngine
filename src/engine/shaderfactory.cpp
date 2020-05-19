//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "shaderfactory.h"
#include "lightmappedgeneric.h"
#include "textgeneric.h"
#include "spritegeneric.h"
#include "unlitgeneric.h"

// ------------------------------------------------------------------------------------ //
// Register shader
// ------------------------------------------------------------------------------------ //
void le::ShaderFactory::Register( const char* Name, CreateShaderFn_t CreateShader )
{
	char*			name = new char[ strlen( Name ) ];
	strcpy( name, Name );

	nameShaders.push_back( name );
	shaders[ Name ] = CreateShader;
}

// ------------------------------------------------------------------------------------ //
// Unregister shader
// ------------------------------------------------------------------------------------ //
void le::ShaderFactory::Unregister( const char* Name )
{
	auto		it = shaders.find( Name );
	if ( it == shaders.end() )		return;

	for ( UInt32_t index = 0, count = nameShaders.size(); index < count; ++index )
		if ( strcmp( nameShaders[ index ], Name ) == 0 )
		{
			delete[] nameShaders[ index ];
			nameShaders.erase( nameShaders.begin() + index );
			break;
		}

	shaders.erase( it );
}

// ------------------------------------------------------------------------------------ //
// Create shader
// ------------------------------------------------------------------------------------ //
le::IShader* le::ShaderFactory::Create( const char* Name )
{
	auto		it = shaders.find( Name );
	if ( it == shaders.end() )		return nullptr;
	return it->second();
}

// ------------------------------------------------------------------------------------ //
// Get count shaders
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ShaderFactory::GetCountShaders() const
{
	return nameShaders.size();
}

// ------------------------------------------------------------------------------------ //
// GetShader
// ------------------------------------------------------------------------------------ //
const char* le::ShaderFactory::GetShader( UInt32_t Index ) const
{
	if ( Index >= nameShaders.size() )		return "unknown";
	return nameShaders[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Register material proxy
// ------------------------------------------------------------------------------------ //
const char** le::ShaderFactory::GetShaders() const
{
	return ( const char** ) nameShaders.data();
}

// ------------------------------------------------------------------------------------ //
// Register material proxy
// ------------------------------------------------------------------------------------ //
le::ShaderFactory::ShaderFactory()
{
	Register( "LightmappedGeneric", []() -> IShader* { return new LightmappedGeneric(); } );
	Register( "TextGeneric", []() -> IShader* { return new TextGeneric(); } );
	Register( "SpriteGeneric", []() -> IShader* { return new SpriteGeneric(); } );
	Register( "UnlitGeneric", []() -> IShader* { return new UnlitGeneric(); } );
}

// ------------------------------------------------------------------------------------ //
// Register material proxy
// ------------------------------------------------------------------------------------ //
le::ShaderFactory::~ShaderFactory()
{
	for ( UInt32_t index = 0, count = nameShaders.size(); index < count; ++index )
		delete[] nameShaders[ index ];
}
