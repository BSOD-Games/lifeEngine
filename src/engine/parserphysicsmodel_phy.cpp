//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/ifactory.h"
#include "physics/iphysicsmodel.h"

#include "global.h"
#include "phydoc.h"
#include "parserphysicsmodel_phy.h"
#include "consolesystem.h"

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::ParserPhysicsModelPHY::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserPhysicsModelPHY::GetVersion() const
{
	return "2.0";
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserPhysicsModelPHY::GetName() const
{
	return "lifeEngine physics model";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserPhysicsModelPHY::GetCountFileExtensions() const
{
	return 1;
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserPhysicsModelPHY::GetFileExtensions() const
{
	static const char*			extensions[ 1 ] = { "phy" };
	return extensions;
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserPhysicsModelPHY::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::IPhysicsModel* le::ParserPhysicsModelPHY::Read( const char* Path, IFactory* PhysicsSystemFactory )
{
	PHYDoc			phyDoc;
	if ( !phyDoc.Load( Path ) )
	{
		g_consoleSystem->PrintError( "Failed loading physics model" );
		return nullptr;
	}

	auto&					verteces = phyDoc.GetVerteces();
	auto&					vertexIndeces = phyDoc.GetVertexIndeces();
	const PHYVector3D&		inertia = phyDoc.GetInertia();

	// Creating physics models
	IPhysicsModel* physicsModel = ( IPhysicsModel* ) PhysicsSystemFactory->Create( PHYSICSMODEL_INTERFACE_VERSION );
	if ( !physicsModel )
	{
		g_consoleSystem->PrintError( "Interface le::IPhysicsModel [%s] not founded in physics system", PHYSICSMODEL_INTERFACE_VERSION );
		return nullptr;
	}

	physicsModel->Initialize( ( Vector3D_t* ) verteces.data(), verteces.size(), ( UInt32_t* ) vertexIndeces.data(), vertexIndeces.size(), phyDoc.IsStatic() );
	physicsModel->SetMasa( phyDoc.GetMasa() );
	physicsModel->SetInertia( Vector3D_t( inertia.x, inertia.y, inertia.z ) );

	return physicsModel;
}
