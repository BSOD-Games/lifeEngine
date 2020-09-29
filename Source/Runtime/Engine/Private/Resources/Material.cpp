// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "Rendering/ShaderVar.h"
#include "Resources/Parsers/IParserMaterial.h"
#include "Resources/Parsers/ParsersMaterialFactory.h"
#include "Resources/Material.h"

/**
 * Constructor
 */
le::Material::Material() :
	shader( nullptr ),
	isNeadUpdateShader( true )
{}

/**
 * Constructor of copy
 */
le::Material::Material( const Material& InCopy ) :
	shader( nullptr ),
	isNeadUpdateShader( true ),
	Resource( InCopy )
{
	if ( InCopy.shader )	SetShader( InCopy.shader->GetName() );
	for ( uint32 index = 0, count = static_cast< uint32 >( InCopy.vars.size() ); index < count; ++index )
		AddVar( InCopy.vars[ index ] );
}

/**
 * Destructor
 */
le::Material::~Material()
{
	Clear();
}

/**
 * Serialize resource
 */
bool le::Material::Serialize( const Path& InPath )
{
	LIFEENGINE_LOG_WARNING( "Engine", "le::Material::Serialize( const Path& InPath ) :: Not implemented" );
	return false;
}

/**
 * Deserialize resource
 */
bool le::Material::Deserialize( const Path& InPath )
{
	if ( InPath.IsEmpty() )		return false;

	IParserMaterial*		parser = GParsersMaterialFactory->Get( InPath.GetExtension() );
	FFileHandle				fileHandle = GFileSystem->OpenFile( InPath );
	if ( !fileHandle || !parser )
	{
		if ( parser )			parser->ReleaseRef();
		if ( fileHandle )		GFileSystem->CloseFile( fileHandle );
		return false;
	}

	if ( !parser->SetFile( fileHandle ) )		return false;
	
	isNeadUpdateShader = true;
	SetShader( parser->GetNameShader() );
	
	std::vector< ShaderVar >		vars = parser->GetShaderVars();
	for ( uint32 index = 0, count = static_cast< uint32 >( vars.size() ); index < count; ++index )
		AddVar( vars[ index ] );

	parser->ReleaseRef();
	GFileSystem->CloseFile( fileHandle );
	return true;
}

/**
 * Remove value by name
 */
void le::Material::RemoveVar( const std::string& InName )
{
	if ( vars.empty() )		return;
	for ( uint32 index = 0, count = static_cast< uint32 >( vars.size() ); index < count; ++index )
		if ( vars[ index ].GetName() == InName )
		{
			vars[ index ].UnsubscribeMaterial( this );
			vars.erase( index + vars.begin() );
			
			isNeadUpdateShader = true;		
			return;
		}
}

/**
 * Find value
 */
le::ShaderVar* le::Material::FindVar( const std::string& InName ) const
{
	if ( vars.empty() )		return nullptr;
	for ( uint32 index = 0, count = static_cast< uint32 >( vars.size() ); index < count; ++index )
		if ( vars[ index ].GetName() == InName )
			return ( ShaderVar* ) &vars[ index ];

	return nullptr;
}

/**
 * Refresh material
 */
void le::Material::Refresh()
{
	if ( isNeadUpdateShader )			UpdateShader();
}

/**
 * Update shader
 */
bool le::Material::UpdateShader()
{
	if ( !isNeadUpdateShader )		return true;
	while ( shader && !shader->Initialize( &vars ) )
		shader = GShaderFactory->Create( shader->GetFallbackShader() );

	isNeadUpdateShader = false;
	return shader;
}

/**
 * Clear material
 */
void le::Material::Clear()
{
	if ( shader )		shader->ReleaseRef();
	for ( uint32 index = 0, count = static_cast< uint32 >( vars.size() ); index < count; ++index )
		vars[ index ].UnsubscribeMaterial( this );

	shader = nullptr;
	vars.clear();
}

/**
 * Get type resource
 */
le::EResourceType le::Material::GetType() const
{
	return RT_Material;
}