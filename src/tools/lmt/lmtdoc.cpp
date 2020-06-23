//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "lmtdoc.h"

#define LMT_VERSION		2

// ------------------------------------------------------------------------------------ //
// Cullface type: string to enum
// ------------------------------------------------------------------------------------ //
inline LMTDoc::CULLFACE_TYPE CullFaceType_StringToEnum( const char* Type )
{
	if ( !Type || Type == "" )				return LMTDoc::CT_BACK;

	if ( strcmp( Type, "front" ) == 0 )		return LMTDoc::CT_FRONT;
	else									return LMTDoc::CT_BACK;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
LMTDoc::LMTDoc() :
	isEnabledDepthTest( false ),
	isEnabledDepthWrite( false ),
	isEnabledBlend( false ),
	isEnabledCullFace( false ),
	cullfaceType( CT_BACK )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
LMTDoc::~LMTDoc()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Load
// ------------------------------------------------------------------------------------ //
bool LMTDoc::Load( const std::string& Path )
{
	// TODO: Refact this fucking shit code!

	std::ifstream		file( Path );
	if ( !file.is_open() )						return false;

	std::string					stringBuffer;
	std::getline( file, stringBuffer, '\0' );

	rapidjson::Document			document;
	document.Parse( stringBuffer.c_str() );
	if ( document.HasParseError() )				return false;

	if ( document.FindMember( "version" ) == document.MemberEnd() ||
		 !document[ "version" ].IsNumber() || document[ "version" ].GetInt() != LMT_VERSION )
		return false;

	// Reading all parameters material
	for ( auto itRoot = document.MemberBegin(), itRootEnd = document.MemberEnd(); itRoot != itRootEnd; ++itRoot )
	{
		// Surface name
		if ( strcmp( itRoot->name.GetString(), "surface" ) == 0 && itRoot->value.IsString() )
			surface = itRoot->value.GetString();

		// Is enabled depth test
		else if ( strcmp( itRoot->name.GetString(), "depthTest" ) == 0 && itRoot->value.IsBool() )
			isEnabledDepthTest = itRoot->value.GetBool();

		// Is enabled depth write
		else if ( strcmp( itRoot->name.GetString(), "depthWrite" ) == 0 && itRoot->value.IsBool() )
			isEnabledDepthWrite = itRoot->value.GetBool();

		// Is enabled blend
		else if ( strcmp( itRoot->name.GetString(), "blend" ) == 0 && itRoot->value.IsBool() )
			isEnabledBlend = itRoot->value.GetBool();

		// Is enabled cullface
		else if ( strcmp( itRoot->name.GetString(), "cullface" ) == 0 && itRoot->value.IsBool() )
			isEnabledCullFace = itRoot->value.GetBool();

		// Cullface type
		else if ( strcmp( itRoot->name.GetString(), "cullface_type" ) == 0 )
		{
			if ( itRoot->value.IsString() )
				cullfaceType = CullFaceType_StringToEnum( itRoot->value.GetString() );
			else if ( itRoot->value.IsNumber() )
				cullfaceType = ( CULLFACE_TYPE ) itRoot->value.GetInt();
		}

		// Shader
		else if ( strcmp( itRoot->name.GetString(), "shader" ) == 0 && itRoot->value.IsString() )
			shader = itRoot->value.GetString();

		// Shader parameters
		else if ( strcmp( itRoot->name.GetString(), "parameters" ) == 0 && itRoot->value.IsObject() )
			for ( auto itParameter = itRoot->value.GetObject().MemberBegin(), itParameterEnd = itRoot->value.GetObject().MemberEnd(); itParameter != itParameterEnd; ++itParameter )
			{
				if ( !itParameter->name.IsString() )		continue;

				LMTParameter			parameter;
				parameter.SetName( itParameter->name.GetString() );

				if ( itParameter->value.IsString() )
					parameter.SetValueTexture( itParameter->value.GetString() );
				else if ( itParameter->value.IsObject() )
				{
					rapidjson::Value::Object            object = itParameter->value.GetObject();
					if ( !object.HasMember( "type" ) )	continue;

					std::string			type = object[ "type" ].GetString();
					if ( type.empty() )		continue;

					if ( type == "vector2d" && object.HasMember( "x" ) && object.HasMember( "y" ) )
					{
						parameter.SetValueVector2D( LMTVector2D( object[ "x" ].GetFloat(), object[ "y" ].GetFloat() ) );
					}
					else if ( type == "vector3d" && object.HasMember( "x" ) && object.HasMember( "y" ) && object.HasMember( "z" ) )
					{
						parameter.SetValueVector3D( LMTVector3D( object[ "x" ].GetFloat(), object[ "y" ].GetFloat(), object[ "z" ].GetFloat() ) );
					}
					else if ( type == "vector4d" && object.HasMember( "x" ) && object.HasMember( "y" ) && object.HasMember( "z" ) && object.HasMember( "w" ) )
					{
						parameter.SetValueVector4D( LMTVector4D( object[ "x" ].GetFloat(), object[ "y" ].GetFloat(), object[ "z" ].GetFloat(), object[ "w" ].GetFloat() ) );
					}
					else if ( type == "color" && object.HasMember( "r" ) && object.HasMember( "g" ) && object.HasMember( "b" ) && object.HasMember( "a" ) )
					{
						parameter.SetValueColor( LMTColor( object[ "r" ].GetFloat(), object[ "g" ].GetFloat(), object[ "b" ].GetFloat(), object[ "a" ].GetFloat() ) );
					}
					else continue;
				}
				else if ( itParameter->value.IsBool() )		parameter.SetValueBool( itParameter->value.GetBool() );
				else if ( itParameter->value.IsDouble() )	parameter.SetValueFloat( itParameter->value.GetDouble() );
				else if ( itParameter->value.IsFloat() )	parameter.SetValueFloat( itParameter->value.GetFloat() );
				else if ( itParameter->value.IsInt() )		parameter.SetValueInt( itParameter->value.GetInt() );

				parameters.push_back( parameter );
			}

		// Proxies
		else if ( strcmp( itRoot->name.GetString(), "proxies" ) == 0 && itRoot->value.IsObject() )
			for ( auto itProxy = itRoot->value.GetObject().MemberBegin(), itProxyEnd = itRoot->value.GetObject().MemberEnd(); itProxy != itProxyEnd; ++itProxy )
			{
				if ( !itProxy->name.IsString() || !itProxy->value.IsObject() ) continue;
				LMTProxy           proxy;

				proxy.SetName( itProxy->name.GetString() );
				for ( auto itProxyValue = itProxy->value.GetObject().MemberBegin(), itProxyValueEnd = itProxy->value.GetObject().MemberEnd(); itProxyValue != itProxyValueEnd; ++itProxyValue )
				{
					if ( !itProxyValue->name.IsString() )		continue;

					LMTProxyParameter			proxyParameter;
					proxyParameter.SetName( itProxyValue->name.GetString() );

					if ( itProxyValue->value.IsArray() )
					{
						rapidjson::Value::Array         array = itProxyValue->value.GetArray();

						if ( array.Begin()->IsInt() )
						{
							std::vector< int >          stdArray;
							for ( std::uint32_t index = 0, count = array.Size(); index < count; ++index )
								stdArray.push_back( array[ index ].GetInt() );

							proxyParameter.SetValueArrayInt( stdArray );
						}
						else if ( array.Begin()->IsFloat() )
						{
							std::vector< float >          stdArray;
							for ( std::uint32_t index = 0, count = array.Size(); index < count; ++index )
								stdArray.push_back( array[ index ].GetInt() );

							proxyParameter.SetValueArrayFloat( stdArray );
						}
						else if ( array.Begin()->IsObject() )
						{
							rapidjson::Value::Object            object = array.Begin()->GetObject();
							bool                                hasX = object.HasMember( "x" );
							bool                                hasY = object.HasMember( "y" );
							bool                                hasZ = object.HasMember( "z" );
							bool                                hasW = object.HasMember( "w" );

							if ( hasX && hasY && !hasZ && !hasW )
							{
								std::vector< LMTVector2D >          stdArray;
								for ( std::uint32_t index = 0, count = array.Size(); index < count; ++index )
								{
									object = array[ index ].GetObject();
									stdArray.push_back( LMTVector2D( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat() ) );
								}

								proxyParameter.SetValueArrayVector2D( stdArray );
							}
							else if ( hasX && hasY && hasZ && !hasW )
							{
								std::vector< LMTVector3D >          stdArray;
								for ( std::uint32_t index = 0, count = array.Size(); index < count; ++index )
								{
									object = array[ index ].GetObject();
									stdArray.push_back( LMTVector3D( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat() ) );
								}

								proxyParameter.SetValueArrayVector3D( stdArray );
							}
							else if ( hasX && hasY && hasZ && hasW )
							{
								std::vector< LMTVector4D >          stdArray;
								for ( std::uint32_t index = 0, count = array.Size(); index < count; ++index )
								{
									object = array[ index ].GetObject();
									stdArray.push_back( LMTVector4D( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat(), object.FindMember( "w" )->value.GetFloat() ) );
								}

								proxyParameter.SetValueArrayVector4D( stdArray );
							}
						}
					}
					else if ( itProxyValue->value.IsString() )
						proxyParameter.SetValueShaderParameter( itProxyValue->value.GetString() );
					else if ( itProxyValue->value.IsObject() )
					{
						rapidjson::Value::Object            object = itProxyValue->value.GetObject();
						bool                                hasX = object.HasMember( "x" );
						bool                                hasY = object.HasMember( "y" );
						bool                                hasZ = object.HasMember( "z" );
						bool                                hasW = object.HasMember( "w" );

						if ( hasX && hasY && !hasZ && !hasW )       proxyParameter.SetValueVector2D( LMTVector2D( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat() ) );
						else if ( hasX && hasY && hasZ && !hasW )   proxyParameter.SetValueVector3D( LMTVector3D( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat() ) );
						else if ( hasX && hasY && hasZ && hasW )    proxyParameter.SetValueVector4D( LMTVector4D( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat(), object.FindMember( "w" )->value.GetFloat() ) );
					}
					else if ( itProxyValue->value.IsBool() )          proxyParameter.SetValueBool( itProxyValue->value.GetBool() );
					else if ( itProxyValue->value.IsDouble() )        proxyParameter.SetValueFloat( itProxyValue->value.GetFloat() );
					else if ( itProxyValue->value.IsFloat() )         proxyParameter.SetValueFloat( itProxyValue->value.GetFloat() );
					else if ( itProxyValue->value.IsInt() )           proxyParameter.SetValueInt( itProxyValue->value.GetInt() );

					proxy.AddParameter( proxyParameter );
				}

				proxes.push_back( proxy );
			}
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Save
// ------------------------------------------------------------------------------------ //
bool LMTDoc::Save( const std::string& Path )
{
	std::ofstream			file( Path );
	if ( !file.is_open() )		return false;

	file << "{\n";

	// Write general settings
	file << "\t\"version\": " << LMT_VERSION << ",\n";
	file << "\t\"surface\": " << "\"" << surface << "\",\n\n";
	file << "\t\"shader\": " << "\"" << shader << "\",\n";
	file << "\t\"depthTest\": " << ( isEnabledDepthTest ? "true" : "false" ) << ",\n";
	file << "\t\"depthWrite\": " << ( isEnabledDepthWrite ? "true" : "false" ) << ",\n";
	file << "\t\"blend\": " << ( isEnabledBlend ? "true" : "false" ) << ",\n";
	file << "\t\"cullface\": " << ( isEnabledCullFace ? "true" : "false" ) << ",\n";
	file << "\t\"cullface_type\": ";

	// Write cullface type
	switch ( cullfaceType )
	{
	case CT_BACK:	file << "\"back\"";		break;
	case CT_FRONT:	file << "\"front\"";	break;
	default:		file << "\"\"";			break;
	}

	// Write parameters
	if ( !parameters.empty() )
	{
		file << ",\n\n";
		file << "\t\"parameters\":\n";
		file << "\t{\n";

		for ( std::uint32_t index = 0, count = parameters.size(); index < count; ++index )
		{
			LMTParameter& parameter = parameters[ index ];
			if ( !parameter.IsDefined() )	continue;

			file << "\t\t\"" << parameter.GetName() << "\": ";

			LMTParameter::PARAMETER_TYPE		type = parameter.GetType();
			switch ( type )
			{
			case LMTParameter::PT_INT:		file << parameter.GetValueInt();					break;
			case LMTParameter::PT_FLOAT:	file << parameter.GetValueFloat();					break;
			case LMTParameter::PT_TEXTURE:	file << "\"" << parameter.GetValueTexture() << "\""; break;

			case LMTParameter::PT_BOOL:
			{
				bool		value = parameter.GetValueBool();
				file << ( value ? "true" : "false" );
				break;
			}
			case LMTParameter::PT_COLOR:
			{
				const LMTColor&			color = parameter.GetValueColor();
				file << "\n\t\t{\n";
				file << "\t\t\t\"type\": \"color\",\n";
				file << "\t\t\t\"r\": " << color.r << ",\n";
				file << "\t\t\t\"g\": " << color.g << ",\n";
				file << "\t\t\t\"b\": " << color.b << ",\n";
				file << "\t\t\t\"a\": " << color.a << "\n";
				file << "\t\t}";
				break;
			}
			case LMTParameter::PT_VECTOR_2D:
			{
				const LMTVector2D&		value = parameter.GetValueVector2D();
				file << "\n\t\t{\n";
				file << "\t\t\t\"type\": \"vector2d\",\n";
				file << "\t\t\t\"x\": " << value.x << ",\n";
				file << "\t\t\t\"y\": " << value.y << "\n";
				file << "\t\t}";
				break;
			}
			case LMTParameter::PT_VECTOR_3D:
			{
				const LMTVector4D&		value = parameter.GetValueVector4D();
				file << "\n\t\t{\n";
				file << "\t\t\t\"type\": \"vector3d\",\n";
				file << "\t\t\t\"x\": " << value.x << ",\n";
				file << "\t\t\t\"y\": " << value.y << ",\n";
				file << "\t\t\t\"z\": " << value.z << "\n";
				file << "\t\t}";
				break;
			}
			case LMTParameter::PT_VECTOR_4D:
			{
				const LMTVector4D&		value = parameter.GetValueVector4D();
				file << "\n\t\t{\n";
				file << "\t\t\t\"type\": \"vector4d\",\n";
				file << "\t\t\t\"x\": " << value.x << ",\n";
				file << "\t\t\t\"y\": " << value.y << ",\n";
				file << "\t\t\t\"z\": " << value.z << "\n";
				file << "\t\t\t\"w\": " << value.w << "\n";
				file << "\t\t}";
				break;
			}
			}

			if ( index + 1 == count )
				file << "\n";
			else
				file << ",\n";
		}

		file << "\t}";
	}

	// Write proxes
	if ( !proxes.empty() )
	{
		file << ",\n\n";
		file << "\t\"proxies\":\n";
		file << "\t{\n";

		for ( std::uint32_t index = 0, count = proxes.size(); index < count; ++index )
		{
			LMTProxy&		proxy = proxes[ index ];
			file << "\t\t\"" << proxy.GetName() << "\":\n";
			file << "\t\t{";

			auto&			parameters = proxy.GetParameters();
			for ( std::uint32_t indexParameter = 0, countParameters = parameters.size(); indexParameter < countParameters; ++indexParameter )
			{
				const LMTProxyParameter&		proxyParameter = parameters[ indexParameter ];
				file << "\t\t\t\"" << proxyParameter.GetName() << "\": ";

				switch ( proxyParameter.GetType() )
				{
				case LMTProxyParameter::PT_ARRAY_FLOAT:
				{
					auto&		arrayFloat = proxyParameter.GetValueArrayFloat();
					file << "\t\t\t[\n";
					
					for ( std::uint32_t arrayIndex = 0, countArray = arrayFloat.size(); arrayIndex < countArray; ++arrayIndex )
					{
						file << arrayFloat[ arrayIndex ];

						if ( arrayIndex + 1 == countArray )
							file << "\n";
						else
							file << ",\n";
					}

					file << "\t\t\t]";
					break;
				}
				case LMTProxyParameter::PT_ARRAY_INT:
				{
					auto&		arrayInt = proxyParameter.GetValueArrayInt();
					file << "\t\t\t[\n";

					for ( std::uint32_t arrayIndex = 0, countArray = arrayInt.size(); arrayIndex < countArray; ++arrayIndex )
					{
						file << arrayInt[ arrayIndex ];

						if ( arrayIndex + 1 == countArray )
							file << "\n";
						else
							file << ",\n";
					}

					file << "\t\t\t]";
					break;
				}
				case LMTProxyParameter::PT_ARRAY_VECTOR_2D: 
				{
					auto&		arrayVector2D = proxyParameter.GetValueArrayVector2D();
					file << "\t\t\t[\n";

					for ( std::uint32_t arrayIndex = 0, countArray = arrayVector2D.size(); arrayIndex < countArray; ++arrayIndex )
					{
						LMTVector2D&		value = arrayVector2D[ arrayIndex ];
						file << "{ \"x\": " << value.x << ", \"y\": " << value.y << " }";

						if ( arrayIndex + 1 == countArray )
							file << "\n";
						else
							file << ",\n";
					}

					file << "\t\t\t]";
					break;
				}
				case LMTProxyParameter::PT_ARRAY_VECTOR_3D: 
				{
					auto&		arrayVector3D = proxyParameter.GetValueArrayVector3D();
					file << "\t\t\t[\n";

					for ( std::uint32_t arrayIndex = 0, countArray = arrayVector3D.size(); arrayIndex < countArray; ++arrayIndex )
					{
						LMTVector3D&		value = arrayVector3D[ arrayIndex ];
						file << "{ \"x\": " << value.x << ", \"y\": " << value.y << ", \"z\": " << value.z << " }";

						if ( arrayIndex + 1 == countArray )
							file << "\n";
						else
							file << ",\n";
					}

					file << "\t\t\t]";
					break;
				}
				case LMTProxyParameter::PT_ARRAY_VECTOR_4D:
				{
					auto&		arrayVector4D = proxyParameter.GetValueArrayVector4D();
					file << "\t\t\t[\n";

					for ( std::uint32_t arrayIndex = 0, countArray = arrayVector4D.size(); arrayIndex < countArray; ++arrayIndex )
					{
						LMTVector4D&		value = arrayVector4D[ arrayIndex ];
						file << "{ \"x\": " << value.x << ", \"y\": " << value.y << ", \"z\": " << value.z << ", \"w\": " << value.w << " }";

						if ( arrayIndex + 1 == countArray )
							file << "\n";
						else
							file << ",\n";
					}

					file << "\t\t\t]";
					break;
				}
				case LMTProxyParameter::PT_BOOL:				file << ( proxyParameter.GetValueBool() ? "true" : "false" );	break;
				case LMTProxyParameter::PT_FLOAT:				file << proxyParameter.GetValueFloat();							break;
				case LMTProxyParameter::PT_INT:					file << proxyParameter.GetValueInt();							break;
				case LMTProxyParameter::PT_SHADER_PARAMETER:	file << proxyParameter.GetValueShaderParameter();				break;
				case LMTProxyParameter::PT_VECTOR_2D: 
				{
					LMTVector2D&		value = proxyParameter.GetValueVector2D();
					file << "{ \"x\": " << value.x << ", \"y\": " << value.y << " }";
					break;
				}
				case LMTProxyParameter::PT_VECTOR_3D:
				{
					LMTVector3D&		value = proxyParameter.GetValueVector3D();
					file << "{ \"x\": " << value.x << ", \"y\": " << value.y << ", \"z\": " << value.z << " }";
					break;
				}
				case LMTProxyParameter::PT_VECTOR_4D:
				{
					LMTVector4D&		value = proxyParameter.GetValueVector4D();
					file << "{ \"x\": " << value.x << ", \"y\": " << value.y << ", \"z\": " << value.z << ", \"w\": " << value.w << " }";
					break;
				}
				}

				if ( indexParameter + 1 == countParameters )
					file << "\n";
				else
					file << ",\n";
			}

			file << "\t\t}";
			if ( index + 1 == count )
				file << "\n";
			else
				file << ",\n\n";
		}

		file << "\t}";
	}

	file << "\n}";
	file.close();
	return true;
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void LMTDoc::Clear()
{
	isEnabledDepthTest = false;
	isEnabledDepthWrite = false;
	isEnabledBlend = false;
	isEnabledCullFace = false;
	cullfaceType = CT_BACK;
	shader = "";
	surface = "";

	parameters.clear();
	proxes.clear();
}

// ------------------------------------------------------------------------------------ //
// Add parameter
// ------------------------------------------------------------------------------------ //
void LMTDoc::AddParameter( const LMTParameter& Parameter )
{
	parameters.push_back( Parameter );
}

// ------------------------------------------------------------------------------------ //
// Remove parameter
// ------------------------------------------------------------------------------------ //
void LMTDoc::RemoveParameter( std::uint32_t Index )
{
	if ( Index >= parameters.size() ) return;
	parameters.erase( parameters.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Add proxy
// ------------------------------------------------------------------------------------ //
void LMTDoc::AddProxy( const LMTProxy& Proxy )
{
	proxes.push_back( Proxy );
}

// ------------------------------------------------------------------------------------ //
// Remove proxy
// ------------------------------------------------------------------------------------ //
void LMTDoc::RemoveProxy( std::uint32_t Index )
{
	if ( Index >= proxes.size() ) return;
	proxes.erase( proxes.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Enable depth test
// ------------------------------------------------------------------------------------ //
void LMTDoc::EnableDepthTest( bool Enable )
{
	isEnabledDepthTest = Enable;
}

// ------------------------------------------------------------------------------------ //
// Enable depth write
// ------------------------------------------------------------------------------------ //
void LMTDoc::EnableDepthWrite( bool Enable )
{
	isEnabledDepthWrite = Enable;
}

// ------------------------------------------------------------------------------------ //
// Enable blend
// ------------------------------------------------------------------------------------ //
void LMTDoc::EnableBlend( bool Enable )
{
	isEnabledBlend = Enable;
}

// ------------------------------------------------------------------------------------ //
// Enable cullface
// ------------------------------------------------------------------------------------ //
void LMTDoc::EnableCullFace( bool Enable )
{
	isEnabledCullFace = Enable;
}

// ------------------------------------------------------------------------------------ //
// Find parameter
// ------------------------------------------------------------------------------------ //
LMTParameter* LMTDoc::FindParameter( const std::string& Name )
{
	if ( !parameters.empty() ) return nullptr;

	for ( std::uint32_t index = 0, count = parameters.size(); index < count; ++index )
		if ( parameters[ index ].GetName() == Name )
			return &parameters[ index ];

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Find proxy
// ------------------------------------------------------------------------------------ //
LMTProxy* LMTDoc::FindProxy( const std::string& Name )
{
	if ( !proxes.empty() ) return nullptr;

	for ( std::uint32_t index = 0, count = proxes.size(); index < count; ++index )
		if ( proxes[ index ].GetName() == Name )
			return &proxes[ index ];

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Set surface
// ------------------------------------------------------------------------------------ //
void LMTDoc::SetSurface( const std::string& Surface )
{
	surface = Surface;
}

// ------------------------------------------------------------------------------------ //
// Set shader
// ------------------------------------------------------------------------------------ //
void LMTDoc::SetShader( const std::string& Shader )
{
	shader = Shader;
}

// ------------------------------------------------------------------------------------ //
// Set cullface type
// ------------------------------------------------------------------------------------ //
void LMTDoc::SetCullfaceType( CULLFACE_TYPE Type )
{
	cullfaceType = Type;
}

// ------------------------------------------------------------------------------------ //
// Is enabled depth test
// ------------------------------------------------------------------------------------ //
bool LMTDoc::IsEnabledDepthTest() const
{
	return isEnabledDepthTest;
}

// ------------------------------------------------------------------------------------ //
// Is enabled depth write
// ------------------------------------------------------------------------------------ //
bool LMTDoc::IsEnabledDepthWrite() const
{
	return isEnabledDepthWrite;
}

// ------------------------------------------------------------------------------------ //
// Is enabled blend
// ------------------------------------------------------------------------------------ //
bool LMTDoc::IsEnabledBlend() const
{
	return isEnabledBlend;
}

// ------------------------------------------------------------------------------------ //
// Is enabled cullface
// ------------------------------------------------------------------------------------ //
bool LMTDoc::IsEnabledCullFace() const
{
	return isEnabledCullFace;
}

// ------------------------------------------------------------------------------------ //
// Get cullface typ
// ------------------------------------------------------------------------------------ //
LMTDoc::CULLFACE_TYPE LMTDoc::GetCullfaceType() const
{
	return cullfaceType;
}

// ------------------------------------------------------------------------------------ //
// Get shader
// ------------------------------------------------------------------------------------ //
const std::string& LMTDoc::GetShader() const
{
	return shader;
}

// ------------------------------------------------------------------------------------ //
// Get surface
// ------------------------------------------------------------------------------------ //
const std::string& LMTDoc::GetSurface() const
{
	return surface;
}

// ------------------------------------------------------------------------------------ //
// Get parameter
// ------------------------------------------------------------------------------------ //
LMTParameter* LMTDoc::GetParameter( std::uint32_t Index ) const
{
	if ( Index >= parameters.size() ) return nullptr;
	return ( LMTParameter* ) &parameters[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get parameters
// ------------------------------------------------------------------------------------ //
const std::vector<LMTParameter>& LMTDoc::GetParameters() const
{
	return parameters;
}

// ------------------------------------------------------------------------------------ //
// Get proxy
// ------------------------------------------------------------------------------------ //
LMTProxy* LMTDoc::GetProxy( std::uint32_t Index ) const
{
	if ( Index >= proxes.size() ) return nullptr;
	return ( LMTProxy* ) &proxes[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get proxes
// ------------------------------------------------------------------------------------ //
const std::vector<LMTProxy>& LMTDoc::GetProxes() const
{
	return proxes;
}
