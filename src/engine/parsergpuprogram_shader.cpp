//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <string>
#include <fstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "engine/ifactory.h"
#include "studiorender/igpuprogram.h"

#include "global.h"
#include "consolesystem.h"
#include "parsergpuprogram_shader.h"

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::ParserGPUProgramShader::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserGPUProgramShader::GetVersion() const
{
	return "1.0";
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserGPUProgramShader::GetName() const
{
	return "Shader";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserGPUProgramShader::GetCountFileExtensions() const
{
	return 1;
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserGPUProgramShader::GetFileExtensions() const
{
	static const char*			extensions[ 1 ] = { "shader" };
	return extensions;
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserGPUProgramShader::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::IGPUProgram* le::ParserGPUProgramShader::Read( const char* Directory, const char* Path, UInt32_t CountDefines, const char** Defines, IFactory* StudioRenderFactory )
{
	std::ifstream				file( Path );
	if ( !file.is_open() )
	{
		g_consoleSystem->PrintError( "Failed open file" );
		return nullptr;
	}

	std::string					stringBuffer;
	std::getline( file, stringBuffer, '\0' );

	rapidjson::Document			document;
	document.Parse( stringBuffer.c_str() );
	if ( document.HasParseError() )
	{
		g_consoleSystem->PrintError( "File has parse errors" );
		return nullptr;
	}

	std::string				vertexShader;
	std::string				pixelShader;

	// Reading all path to shaders
	for ( auto itRoot = document.MemberBegin(), itRootEnd = document.MemberEnd(); itRoot != itRootEnd; ++itRoot )
	{
		// Vertex shader
		if ( strcmp( itRoot->name.GetString(), "vertex" ) == 0 && itRoot->value.IsString() )
			vertexShader = itRoot->value.GetString();

		// Pixel shader
		else if ( strcmp( itRoot->name.GetString(), "pixel" ) == 0 && itRoot->value.IsString() )
			pixelShader = itRoot->value.GetString();
	}

	// Loading vertex shader
	file.close();
	file.open( std::string( Directory ) + "/" + vertexShader );
	if ( !file.is_open() )
	{
		g_consoleSystem->PrintError( "Failed open vertex shader [%s]", ( std::string( Directory ) + "/" + vertexShader ).c_str() );
		return nullptr;
	}

	vertexShader.clear();
	std::getline( file, vertexShader, '\0' );

	// Loading pixel shader
	file.close();
	file.open( std::string( Directory ) + "/" + pixelShader );
	if ( !file.is_open() )
	{
		g_consoleSystem->PrintError( "Failed open pixel shader [%s]", ( std::string( Directory ) + "/" + pixelShader ).c_str() );
		return nullptr;
	}

	pixelShader.clear();
	std::getline( file, pixelShader, '\0' );

	// Creating gpu program
	le::IGPUProgram* gpuProgram = ( le::IGPUProgram* ) StudioRenderFactory->Create( GPUPROGRAM_INTERFACE_VERSION );
	if ( !gpuProgram )
	{
		g_consoleSystem->PrintError( "Interface le::IGPUProgram [%s] not founded in studiorender", GPUPROGRAM_INTERFACE_VERSION );
		return nullptr;
	}

	le::GPUProgramDescriptor		gpuProgramDescriptor;
	gpuProgramDescriptor.vertexShaderSource = vertexShader.c_str();
	gpuProgramDescriptor.fragmentShaderSource = pixelShader.c_str();
	gpuProgramDescriptor.geometryShaderSource = nullptr;

	if ( !gpuProgram->Compile( gpuProgramDescriptor, CountDefines, Defines ) )
	{
		g_consoleSystem->PrintError( "Shader not compiled" );
		return nullptr;
	}

	return gpuProgram;
}
