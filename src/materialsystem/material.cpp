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
#include "engine/iconsolesystem.h"
#include "common/shaderparaminfo.h"
#include "stdshaders/ishader.h"

#include "global.h"
#include "materialsystem.h"
#include "material.h"
#include "materialvar.h"

// ------------------------------------------------------------------------------------ //
// Приготовить материал к отрисовке геометрии
// ------------------------------------------------------------------------------------ //
void le::Material::OnDrawMesh( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	// TODO: Подправить систему материалов, ибо костыльно приминяется
	// лайтмап, а в будущем будет и shadowmaps

	if ( shader && ( !isNeadRefrash || Refrash() ) )
		shader->OnDrawMesh( vars.size(), ( IMaterialVar** ) vars.data(), Transformation, Camera, Lightmap );
}

// ------------------------------------------------------------------------------------ //
// Найти переменную
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
// Очистить материал
// ------------------------------------------------------------------------------------ //
void le::Material::Clear()
{
	shader = nullptr;
	surface = "unknow";

	for ( UInt32_t index = 0, count = vars.size(); index < count; ++index )
		delete vars[ index ];

	vars.clear();
}

// ------------------------------------------------------------------------------------ //
// Задать шейдер
// ------------------------------------------------------------------------------------ //
void le::Material::SetShader( const char* Name )
{	
	IShader*				tempShader = g_materialSystem->FindShader( Name );
	if ( !tempShader )		return;

	ShaderParamInfo*		shaderParams = tempShader->GetParams();
	if ( !shaderParams )	return;

	for ( UInt32_t index = 0, count = vars.size(); index < count; ++index )
		delete vars[ index ];

	vars.clear();

	for ( UInt32_t index = 0, count = tempShader->GetCountParams(); index < count; ++index )
	{
		ShaderParamInfo&	shaderParam = shaderParams[ index ];	
		MaterialVar*		materialVar = new MaterialVar( this );

		materialVar->SetName( shaderParam.name );
		vars.push_back( materialVar );
	}
	
	shader = tempShader;
}

// ------------------------------------------------------------------------------------ //
// Задать название поверхности
// ------------------------------------------------------------------------------------ //
void le::Material::SetSurfaceName( const char* Name )
{
	surface = Name;
}

// ------------------------------------------------------------------------------------ //
// Получить название шейдера
// ------------------------------------------------------------------------------------ //
const char* le::Material::GetShaderName() const
{
	return shader ? shader->GetName() : "";
}

// ------------------------------------------------------------------------------------ //
// Получить название поверхности
// ------------------------------------------------------------------------------------ //
const char* le::Material::GetSurfaceName() const
{
	return surface.c_str();
}

// ------------------------------------------------------------------------------------ //
// Получить количество переменных
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Material::GetCountVars() const
{
	return vars.size();
}

// ------------------------------------------------------------------------------------ //
// Получить по индексу переменную
// ------------------------------------------------------------------------------------ //
le::IMaterialVar* le::Material::GetVar( UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( Index >= vars.size() );
	return ( IMaterialVar* ) vars[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Получить массив переменных
// ------------------------------------------------------------------------------------ //
le::IMaterialVar** le::Material::GetVars() const
{
	return ( IMaterialVar** ) vars.data();
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Material::Material() :
	surface( "unknow" ),
	shader( nullptr ),
	isNeadRefrash( false )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Material::~Material()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Обновить материал
// ------------------------------------------------------------------------------------ //
bool le::Material::Refrash()
{
	try
	{
		if ( !shader )			throw;

		while ( !shader->InitInstance( vars.size(), ( IMaterialVar** ) vars.data() ) )
		{
			const char*			fallbackShader = shader->GetFallbackShader();
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
// Поставить метку на обновление материала
// ------------------------------------------------------------------------------------ //
void le::Material::SetNeadRefrash()
{
	isNeadRefrash = true;
}
