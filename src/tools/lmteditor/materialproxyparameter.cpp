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

#include "engineapi.h"
#include "errors.h"
#include "materialproxyparameter.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
MaterialProxyParameter::MaterialProxyParameter() :
	materialProxyVar( nullptr )
{
	materialProxyVar = ( le::IMaterialProxyVar* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( MATERIALPROXYVAR_INTERFACE_VERSION );
	if ( !materialProxyVar )	Error_Critical( "Interface le::IMaterialProxyVar version[" MATERIALPROXYVAR_INTERFACE_VERSION "] not found in core" );
	materialProxyVar->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
MaterialProxyParameter::~MaterialProxyParameter()
{
	ResetValue();

	if ( materialProxyVar )
	{
		if ( materialProxyVar->GetCountReferences() <= 1 )		materialProxyVar->Release();
		else													materialProxyVar->DecrementReference();
	}
}

// ------------------------------------------------------------------------------------ //
// Reset value
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::ResetValue()
{
	if ( !materialProxyVar ) return;
	materialProxyVar->Clear();
}

// ------------------------------------------------------------------------------------ //
// Set name
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::SetName( const QString& Name, le::MATERIAL_PROXY_VAR_TYPE Type )
{
	if ( !materialProxyVar ) return;
	materialProxyVar->SetName( Name.toLocal8Bit().data() );
	materialProxyVar->SetType( Type );
	type = Type;
}

// ------------------------------------------------------------------------------------ //
// Set value float
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::SetValueFloat( float Value )
{
	if ( !materialProxyVar || type != le::MPVT_FLOAT ) return;
	materialProxyVar->SetValueFloat( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value Vector2D
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::SetValueVector2D( const le::Vector2D_t& Value )
{
	if ( !materialProxyVar || type != le::MPVT_VECTOR_2D ) return;
	materialProxyVar->SetValueVector2D( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value Vector3D
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::SetValueVector3D( const le::Vector3D_t& Value )
{
	if ( !materialProxyVar || type != le::MPVT_VECTOR_3D ) return;
	materialProxyVar->SetValueVector3D( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value Vector4D
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::SetValueVector4D( const le::Vector4D_t& Value )
{
	if ( !materialProxyVar || type != le::MPVT_VECTOR_4D ) return;
	materialProxyVar->SetValueVector4D( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value shader parameter
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::SetValueShaderParameter( le::IShaderParameter* Value )
{
	if ( !materialProxyVar || type != le::MPVT_SHADER_PARAMETER ) return;
	materialProxyVar->SetValueShaderParameter( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value bool
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::SetValueBool( bool Value )
{
	if ( !materialProxyVar || type != le::MPVT_BOOL ) return;
	materialProxyVar->SetValueBool( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value int
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::SetValueInt( int Value )
{
	if ( !materialProxyVar || type != le::MPVT_INT ) return;
	materialProxyVar->SetValueInt( Value );
}

// ------------------------------------------------------------------------------------ //
// Get value float
// ------------------------------------------------------------------------------------ //
float MaterialProxyParameter::GetValueFloat() const
{
	if ( !materialProxyVar )		return 0.f;
	return materialProxyVar->GetValueFloat();
}

// ------------------------------------------------------------------------------------ //
// Get value Vector2D
// ------------------------------------------------------------------------------------ //
le::Vector2D_t MaterialProxyParameter::GetValueVector2D() const
{
	if ( !materialProxyVar )		return le::Vector2D_t( 0.f, 0.f );
	return materialProxyVar->GetValueVector2D();
}

// ------------------------------------------------------------------------------------ //
// Get value Vector3D
// ------------------------------------------------------------------------------------ //
le::Vector3D_t MaterialProxyParameter::GetValueVector3D() const
{
	if ( !materialProxyVar )		return le::Vector3D_t( 0.f, 0.f, 0.f );
	return materialProxyVar->GetValueVector3D();
}

// ------------------------------------------------------------------------------------ //
// Get value Vector4D
// ------------------------------------------------------------------------------------ //
le::Vector4D_t MaterialProxyParameter::GetValueVector4D() const
{
	if ( !materialProxyVar )		return le::Vector4D_t( 0.f, 0.f, 0.f, 0.f );
	return materialProxyVar->GetValueVector4D();
}

// ------------------------------------------------------------------------------------ //
// Get value shader parameter
// ------------------------------------------------------------------------------------ //
le::IShaderParameter* MaterialProxyParameter::GetValueShaderParameter() const
{
	if ( !materialProxyVar )		return nullptr;
	return materialProxyVar->GetValueShaderParameter();
}

// ------------------------------------------------------------------------------------ //
// Get value bool
// ------------------------------------------------------------------------------------ //
bool MaterialProxyParameter::GetValueBool() const
{
	if ( !materialProxyVar )		return false;
	return materialProxyVar->GetValueBool();
}

// ------------------------------------------------------------------------------------ //
// Get value int
// ------------------------------------------------------------------------------------ //
int MaterialProxyParameter::GetValueInt() const
{
	if ( !materialProxyVar )		return 0;
	return materialProxyVar->GetValueInt();
}
