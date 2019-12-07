//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/iconsolesystem.h"

#include "shaderdll.h"
#include "testshader.h"

LIFEENGINE_STDSHADERS_API( le::ShaderDLL );

// ------------------------------------------------------------------------------------ //
// ���������������� ���������� ��������
// ------------------------------------------------------------------------------------ //
bool le::ShaderDLL::Initialize( IEngine* Engine )
{
	shaders.push_back( new TestShader() );

	return true;
}

// ------------------------------------------------------------------------------------ //
// �������� ���������� ��������
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ShaderDLL::GetShaderCount() const
{
	return shaders.size();
}

// ------------------------------------------------------------------------------------ //
// �������� ������
// ------------------------------------------------------------------------------------ //
le::IShader* le::ShaderDLL::GetShader( UInt32_t Index ) const
{
	if ( Index >= shaders.size() ) return nullptr;
	return shaders[ Index ];
}

// ------------------------------------------------------------------------------------ //
// ����������
// ------------------------------------------------------------------------------------ //
le::ShaderDLL::~ShaderDLL()
{
	for ( UInt32_t index = 0, count = shaders.size(); index < count; ++index )
		delete shaders[ index ];

	shaders.clear();
}