//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "materialsystemfactory.h"
#include "materialvar.h"
#include "material.h"

// ------------------------------------------------------------------------------------ //
// ������� ������
// ------------------------------------------------------------------------------------ //
void* le::MaterialSystemFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, MATERIALVAR_INTERFACE_VERSION ) == 0 )			return new MaterialVar();
	else if ( strcmp( NameInterface, MATERIAL_INTERFACE_VERSION ) == 0 )		return new Material();

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// ������� ������
// ------------------------------------------------------------------------------------ //
void le::MaterialSystemFactory::Delete( const char* NameInterface, void* Object )
{
	if ( !Object ) return;
	delete Object;
}