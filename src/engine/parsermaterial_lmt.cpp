//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/iresourcesystem.h"
#include "engine/imaterialproxy.h"

#include "global.h"
#include "lmtdoc.h"
#include "parsermaterial_lmt.h"
#include "shaderparameter.h"
#include "material.h"
#include "materialproxyvar.h"
#include "materialproxyfactory.h"
#include "materialsystem.h"
#include "consolesystem.h"

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserMaterialLMT::GetVersion() const
{
	return "2.0";
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserMaterialLMT::GetName() const
{
	return "lifeEngine materials";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserMaterialLMT::GetCountFileExtensions() const
{
	return 1;
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserMaterialLMT::GetFileExtensions() const
{
	static const char*			extensions[ 1 ] = { "lmt" };
	return extensions;
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserMaterialLMT::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::IMaterial* le::ParserMaterialLMT::Read( const char* Path, IResourceSystem* ResourceSystem, IMaterialSystem* MaterialSystem, IFactory* EngineFactory )
{
	LMTDoc			lmtDoc;
	if ( !lmtDoc.Load( Path ) )
	{
		g_consoleSystem->PrintError( "Failed loading material" );
		return nullptr;
	}

	// Create material and set surface name and shader
	Material*		material = new Material();
	material->SetSurfaceName( lmtDoc.GetSurface().c_str() );
	material->SetShader( lmtDoc.GetShader().c_str() );

	// Set cullface type
	switch ( lmtDoc.GetCullfaceType() )
	{
	case LMTDoc::CT_BACK:		material->SetCullFaceType( le::CT_BACK );	break;
	case LMTDoc::CT_FRONT:		material->SetCullFaceType( le::CT_FRONT );	break;
	}

	// Set settings material
	material->EnableBlend( lmtDoc.IsEnabledBlend() );
	material->EnableCullFace( lmtDoc.IsEnabledCullFace() );
	material->EnableDepthTest( lmtDoc.IsEnabledDepthTest() );
	material->EnableDepthWrite( lmtDoc.IsEnabledDepthWrite() );

	// Getting shader parameters
	const std::vector< LMTParameter >&		shaderParameters = lmtDoc.GetParameters();
	if ( !shaderParameters.empty() )
		for ( UInt32_t index = 0, count = shaderParameters.size(); index < count; ++index )
		{
			const LMTParameter&			parameter = shaderParameters[ index ];
			ShaderParameter*		shaderParameter = new ShaderParameter();

			SHADER_PARAMETER_TYPE			shaderParameterType;
			switch ( parameter.GetType() )
			{
			case LMTParameter::PT_INT:			shaderParameterType = le::SPT_INT;			break;
			case LMTParameter::PT_FLOAT:		shaderParameterType = le::SPT_FLOAT;		break;
			case LMTParameter::PT_BOOL:			shaderParameterType = le::SPT_SHADER_FLAG;	break;
			case LMTParameter::PT_TEXTURE:		shaderParameterType = le::SPT_TEXTURE;		break;
			case LMTParameter::PT_COLOR:		shaderParameterType = le::SPT_COLOR;		break;
			case LMTParameter::PT_VECTOR_2D:	shaderParameterType = le::SPT_VECTOR_2D;	break;
			case LMTParameter::PT_VECTOR_3D:	shaderParameterType = le::SPT_VECTOR_3D;	break;
			case LMTParameter::PT_VECTOR_4D:	shaderParameterType = le::SPT_VECTOR_4D;	break;
			default:							delete shaderParameter;						continue;
			}

			shaderParameter->SetName( parameter.GetName().c_str() );

			switch ( shaderParameterType )
			{
			case SPT_INT:			shaderParameter->SetValueInt( parameter.GetValueInt() );			break;
			case SPT_FLOAT:			shaderParameter->SetValueFloat( parameter.GetValueFloat() );		break;
			case SPT_SHADER_FLAG:	shaderParameter->SetValueShaderFlag( parameter.GetValueBool() );	break;
			case SPT_TEXTURE:
			{
				std::string				path = parameter.GetValueTexture();
				ITexture*				texture = ResourceSystem->LoadTexture( path.c_str(), path.c_str() );
				if ( !texture )
				{
					delete shaderParameter;
					continue;
				}

				shaderParameter->SetValueTexture( texture );
				break;
			}
			case SPT_COLOR:
			{
				LMTColor			color = parameter.GetValueColor();
				shaderParameter->SetValueColor( Color_t( color.r, color.g, color.b, color.a ) );
				break;
			}
			case SPT_VECTOR_2D:
			{
				LMTVector2D		vec2d = parameter.GetValueVector2D();
				shaderParameter->SetValueVector2D( Vector2D_t( vec2d.x, vec2d.y ) );
				break;
			}
			case SPT_VECTOR_3D:
			{
				LMTVector3D		vec3d = parameter.GetValueVector3D();
				shaderParameter->SetValueVector3D( Vector3D_t( vec3d.x, vec3d.y, vec3d.z ) );
				break;
			}
			case SPT_VECTOR_4D:
			{
				LMTVector4D		vec4d = parameter.GetValueVector4D();
				shaderParameter->SetValueVector4D( Vector4D_t( vec4d.x, vec4d.y, vec4d.z, vec4d.w ) );
				break;
			}
			}

			material->AddParameter( shaderParameter );
		}

	// Getting proxes
	const std::vector< LMTProxy >& proxes = lmtDoc.GetProxes();
	if ( !proxes.empty() )
		for ( UInt32_t index = 0, count = proxes.size(); index < count; ++index )
		{
			const LMTProxy&			lmtProxy = proxes[ index ];
			auto&					lmtProxyParameters = lmtProxy.GetParameters();
			
			IMaterialProxy*				materialProxy = static_cast< MaterialProxyFactory* >( MaterialSystem->GetMaterialProxyFactory() )->Create( lmtProxy.GetName().c_str() );
			if ( !materialProxy )		continue;

			for ( le::UInt32_t indexParameter = 0, countParameters = lmtProxyParameters.size(); indexParameter < countParameters; ++indexParameter )
			{
				const LMTProxyParameter&		lmtProxyParameter = lmtProxyParameters[ indexParameter ];
				MaterialProxyVar*				proxyParameter = new MaterialProxyVar();

				MATERIAL_PROXY_VAR_TYPE			proxyParameterType = le::MPVT_NONE;
				switch ( lmtProxyParameter.GetType() )
				{
				case LMTProxyParameter::PT_BOOL:				proxyParameterType = MPVT_BOOL;					break;
				case LMTProxyParameter::PT_INT:					proxyParameterType = MPVT_INT;					break;
				case LMTProxyParameter::PT_FLOAT:				proxyParameterType = MPVT_FLOAT;				break;
				case LMTProxyParameter::PT_SHADER_PARAMETER:	proxyParameterType = MPVT_SHADER_PARAMETER;		break;
				case LMTProxyParameter::PT_VECTOR_2D:			proxyParameterType = MPVT_VECTOR_2D;			break;
				case LMTProxyParameter::PT_VECTOR_3D:			proxyParameterType = MPVT_VECTOR_3D;			break;
				case LMTProxyParameter::PT_VECTOR_4D:			proxyParameterType = MPVT_VECTOR_4D;			break;
				}

				proxyParameter->SetName( lmtProxyParameter.GetName().c_str() );

				switch ( proxyParameterType )
				{
				case MPVT_BOOL:					proxyParameter->SetValueBool( lmtProxyParameter.GetValueBool() );		break;
				case MPVT_INT:					proxyParameter->SetValueInt( lmtProxyParameter.GetValueInt() );			break;
				case MPVT_FLOAT:				proxyParameter->SetValueFloat( lmtProxyParameter.GetValueFloat() );		break;
				case MPVT_SHADER_PARAMETER:
				{
					std::string					nameShaderParameter = lmtProxyParameter.GetValueShaderParameter();
					IShaderParameter*			parameter = material->FindParameter( nameShaderParameter.c_str() );
					if ( !parameter )
					{
						delete proxyParameter;
						continue;
					}

					proxyParameter->SetValueShaderParameter( parameter );
					break;
				}
				case MPVT_VECTOR_2D:
				{
					LMTVector2D			vec2d = lmtProxyParameter.GetValueVector2D();
					proxyParameter->SetValueVector2D( Vector2D_t( vec2d.x, vec2d.y ) );
					break;
				}
				case MPVT_VECTOR_3D:
				{
					LMTVector3D			vec3d = lmtProxyParameter.GetValueVector3D();
					proxyParameter->SetValueVector3D( Vector3D_t( vec3d.x, vec3d.y, vec3d.z ) );
					break;
				}
				case MPVT_VECTOR_4D:
				{
					LMTVector4D			vec4d = lmtProxyParameter.GetValueVector4D();
					proxyParameter->SetValueVector4D( Vector4D_t( vec4d.x, vec4d.y, vec4d.z, vec4d.w ) );
					break;
				}
				}

				materialProxy->SetVar( proxyParameter );
			}

			material->AddProxy( materialProxy );
		}

	return material;
}
