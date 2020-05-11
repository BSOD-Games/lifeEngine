//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/lifeengine.h"
#include "engine/ifactory.h"
#include "engine/iconsolesystem.h"
#include "engine/imaterialproxy.h"
#include "studiorender/ishader.h"
#include "studiorender/istudiorender.h"

#include "global.h"
#include "studiorender.h"
#include "shadermanager.h"
#include "material.h"
#include "openglstate.h"

// ------------------------------------------------------------------------------------ //
// Clear material
// ------------------------------------------------------------------------------------ //
void le::Material::Clear()
{
	for ( auto it = parameters.begin(), itEnd = parameters.end(); it != itEnd; ++it )
		if ( ( *it )->GetCountReferences() <= 1 )
			( *it )->Release();
		else
			( *it )->DecrementReference();

	for ( auto it = materialProxes.begin(), itEnd = materialProxes.end(); it != itEnd; ++it )
		if ( ( *it )->GetCountReferences() <= 1 )
			( *it )->Release();
		else
			( *it )->DecrementReference();

	parameters.clear();
	materialProxes.clear();
	shader = nullptr;
	surface = "unknow";
}

// ------------------------------------------------------------------------------------ //
// Enable depth test
// ------------------------------------------------------------------------------------ //
void le::Material::EnableDepthTest( bool Enable )
{
	isDepthTest = Enable;
}

// ------------------------------------------------------------------------------------ //
// Enable depth write
// ------------------------------------------------------------------------------------ //
void le::Material::EnableDepthWrite( bool Enable )
{
	isDepthWrite = Enable;
}

// ------------------------------------------------------------------------------------ //
// Enable blend
// ------------------------------------------------------------------------------------ //
void le::Material::EnableBlend( bool Enable )
{
	isBlend = Enable;
}

// ------------------------------------------------------------------------------------ //
// Enable cull face
// ------------------------------------------------------------------------------------ //
void le::Material::EnableCullFace( bool Enable )
{
	isCullFace = Enable;
}

