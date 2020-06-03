//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/ifactory.h"
#include "engine/iengine.h"
#include "engine/ishaderparameter.h"

#include "engineapi.h"
#include "errors.h"
#include "material.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Material::Material() :
	material( nullptr )
{
	material = ( le::IMaterial* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( MATERIAL_INTERFACE_VERSION );
	if ( !material )	Error_Critical( "Interface le::IMaterial version[" MATERIAL_INTERFACE_VERSION "] not found in core" );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Material::~Material()
{
	if ( material )
	{
		if ( material->GetCountReferences() <= 1 )		material->Release();
		else											material->DecrementReference();
	}
}

// ------------------------------------------------------------------------------------ //
// Set shader
// ------------------------------------------------------------------------------------ //
void Material::SetShader( const QString& Name )
{
	if ( !material ) return;
	material->SetShader( Name.toLocal8Bit().data() );
}

// ------------------------------------------------------------------------------------ //
// Enabled depth test
// ------------------------------------------------------------------------------------ //
void Material::EnableDepthTest( bool Enable )
{
	if ( !material ) return;
	material->EnableDepthTest( Enable );
}

// ------------------------------------------------------------------------------------ //
// Enabled depth write
// ------------------------------------------------------------------------------------ //
void Material::EnableDepthWrite( bool Enable )
{
	if ( !material ) return;
	material->EnableDepthWrite( Enable );
}

// ------------------------------------------------------------------------------------ //
// Enabled blend
// ------------------------------------------------------------------------------------ //
void Material::EnableBlend( bool Enable )
{
	if ( !material ) return;
	material->EnableBlend( Enable );
}

// ------------------------------------------------------------------------------------ //
// Enabled cullface
// ------------------------------------------------------------------------------------ //
void Material::EnableCullFace( bool Enable )
{
	if ( !material ) return;
	material->EnableCullFace( Enable );
}

// ------------------------------------------------------------------------------------ //
// Set cullface type
// ------------------------------------------------------------------------------------ //
void Material::SetCullFaceType( le::CULLFACE_TYPE CullFaceType )
{
	if ( !material ) return;
	material->SetCullFaceType( CullFaceType );
}

// ------------------------------------------------------------------------------------ //
// Add parameter
// ------------------------------------------------------------------------------------ //
void Material::AddParameter( const QString& Name, le::SHADER_PARAMETER_TYPE Type )
{
	if ( !material ) return;

	ShaderParameterPtr				shaderParameter = std::make_shared< ShaderParameter >();
	shaderParameter->SetName( Name, Type );

	material->AddParameter( shaderParameter->GetHandle() );
	parameters.push_back( shaderParameter );
}

// ------------------------------------------------------------------------------------ //
// Remove parameter
// ------------------------------------------------------------------------------------ //
void Material::RemoveParameter( quint32 Index )
{
	if ( Index >= parameters.size() ) return;

	material->RemoveParameter( Index );
	parameters.erase( parameters.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Has parameter
// ------------------------------------------------------------------------------------ //
bool Material::HasParameter( const QString& Name ) const
{
	for ( quint32 index = 0, count = parameters.size(); index < count; ++index )
		if ( parameters[ index ]->GetName() == Name )
			return true;

	return false;
}