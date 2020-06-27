//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/imaterialproxyfactory.h"

#include "engineapi.h"
#include "materialproxy.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
MaterialProxy::MaterialProxy() :
	materialProxy( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
MaterialProxy::~MaterialProxy()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Create proxy
// ------------------------------------------------------------------------------------ //
bool MaterialProxy::Create( const QString& Name )
{
	materialProxy = EngineAPI::GetInstance()->GetMaterialSystem()->GetMaterialProxyFactory()->Create( Name.toStdString().c_str() );
	if ( !materialProxy )			return false;

	materialProxy->IncrementReference();
	name = Name;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Delete proxy
// ------------------------------------------------------------------------------------ //
void MaterialProxy::Delete()
{
	if ( !materialProxy ) return;

	if ( materialProxy->GetCountReferences() <= 1 )		materialProxy->Release();
	else												materialProxy->DecrementReference();

	name = "";
	parameters.clear();
}

// ------------------------------------------------------------------------------------ //
// Add parameter
// ------------------------------------------------------------------------------------ //
void MaterialProxy::AddParameter( const QString& Name, le::MATERIAL_PROXY_VAR_TYPE Type )
{
	if ( !materialProxy ) return;

	MaterialProxyParameterPtr			proxyParameter = std::make_shared< MaterialProxyParameter >();
	proxyParameter->SetName( Name, Type );

	materialProxy->SetVar( proxyParameter->GetHandle() );
	parameters.push_back( proxyParameter );
}

// ------------------------------------------------------------------------------------ //
// Remove parameter
// ------------------------------------------------------------------------------------ //
void MaterialProxy::RemoveParameter( quint32 Index )
{
	if ( !materialProxy || Index >= parameters.size() ) return;

	// TODO: Add set shader parameter to default value
	materialProxy->ClearVar( parameters[ Index ]->GetName().toStdString().c_str() );
	parameters.erase( parameters.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Has parameter
// ------------------------------------------------------------------------------------ //
bool MaterialProxy::HasParameter( const QString& Name ) const
{
	for ( quint32 index = 0, count = parameters.size(); index < count; ++index )
		if ( parameters[ index ]->GetName() == Name )
			return true;

	return false;
}
