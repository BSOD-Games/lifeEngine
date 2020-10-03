// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "Misc/EngineGlobals.h"
#include "Math/Math.h"
#include "System/FileSystem.h"
#include "Rendering/ShaderVar.h"
#include "Resources/Texture2D.h"
#include "Resources/ResourceSystem.h"
#include "ParserMaterialLMT.h"

/**
 * Constructor
 */
le::ParserMaterialLMT::ParserMaterialLMT() :
	jsonDocument( nullptr )
{}

/**
 * Destructor
 */
le::ParserMaterialLMT::~ParserMaterialLMT()
{
	Clear();
}

/**
 * Set file
 */
bool le::ParserMaterialLMT::SetFile( FFileHandle InFileHandle )
{
	LIFEENGINE_ASSERT( InFileHandle );
	if ( IsOppened() )		Clear();

	std::string			strBuffer;
	GFileSystem->ReadLineFromFile( InFileHandle, strBuffer, '\0' );

	rapidjson::Document*		jsonDocument = new rapidjson::Document();
	this->jsonDocument = jsonDocument;

	jsonDocument->Parse( strBuffer.c_str() );
	if ( jsonDocument->HasParseError() )		return false;
	return true;
}

/**
 * Get name shader
 */
std::string le::ParserMaterialLMT::GetNameShader() const
{
	rapidjson::Document*		jsonDocument =	static_cast< rapidjson::Document* >( this->jsonDocument );
	for ( auto itRoot = jsonDocument->MemberBegin(), itRootEnd = jsonDocument->MemberEnd(); itRoot != itRootEnd; ++itRoot )
		if ( !strcmp( itRoot->name.GetString(), "Shader" ) && itRoot->value.IsString() )
			return itRoot->value.GetString();

	return "";
}

/**
 * Get shader vars
 */
std::vector< le::ShaderVar > le::ParserMaterialLMT::GetShaderVars() const
{
	if ( !IsOppened() )			return std::vector<ShaderVar>();

	std::vector< ShaderVar >	shaderVars;
	rapidjson::Document*		jsonDocument = static_cast< rapidjson::Document* >( this->jsonDocument );
	
	for ( auto itRoot = jsonDocument->MemberBegin(), itRootEnd = jsonDocument->MemberEnd(); itRoot != itRootEnd; ++itRoot )
		if ( itRoot->value.IsObject() )
		{
			std::string				type;
			rapidjson::Value		jsonValue;			

			// Getting value and type from JSON object
			for ( auto itVar = itRoot->value.GetObject().MemberBegin(), itVarEnd = itRoot->value.GetObject().MemberEnd(); itVar != itVarEnd; ++itVar )
			{
				if ( !strcmp( itVar->name.GetString(), "Type" ) && itVar->value.IsString() )
					type = itVar->value.GetString();
				else if ( !strcmp( itVar->name.GetString(), "Value" ) )
					jsonValue = itVar->value;
			}

			if ( type.empty() || jsonValue.IsNull() )		continue;

			ShaderVar				shaderVar;
			shaderVar.SetName( itRoot->name.GetString() );

			// Int value
			if ( type == "Int" && jsonValue.IsInt() )
				shaderVar.SetValueInt( jsonValue.GetInt() );

			// Float value
			else if ( type == "Float" && jsonValue.IsFloat() )
				shaderVar.SetValueFloat( jsonValue.GetFloat() );

			// Bool value
			else if ( type == "Bool" && jsonValue.IsBool() )
				shaderVar.SetValueBool( jsonValue.GetBool() );

			// Texture2D value
			else if ( type == "Texture2D" && jsonValue.IsString() )
				shaderVar.SetValueTexture2D( Cast< Texture2D >( GResourceSystem->FindResource( jsonValue.GetString(), RT_Texture2D ) ) );

			// Object types
			else if ( jsonValue.IsObject() )
			{
				// Vector2D type
				if ( type == "Vector2D" )
				{
					FVector2D		vector2D;
					for ( auto itObject = jsonValue.GetObject().MemberBegin(), itObjectEnd = jsonValue.GetObject().MemberEnd(); itObject != itObjectEnd; ++itObject )
						if ( itObject->value.IsInt() || itObject->value.IsFloat() )
						{
							if ( !strcmp( itObject->name.GetString(), "X" ) )			vector2D.x = itObject->value.GetFloat();
							else if ( !strcmp( itObject->name.GetString(), "Y" ) )		vector2D.y = itObject->value.GetFloat();
						}

					shaderVar.SetValueVector2D( vector2D );
				}

				// Color type
				else if ( type == "Color" )
				{
					SColor		color;
					for ( auto itObject = jsonValue.GetObject().MemberBegin(), itObjectEnd = jsonValue.GetObject().MemberEnd(); itObject != itObjectEnd; ++itObject )
						if ( itObject->value.IsInt() || itObject->value.IsFloat() )
						{
							if ( !strcmp( itObject->name.GetString(), "R" ) )			color.r = itObject->value.GetInt();
							else if ( !strcmp( itObject->name.GetString(), "G" ) )		color.g = itObject->value.GetInt();
							else if ( !strcmp( itObject->name.GetString(), "B" ) )		color.b = itObject->value.GetInt();
							else if ( !strcmp( itObject->name.GetString(), "A" ) )		color.a = itObject->value.GetInt();
						}

					shaderVar.SetValueColor( color );
				}
			}

			shaderVars.push_back( shaderVar );
		}

	return shaderVars;
}

/**
 * Clear
 */
void le::ParserMaterialLMT::Clear()
{
	if ( !jsonDocument )	return;
	delete static_cast< rapidjson::Document* >( jsonDocument );
	jsonDocument = nullptr;
}
