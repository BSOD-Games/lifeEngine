//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <exception>
#include <SDL2/SDL.h>

#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/iconsolesystem.h"
#include "stdshaders/ishaderdll.h"
#include "stdshaders/ishader.h"

#include "global.h"
#include "materialsystem.h"

LIFEENGINE_MATERIALSYSTEM_API( le::MaterialSystem );

// ------------------------------------------------------------------------------------ //
// ���������������� ������� ����������
// ------------------------------------------------------------------------------------ //
bool le::MaterialSystem::Initialize( IEngine* Engine )
{
	try
	{
		if ( !Engine )					throw std::exception( "Material system requared engine api" );
		g_engine = Engine;

		g_consoleSystem = Engine->GetConsoleSystem();
		if ( !g_consoleSystem )			throw std::exception( "Material system requared console system" );

		g_studioRender = Engine->GetStudioRender();
		if ( !g_studioRender )			throw std::exception( "Material system requared studiorender" );
	}
	catch ( std::exception& Exception )
	{
		if ( g_consoleSystem ) g_consoleSystem->PrintError( Exception.what() );
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// ��������� ���������� ��������
// ------------------------------------------------------------------------------------ //
bool le::MaterialSystem::LoadShaderDLL( const char* FullPath )
{
	LIFEENGINE_ASSERT( FullPath );

	// ���������, �� ��������� �� ��� ���������� ��������
	for ( UInt32_t index, count = shaderLibs.size(); index < count; ++index )
		if ( shaderLibs[ index ].fileName == FullPath )
			return true;

	g_consoleSystem->PrintInfo( "Loading shader dll [%s]", FullPath );

	try
	{
		ShaderDLLDescriptor			shaderDLLDescriptor;

		// ��������� ������
		shaderDLLDescriptor.handle = SDL_LoadObject( FullPath );
		if ( !shaderDLLDescriptor.handle )	throw std::exception( SDL_GetError() );

		// ����� �� ������ API ��� ������ � ���
		shaderDLLDescriptor.LE_CreateShaderDLL = ( LE_CreateShaderDLLFn_t ) SDL_LoadFunction( shaderDLLDescriptor.handle, "LE_CreateShaderDLL" );
		shaderDLLDescriptor.LE_DeleteShaderDLL = ( LE_DeleteShaderDLLFn_t ) SDL_LoadFunction( shaderDLLDescriptor.handle, "LE_DeleteShaderDLL" );
		shaderDLLDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( shaderDLLDescriptor.handle, "LE_SetCriticalError" );
		if ( !shaderDLLDescriptor.LE_CreateShaderDLL )	throw std::exception( "Function LE_CreateShaderDLL not found" );

		// ������� ������
		if ( shaderDLLDescriptor.LE_SetCriticalError )
			shaderDLLDescriptor.LE_SetCriticalError( g_criticalError );

		shaderDLLDescriptor.shaderDLL = ( IShaderDLL* ) shaderDLLDescriptor.LE_CreateShaderDLL();
		if ( !shaderDLLDescriptor.shaderDLL->Initialize( g_engine ) )				throw std::exception( "Fail initialize shader dll" );
	
		shaderDLLDescriptor.fileName = FullPath;

		// ��������� ��� ������� �� ����������
		for ( UInt32_t index = 0, count = shaderDLLDescriptor.shaderDLL->GetShaderCount(); index < count; ++index )
		{
			IShader*		shader = shaderDLLDescriptor.shaderDLL->GetShader( index );
			if ( !shader ) continue;

			shaderDLLDescriptor.shaders.insert( std::make_pair( shader->GetName(), shader ) );
			g_consoleSystem->PrintInfo( "Loaded shader [%s] from [%s]", shader->GetName(), FullPath );
		}

		// ������������ ���������� �������� � �������
		shaderLibs.push_back( shaderDLLDescriptor );
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
		return false;
	}

	g_consoleSystem->PrintInfo( "Loaded shader dll [%s]", FullPath );
	return true;
}

// ------------------------------------------------------------------------------------ //
// ��������� ���������� ��������
// ------------------------------------------------------------------------------------ //
void le::MaterialSystem::UnloadShaderDLL( const char* FullPath )
{
	LIFEENGINE_ASSERT( FullPath );

	for ( UInt32_t index = 0, count = shaderLibs.size(); index < count; ++index )
		if ( shaderLibs[ index ].fileName == FullPath )
		{
			UnloadShaderDLL( shaderLibs[ index ] );
			shaderLibs.erase( shaderLibs.begin() + index );
			break;
		}
}

// ------------------------------------------------------------------------------------ //
// �������� ������� ������� ����������
// ------------------------------------------------------------------------------------ //
le::IFactory* le::MaterialSystem::GetFactory() const
{
	return ( IFactory* ) &materialSystemFactory;
}

// ------------------------------------------------------------------------------------ //
// �����������
// ------------------------------------------------------------------------------------ //
le::MaterialSystem::MaterialSystem()
{
	LIFEENGINE_ASSERT( !g_materialSystem );
	g_materialSystem = this;
}

// ------------------------------------------------------------------------------------ //
// ����������
// ------------------------------------------------------------------------------------ //
le::MaterialSystem::~MaterialSystem()
{
	for ( auto it = shaderLibs.begin(); it != shaderLibs.end(); ++it )
		UnloadShaderDLL( *it );
}

// ------------------------------------------------------------------------------------ //
// ����� ������
// ------------------------------------------------------------------------------------ //
le::IShader* le::MaterialSystem::FindShader( const char* Name ) const
{
	LIFEENGINE_ASSERT( Name );

	for ( UInt32_t index = shaderLibs.size(); --index >= 0; )
	{
		const ShaderDLLDescriptor&			shaderDLLDescriptor = shaderLibs[ index ];
		auto& itShader = shaderDLLDescriptor.shaders.find( Name );

		if ( itShader != shaderDLLDescriptor.shaders.end() )
			return itShader->second;
	}

	// -------
	// � ����� Source Engine <3
	// -------

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// ��������� ���������� ��������
// ------------------------------------------------------------------------------------ //
void le::MaterialSystem::UnloadShaderDLL( const ShaderDLLDescriptor& ShaderDLLDescriptor )
{
	if ( ShaderDLLDescriptor.LE_DeleteShaderDLL )
		ShaderDLLDescriptor.LE_DeleteShaderDLL( ShaderDLLDescriptor.shaderDLL );

	SDL_UnloadObject( ShaderDLLDescriptor.handle );
	g_consoleSystem->PrintInfo( "Unloaded shader dll [%s]", ShaderDLLDescriptor.fileName.c_str() );
}