// ------------------------------------------------------------------------------------ //
// Find parameter
// ------------------------------------------------------------------------------------ //
le::IShaderParameter* le::Material::FindParameter( const char* Name ) const
{
	if ( parameters.empty() )       return nullptr;

	for ( UInt32_t index = 0, count = parameters.size(); index < count; ++index )
		if ( strcmp( Name, parameters[ index ]->GetName() ) == 0 )
			return parameters[ index ];

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Set shader
// ------------------------------------------------------------------------------------ //
void le::Material::SetShader( const char* NameShader )
{
	if ( !NameShader || NameShader == "" )
	{
		shader = nullptr;
		return;
	}

	ShaderManager*			shaderManager = ( ShaderManager* ) g_studioRender->GetShaderManager();
	IShader*				tempShader = shaderManager->FindShader( NameShader );

	if ( tempShader )
	{
		shader = tempShader;
		isNeadRefrash = true;
	}
	else
		g_consoleSystem->PrintError( "Shader [%s] not found in system", NameShader );
}

// ------------------------------------------------------------------------------------ //
// Set cullface type
// ------------------------------------------------------------------------------------ //
void le::Material::SetCullFaceType( CULLFACE_TYPE CullFaceType )
{
	cullFaceType = CullFaceType;
}

// ------------------------------------------------------------------------------------ //
// Set surface name
// ------------------------------------------------------------------------------------ //
void le::Material::SetSurfaceName( const char* Name )
{
	surface = Name;
}

// ------------------------------------------------------------------------------------ //
// Is depth test
// ------------------------------------------------------------------------------------ //
bool le::Material::IsDepthTest() const
{
	return isDepthTest;
}

// ------------------------------------------------------------------------------------ //
// Is depth write
// ------------------------------------------------------------------------------------ //
bool le::Material::IsDepthWrite() const
{
	return isDepthWrite;
}

// ------------------------------------------------------------------------------------ //
// Is blend
// ------------------------------------------------------------------------------------ //
bool le::Material::IsBlend() const
{
	return isBlend;
}

// ------------------------------------------------------------------------------------ //
// Is cullface
// ------------------------------------------------------------------------------------ //
bool le::Material::IsCullFace() const
{
	return isCullFace;
}

// ------------------------------------------------------------------------------------ //
// Get cullface type
// ------------------------------------------------------------------------------------ //
le::CULLFACE_TYPE le::Material::GetCullFaceType() const
{
	return cullFaceType;
}

// ------------------------------------------------------------------------------------ //
// Get shader
// ------------------------------------------------------------------------------------ //
le::IShader* le::Material::GetShader() const
{
	return shader;
}

// ------------------------------------------------------------------------------------ //
// Get count parameters
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Material::GetCountParameters() const
{
	return parameters.size();
}

// ------------------------------------------------------------------------------------ //
// Get parameters
// ------------------------------------------------------------------------------------ //
le::IShaderParameter** le::Material::GetParameters() const
{
	return ( IShaderParameter** ) parameters.data();
}

// ------------------------------------------------------------------------------------ //
// Get parameter
// ------------------------------------------------------------------------------------ //
le::IShaderParameter* le::Material::GetParameter( UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( Index < parameters.size() );
	return ( IShaderParameter* ) parameters[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get count proxes
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Material::GetCountProxes() const
{
	return materialProxes.size();
}

// ------------------------------------------------------------------------------------ //
// Get proxes
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy** le::Material::GetProxes() const
{
	return ( IMaterialProxy** ) materialProxes.data();
}

// ------------------------------------------------------------------------------------ //
// Get proxy
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy* le::Material::GetProxy( UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( Index < materialProxes.size() );
	return ( IMaterialProxy* ) materialProxes[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get surface name
// ------------------------------------------------------------------------------------ //
const char* le::Material::GetSurfaceName() const
{
	return surface.c_str();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Material::Material() :
    surface( "unknow" ),
	isNeadRefrash( true ),
	isDepthTest( true ),
	isDepthWrite( true ),
	isBlend( false ),
	isCullFace( true ),
	cullFaceType( CT_BACK ),
	shader( nullptr ),
    countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Material::~Material()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Apply material
// ------------------------------------------------------------------------------------ //
void le::Material::Apply( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	InitStates();

	if ( shader && ( !isNeadRefrash || Refrash() ) )
	{
		for ( UInt32_t index = 0, count = materialProxes.size(); index < count; ++index )
			materialProxes[ index ]->NeadUpdate();

		shader->OnDrawMesh( parameters.size(), ( IShaderParameter** ) parameters.data(), Transformation, Camera, Lightmap );
	}
}

// ------------------------------------------------------------------------------------ //
// Initialize states
// ------------------------------------------------------------------------------------ //
void le::Material::InitStates()
{
	OpenGLState::EnableDepthTest( isDepthTest );
	OpenGLState::EnableDepthWrite( isDepthWrite );
	OpenGLState::EnableBlend( isBlend );
	OpenGLState::EnableCullFace( isCullFace );
	OpenGLState::SetCullFaceType( cullFaceType );
}

// ------------------------------------------------------------------------------------ //
// Refrash material
// ------------------------------------------------------------------------------------ //
bool le::Material::Refrash()
{
	if ( !isNeadRefrash )		return true;

	try
	{
		if ( !shader )			throw;

		while ( !shader->InitInstance( parameters.size(), ( IShaderParameter** ) parameters.data() ) )
		{
			const char* fallbackShader = shader->GetFallbackShader();
			if ( !fallbackShader || strcmp( fallbackShader, "" ) == 0 )		throw;

			SetShader( fallbackShader );
			if ( !shader )		throw;
		}
	}
	catch ( ... )
	{
		return false;
	}

	isNeadRefrash = false;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Material::IncrementReference()
{
    ++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Material::DecrementReference()
{
    --countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete material
// ------------------------------------------------------------------------------------ //
void le::Material::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Material::GetCountReferences() const
{
    return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Add parameter
// ------------------------------------------------------------------------------------ //
void le::Material::AddParameter( IShaderParameter* Parameter )
{
	LIFEENGINE_ASSERT( Parameter );

	Parameter->IncrementReference();
	parameters.push_back( ( ShaderParameter* ) Parameter );
	parameters.back()->SetMaterial( this );
	isNeadRefrash = true;
}

// ------------------------------------------------------------------------------------ //
// Remove parameter
// ------------------------------------------------------------------------------------ //
void le::Material::RemoveParameter( UInt32_t Index )
{
	LIFEENGINE_ASSERT( Index < parameters.size() );
	ShaderParameter*            parameter = parameters[ Index ];

	if ( parameter->GetCountReferences() <= 1 )
		parameter->Release();
	else
		parameter->DecrementReference();

	parameters.erase( parameters.begin() + Index );
	isNeadRefrash = true;
}

// ------------------------------------------------------------------------------------ //
// Add proxy
// ------------------------------------------------------------------------------------ //
void le::Material::AddProxy( IMaterialProxy* MaterialProxy )
{
	LIFEENGINE_ASSERT( MaterialProxy );

	MaterialProxy->IncrementReference();
	materialProxes.push_back( MaterialProxy );
}

// ------------------------------------------------------------------------------------ //
// Remove proxy
// ------------------------------------------------------------------------------------ //
void le::Material::RemoveProxy( UInt32_t Index )
{
	LIFEENGINE_ASSERT( Index < materialProxes.size() );
	IMaterialProxy*         proxy = materialProxes[ Index ];

	if ( proxy->GetCountReferences() <= 1 )
		proxy->Release();
	else
		proxy->DecrementReference();

	materialProxes.erase( materialProxes.begin() + Index );
}
