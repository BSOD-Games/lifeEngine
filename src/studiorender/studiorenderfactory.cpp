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

#include "studiorenderfactory.h"
#include "gpuprogram.h"

// ------------------------------------------------------------------------------------ //
// ������� ������
// ------------------------------------------------------------------------------------ //
void* le::StudioRenderFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, GPUPROGRAM_INTERFACE_VERSION ) == 0 )			return new GPUProgram();

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// ������� ������
// ------------------------------------------------------------------------------------ //
void le::StudioRenderFactory::Delete( const char* NameInterface, void* Object )
{
	if ( !Object ) return;
	delete Object;
}