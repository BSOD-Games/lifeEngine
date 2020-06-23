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
	materialProxyVar( nullptr ),
	value( nullptr )
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

	switch ( type )
	{
	default: break;
	}

	value = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Set name
// ------------------------------------------------------------------------------------ //
void MaterialProxyParameter::SetName( const QString& Name, le::MATERIAL_PROXY_VAR_TYPE Type )
{
	if ( !materialProxyVar ) return;
	materialProxyVar->SetName( Name.toLocal8Bit().data() );
	type = Type;
}
