//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/ifactory.h"
#include "engine/resourcesystem.h"
#include "engine/consolesystem.h"
#include "studiorender/igpuprogram.h"

#include "global.h"
#include "baseshader.h"

// ------------------------------------------------------------------------------------ //
// Event: draw text
// ------------------------------------------------------------------------------------ //
void le::BaseShader::OnDrawText( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Glyph )
{
	static bool			isPrintedWarning = false;
	if ( !isPrintedWarning )
	{
		g_consoleSystem->PrintWarning( "BaseShader::OnDrawText not implemented for shader [%s]", GetName() );
		isPrintedWarning = true;
	}
}

// ------------------------------------------------------------------------------------ //
// Event: draw sprite
// ------------------------------------------------------------------------------------ //
void le::BaseShader::OnDrawSprite( const Matrix4x4_t& Transformation, ICamera* Camera )
{
	static bool			isPrintedWarning = false;
	if ( !isPrintedWarning )
	{
		g_consoleSystem->PrintWarning( "BaseShader::OnDrawSprite not implemented for shader [%s]", GetName() );
		isPrintedWarning = true;
	}
}

// ------------------------------------------------------------------------------------ //
// Event: draw static model
// ------------------------------------------------------------------------------------ //
void le::BaseShader::OnDrawStaticModel( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	static bool			isPrintedWarning = false;
	if ( !isPrintedWarning )
	{
		g_consoleSystem->PrintWarning( "BaseShader::OnDrawStaticModel not implemented for shader [%s]", GetName() );
		isPrintedWarning = true;
	}
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::BaseShader::BaseShader() :
	gpuProgram( nullptr ),
	flagShader( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::BaseShader::~BaseShader()
{
	if ( gpuProgram )
	{
		gpuProgram->DecrementReference();
		g_resourceSystem->UnloadGPUProgram( nameShader.c_str(), flagShader );
	}
}

// ------------------------------------------------------------------------------------ //
// Загрузить шейдер
// ------------------------------------------------------------------------------------ //
bool le::BaseShader::LoadShader( const std::string& Name, const std::string& Path, const std::vector< const char* >& Defines, UInt32_t Flags )
{
	gpuProgram = g_resourceSystem->LoadGPUProgram( Name.c_str(), Path.c_str(), Flags, Defines.size(), ( const char** ) Defines.data() );
	
	if ( gpuProgram )
	{
		gpuProgram->IncrementReference();
		flagShader = Flags;
	}

	return gpuProgram;
}