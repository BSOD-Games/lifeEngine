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
#include "engine/iconsolesystem.h"
#include "stdshaders/ishader.h"

#include "global.h"
#include "materialsystem.h"
#include "material.h"
#include "materialvar.h"

// ------------------------------------------------------------------------------------ //
// �������� ���������� � ��������
// ------------------------------------------------------------------------------------ //
void le::Material::AddVar( IMaterialVar* MaterialVar )
{
	LIFEENGINE_ASSERT( MaterialVar );
	
	std::string			varName = MaterialVar->GetName();

	for ( UInt32_t index = 0, count = vars.size(); index < count; ++index )
		if ( vars[ index ]->GetName() == varName )
		{
			g_consoleSystem->PrintError( "Material var [%s] already exist in material [%s]", varName.c_str(), name.c_str() );
			return;
		}

	vars.push_back( ( le::MaterialVar* ) MaterialVar );
}

// ------------------------------------------------------------------------------------ //
// ����� ����������
// ------------------------------------------------------------------------------------ //
le::IMaterialVar* le::Material::FindVar( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	for ( UInt32_t index = 0, count = vars.size(); index < count; ++index )
		if ( strcmp( vars[ index ]->GetName(), Name ) == 0 )
			return vars[ index ];

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// �������� ��������
// ------------------------------------------------------------------------------------ //
void le::Material::Clear()
{
	shader = nullptr;
	vars.clear();
}

// ------------------------------------------------------------------------------------ //
// ������ �������� ���������
// ------------------------------------------------------------------------------------ //
void le::Material::SetName( const char* Name )
{
	name = Name;
}

// ------------------------------------------------------------------------------------ //
// ������ ������
// ------------------------------------------------------------------------------------ //
void le::Material::SetShader( const char* Name )
{
	IShader*		tempShader = g_materialSystem->FindShader( Name );
	if ( !tempShader ) return;

	shader = tempShader;
}

// ------------------------------------------------------------------------------------ //
// �������� �������� ���������
// ------------------------------------------------------------------------------------ //
const char* le::Material::GetName() const
{
	return name.c_str();
}

// ------------------------------------------------------------------------------------ //
// �������� �������� �������
// ------------------------------------------------------------------------------------ //
const char* le::Material::GetShaderName() const
{
	return shader ? shader->GetName() : "";
}

// ------------------------------------------------------------------------------------ //
// �������� ���������� ����������
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Material::GetCountVars() const
{
	return vars.size();
}

// ------------------------------------------------------------------------------------ //
// �������� �� ������� ����������
// ------------------------------------------------------------------------------------ //
le::IMaterialVar* le::Material::GetVar( UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( Index >= vars.size() );
	return ( IMaterialVar* ) vars[ Index ];
}

// ------------------------------------------------------------------------------------ //
// �������� ������ ����������
// ------------------------------------------------------------------------------------ //
le::IMaterialVar** le::Material::GetVars() const
{
	return ( IMaterialVar** ) vars.data();
}

// ------------------------------------------------------------------------------------ //
// �����������
// ------------------------------------------------------------------------------------ //
le::Material::Material() :
	name( "unknow" ),
	shader( nullptr )
{}
