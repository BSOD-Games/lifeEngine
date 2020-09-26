// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "Rendering/RHI/IRHIShader.h"
#include "Rendering/ShaderManager.h"
#include "Rendering/RenderSystem.h"

/**
 * Find shader
 */
le::IRHIShader* le::ShaderManager::FindShader( const std::string& InName, const SShaderPaths& InShaderPaths, std::vector< std::string >* InDefines, uint32 InFlags )
{
	// Find shader in cache
	auto		itShaderGroup = shaders.find( InName );
	if ( itShaderGroup != shaders.end() )
	{
		auto		itShader = itShaderGroup->second.find( InFlags );
		if ( itShader != itShaderGroup->second.end() )
			return itShader->second;
	}

	std::string			error, code;
	IRHIShader*			shader = GRenderSystem->CreateShader();
	
	// Compiling vertex shader
	if ( !InShaderPaths.vertexShaderPath.IsEmpty() )
	{
		FFileHandle			fileVertexShader = GFileSystem->OpenFile( InShaderPaths.vertexShaderPath, true );
		GFileSystem->ReadLineFromFile( fileVertexShader, code, '\0' );
		GFileSystem->CloseFile( fileVertexShader );

		if ( !shader->Compile( code, ST_Vertex, InDefines, &error ) )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Failed compile vertex shader [%s] :: %s", InName.c_str(), code.c_str() );
			GRenderSystem->DeleteShader( shader );
			
			return nullptr;
		}
	}

	// Compiling geometry shader
	if ( !InShaderPaths.geometryShaderPath.IsEmpty() )
	{
		FFileHandle		fileGeometryShader = GFileSystem->OpenFile( InShaderPaths.geometryShaderPath, true );
		GFileSystem->ReadLineFromFile( fileGeometryShader, code, '\0' );
		GFileSystem->CloseFile( fileGeometryShader );

		if ( !shader->Compile( code, ST_Geometry, InDefines, &error ) )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Failed compile geometry shader [%s] :: %s", InName.c_str(), code.c_str() );
			GRenderSystem->DeleteShader( shader );

			return nullptr;
		}
	}

	// Compiling pixel shader
	if ( !InShaderPaths.pixelShaderPath.IsEmpty() )
	{
		FFileHandle		filePixelShader = GFileSystem->OpenFile( InShaderPaths.pixelShaderPath, true );
		GFileSystem->ReadLineFromFile( filePixelShader, code, '\0' );
		GFileSystem->CloseFile( filePixelShader );

		if ( !shader->Compile( code, ST_Pixel, InDefines, &error ) )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Failed compile pixel shader [%s] :: %s", InName.c_str(), code.c_str() );
			GRenderSystem->DeleteShader( shader );

			return nullptr;
		}
	}

	// Linking shader
	if ( !shader->Link( &code ) )
	{
		LIFEENGINE_LOG_ERROR( "Engine", "Failed linking shader [%s] :: %s", InName.c_str(), code.c_str() );
		GRenderSystem->DeleteShader( shader );

		return nullptr;
	}

	shaders[ InName ][ InFlags ] = shader;
	shader->AddRef();
	
	LIFEENGINE_LOG_INFO( "Engine", "Shader loaded [%s]", InName.c_str() );
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
	
	LIFEENGINE_LOG_INFO( "Engine", "Unloaded shader [%s] with flags [%X]", itShaderGroup->first.c_str(), itShader->first );
	
	itShader->second->ReleaseRef();
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
				LIFEENGINE_LOG_INFO( "Engine", "Unloaded shader [%s] with flags [%X]", itShaderGroup->first.c_str(), itShader->first );
				itShader->second->ReleaseRef();

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