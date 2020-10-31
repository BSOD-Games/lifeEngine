// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "Rendering/RHI/IRHI.h"
#include "Rendering/RHI/IRHIShader.h"
#include "Rendering/ShaderManager.h"

/**
 * Find shader
 */
le::FIRHIShaderRef le::ShaderManager::FindShader( const std::string& InName, const SShaderPaths& InShaderPaths, std::vector< std::string >* InDefines, uint32 InFlags )
{
	// Find shader in cache
	auto		itShaderGroup = shaders.find( InName );
	if ( itShaderGroup != shaders.end() )
	{
		auto		itShader = itShaderGroup->second.find( InFlags );
		if ( itShader != itShaderGroup->second.end() )
			return itShader->second;
	}

	LIFEENGINE_ASSERT( GRHI );

	std::string			error, code;
	FIRHIShaderRef		shader = GRHI->CreateShader();
	
	// Compiling vertex shader
	if ( !InShaderPaths.vertexShaderPath.IsEmpty() )
	{
		FFileHandle			fileVertexShader = GFileSystem->OpenFile( InShaderPaths.vertexShaderPath, true );
		if ( !fileVertexShader )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Failed open vertex shader [%s]", InName.c_str() );
			return nullptr;
		}

		GFileSystem->ReadLineFromFile( fileVertexShader, code, '\0' );
		GFileSystem->CloseFile( fileVertexShader );

		if ( !shader->Compile( code, ST_Vertex, InDefines, &error ) )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Failed compile vertex shader [%s] :: %s", InName.c_str(), error.c_str() );		
			return nullptr;
		}
	}

	// Compiling geometry shader
	if ( !InShaderPaths.geometryShaderPath.IsEmpty() )
	{
		FFileHandle		fileGeometryShader = GFileSystem->OpenFile( InShaderPaths.geometryShaderPath, true );
		if ( !fileGeometryShader )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Failed open geometry shader [%s]", InName.c_str() );
			return nullptr;
		}

		GFileSystem->ReadLineFromFile( fileGeometryShader, code, '\0' );
		GFileSystem->CloseFile( fileGeometryShader );

		if ( !shader->Compile( code, ST_Geometry, InDefines, &error ) )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Failed compile geometry shader [%s] :: %s", InName.c_str(), error.c_str() );
			return nullptr;
		}
	}

	// Compiling pixel shader
	if ( !InShaderPaths.pixelShaderPath.IsEmpty() )
	{
		FFileHandle		filePixelShader = GFileSystem->OpenFile( InShaderPaths.pixelShaderPath, true );
		if ( !filePixelShader )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Failed open pixel shader [%s]", InName.c_str() );
			return nullptr;
		}

		GFileSystem->ReadLineFromFile( filePixelShader, code, '\0' );
		GFileSystem->CloseFile( filePixelShader );

		if ( !shader->Compile( code, ST_Pixel, InDefines, &error ) )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Failed compile pixel shader [%s] :: %s", InName.c_str(), error.c_str() );
			return nullptr;
		}
	}

	// Linking shader
	if ( !shader->Link( &error ) )
	{
		LIFEENGINE_LOG_ERROR( "Engine", "Failed linking shader [%s] :: %s", InName.c_str(), error.c_str() );
		return nullptr;
	}

	shaders[ InName ][ InFlags ] = shader;
	LIFEENGINE_LOG_INFO( "Engine", "Shader loaded [%s] with flags [0x%X]", InName.c_str(), InFlags );
	return shader;
}

/**
 * Unload shader
 */
void le::ShaderManager::UnloadShader( const std::string& InName, uint32 InFlags )
{
	auto		itShaderGroup = shaders.find( InName );
	if ( itShaderGroup == shaders.end() )				return;
		
	auto		itShader = itShaderGroup->second.find( InFlags );
	if ( itShader == itShaderGroup->second.end() )		return;
	
	if ( itShader->second->GetRefCount() >= 2 )		return;

	LIFEENGINE_LOG_INFO( "Engine", "Unloaded shader [%s] with flags [0x%X]", itShaderGroup->first.c_str(), itShader->first );
	itShaderGroup->second.erase( itShader );
}

/**
 * Unload shaders
 */
void le::ShaderManager::UnloadShaders()
{
	for ( auto itShaderGroup = shaders.begin(), itShaderGroupEnd = shaders.end(); itShaderGroup != itShaderGroupEnd; )
	{
		for ( auto itShader = itShaderGroup->second.begin(), itShaderEnd = itShaderGroup->second.end(); itShader != itShaderEnd; )
			if ( itShader->second->GetRefCount() <= 1 )
			{
				LIFEENGINE_LOG_INFO( "Engine", "Unloaded shader [%s] with flags [0x%X]", itShaderGroup->first.c_str(), itShader->first );

				itShader = itShaderGroup->second.erase( itShader );
				itShaderEnd = itShaderGroup->second.end();
			}
			else
				++itShader;

		if ( !itShaderGroup->second.empty() )		++itShaderGroup;
		else
		{
			itShaderGroup = shaders.erase( itShaderGroup );
			itShaderGroupEnd = shaders.end();
		}
	}
}