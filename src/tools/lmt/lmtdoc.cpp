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

#include "lmtdoc.h"

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
	return false;
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
	file << "\t\"version\": 2,\n";
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
				const LMTColor& color = parameter.GetValueColor();
				file << "{ \"r\": " << color.r << ", \"g\": " << color.g << ", \"b\": " << color.b << ", \"a\": " << color.a << " }";
				break;
			}
			case LMTParameter::PT_VECTOR_2D:
			{
				const LMTVector2D& value = parameter.GetValueVector2D();
				file << "{ \"x\": " << value.x << ", \"y\": " << value.y << " }";
				break;
			}
			case LMTParameter::PT_VECTOR_3D:
			{
				const LMTVector3D& value = parameter.GetValueVector3D();
				file << "{ \"x\": " << value.x << ", \"y\": " << value.y << ", \"z\": " << value.z << " }";
				break;
			}
			case LMTParameter::PT_VECTOR_4D:
			{
				const LMTVector4D& value = parameter.GetValueVector4D();
				file << "{ \"x\": " << value.x << ", \"y\": " << value.y << ", \"z\": " << value.z << ", \"w\": " << value.w << " }";
				break;
			}
			}

			if ( index + 1 == count )
				file << "\n";
			else
				file << ",\n";
		}

		file << "\t}\n";
	}
	else
		file << "\n";

	file << "}";
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
