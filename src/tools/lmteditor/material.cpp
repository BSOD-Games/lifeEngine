//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qcolor.h>

#include "engine/ifactory.h"
#include "engine/iengine.h"
#include "engine/ishaderparameter.h"

#include "engineapi.h"
#include "errors.h"
#include "material.h"
#include "lmtdoc.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Material::Material() :
	material( nullptr )
{
	material = ( le::IMaterial* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( MATERIAL_INTERFACE_VERSION );
	if ( !material )	Error_Critical( "Interface le::IMaterial version[" MATERIAL_INTERFACE_VERSION "] not found in core" );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Material::~Material()
{
	if ( material )
	{
		if ( material->GetCountReferences() <= 1 )		material->Release();
		else											material->DecrementReference();
	}
}

// ------------------------------------------------------------------------------------ //
// Load material
// ------------------------------------------------------------------------------------ //
bool Material::Load( const QString& Path )
{
	LMTDoc			lmtDoc;
	if ( !lmtDoc.Load( Path.toStdString() ) )
		return false;

	// Getting general settings
	material->Clear();
	material->SetSurfaceName( lmtDoc.GetSurface().c_str() );
	material->SetShader( lmtDoc.GetShader().c_str() );
	
	switch ( lmtDoc.GetCullfaceType() )
	{
	case LMTDoc::CT_BACK:		material->SetCullFaceType( le::CT_BACK );	break;
	case LMTDoc::CT_FRONT:		material->SetCullFaceType( le::CT_FRONT );	break;
	}

	material->EnableBlend( lmtDoc.IsEnabledBlend() );
	material->EnableCullFace( lmtDoc.IsEnabledCullFace() );
	material->EnableDepthTest( lmtDoc.IsEnabledDepthTest() );
	material->EnableDepthWrite( lmtDoc.IsEnabledDepthWrite() );

	// Getting shader parameters
	const std::vector< LMTParameter >&		shaderParameters = lmtDoc.GetParameters();
	if ( !shaderParameters.empty() )
		for ( quint32 index = 0, count = shaderParameters.size(); index < count; ++index )
		{
			const LMTParameter&					parameter = shaderParameters[ index ];
			ShaderParameterPtr					shaderParameter = std::make_shared< ShaderParameter >();
			
			le::SHADER_PARAMETER_TYPE			shaderParameterType;
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
			}

			shaderParameter->SetName( parameter.GetName().c_str(), shaderParameterType );

			switch ( shaderParameterType )
			{
			case le::SPT_INT:			shaderParameter->SetValueInt( parameter.GetValueInt() );		break;
			case le::SPT_FLOAT:			shaderParameter->SetValueFloat( parameter.GetValueFloat() );	break;
			case le::SPT_SHADER_FLAG:	shaderParameter->SetValueBool( parameter.GetValueBool() );		break;
			case le::SPT_TEXTURE:
			{
				std::string				path = parameter.GetValueTexture();
				le::ITexture*			texture = EngineAPI::GetInstance()->GetResourceSystem()->LoadTexture( path.c_str(), path.c_str() );
				if ( !texture )			continue;

				shaderParameter->SetValueTexture( ShaderParameter::Texture( texture, path.c_str() ) );
				break;
			}
			case le::SPT_COLOR:
			{
				LMTColor			color = parameter.GetValueColor();
				shaderParameter->SetValueColor( QColor::fromRgbF( color.r, color.g, color.b, color.a ) );
				break;
			}
			case le::SPT_VECTOR_2D:
			{
				LMTVector2D		vec2d = parameter.GetValueVector2D();
				shaderParameter->SetValueVector2D( le::Vector2D_t( vec2d.x, vec2d.y ) );
				break;
			}
			case le::SPT_VECTOR_3D:
			{
				LMTVector3D		vec3d = parameter.GetValueVector3D();
				shaderParameter->SetValueVector3D( le::Vector3D_t( vec3d.x, vec3d.y, vec3d.z ) );
				break;
			}
			case le::SPT_VECTOR_4D:
			{
				LMTVector4D		vec4d = parameter.GetValueVector4D();
				shaderParameter->SetValueVector4D( le::Vector4D_t( vec4d.x, vec4d.y, vec4d.z, vec4d.w ) );
				break;
			}
			}

			material->AddParameter( shaderParameter->GetHandle() );
			parameters.push_back( shaderParameter );
		}

	// Getting proxes
	const std::vector< LMTProxy >&		proxes = lmtDoc.GetProxes();
	if ( !proxes.empty() )
		for ( quint32 index = 0, count = proxes.size(); index < count; ++index )
		{
			const LMTProxy&			lmtProxy = proxes[ index ];
			auto&					lmtProxyParameters = lmtProxy.GetParameters();
			MaterialProxyPtr		materialProxy = std::make_shared< MaterialProxy >();
			if ( !materialProxy->Create( lmtProxy.GetName().c_str() ) )		continue;
			
			for ( quint32 indexParameter = 0, countParameters = lmtProxyParameters.size(); indexParameter < countParameters; ++indexParameter )
			{
				const LMTProxyParameter&		lmtProxyParameter = lmtProxyParameters[ indexParameter ];
				MaterialProxyParameterPtr		proxyParameter = std::make_shared< MaterialProxyParameter >();
	
				le::MATERIAL_PROXY_VAR_TYPE		proxyParameterType = le::MPVT_NONE;
				switch ( lmtProxyParameter.GetType() )
				{
				case LMTProxyParameter::PT_BOOL:				proxyParameterType = le::MPVT_BOOL;					break;
				case LMTProxyParameter::PT_INT:					proxyParameterType = le::MPVT_INT;					break;
				case LMTProxyParameter::PT_FLOAT:				proxyParameterType = le::MPVT_FLOAT;				break;
				case LMTProxyParameter::PT_SHADER_PARAMETER:	proxyParameterType = le::MPVT_SHADER_PARAMETER;		break;
				case LMTProxyParameter::PT_VECTOR_2D:			proxyParameterType = le::MPVT_VECTOR_2D;			break;
				case LMTProxyParameter::PT_VECTOR_3D:			proxyParameterType = le::MPVT_VECTOR_3D;			break;
				case LMTProxyParameter::PT_VECTOR_4D:			proxyParameterType = le::MPVT_VECTOR_4D;			break;
				}

				proxyParameter->SetName( lmtProxyParameter.GetName().c_str(), proxyParameterType );

				switch ( proxyParameterType )
				{
				case le::MPVT_BOOL:					proxyParameter->SetValueBool( lmtProxyParameter.GetValueBool() );		break;
				case le::MPVT_INT:					proxyParameter->SetValueInt( lmtProxyParameter.GetValueInt() );			break;
				case le::MPVT_FLOAT:				proxyParameter->SetValueFloat( lmtProxyParameter.GetValueFloat() );		break;
				case le::MPVT_SHADER_PARAMETER:
				{
					std::string		nameShaderParameter = lmtProxyParameter.GetValueShaderParameter();
					bool			isFindShaderParameter = false;
					
					for ( quint32 indexShaderParameter = 0, countShaderParameter = parameters.size(); indexShaderParameter < countShaderParameter; ++indexShaderParameter )
						if ( parameters[ indexShaderParameter ]->GetName() == nameShaderParameter.c_str() )
						{
							proxyParameter->SetValueShaderParameter( parameters[ indexShaderParameter ]->GetHandle() );
							isFindShaderParameter = true;
							break;
						}

					if ( isFindShaderParameter )	break;
					else							continue;
				}
				case le::MPVT_VECTOR_2D:
				{
					LMTVector2D			vec2d = lmtProxyParameter.GetValueVector2D();
					proxyParameter->SetValueVector2D( le::Vector2D_t( vec2d.x, vec2d.y ) );
					break;
				}
				case le::MPVT_VECTOR_3D:
				{
					LMTVector3D			vec3d = lmtProxyParameter.GetValueVector3D();
					proxyParameter->SetValueVector3D( le::Vector3D_t( vec3d.x, vec3d.y, vec3d.z ) );
					break;
				}
				case le::MPVT_VECTOR_4D:
				{
					LMTVector4D			vec4d = lmtProxyParameter.GetValueVector4D();
					proxyParameter->SetValueVector4D( le::Vector4D_t( vec4d.x, vec4d.y, vec4d.z, vec4d.w ) );
					break;
				}
				}

				materialProxy->AddParameter( proxyParameter );
			}

			material->AddProxy( materialProxy->GetHandle() );
			this->proxes.push_back( materialProxy );
		}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Save material
// ------------------------------------------------------------------------------------ //
bool Material::Save( const QString& Path )
{
	LMTDoc			lmtDoc;
	lmtDoc.EnableBlend( material->IsBlend() );
	lmtDoc.EnableCullFace( material->IsCullFace() );
	lmtDoc.EnableDepthTest( material->IsDepthTest() );
	lmtDoc.EnableDepthWrite( material->IsDepthWrite() );
	lmtDoc.SetShader( material->GetNameShader() );
	lmtDoc.SetSurface( material->GetSurfaceName() );

	// Set caullface type
	switch ( material->GetCullFaceType() )
	{
	case le::CT_BACK:	lmtDoc.SetCullfaceType( LMTDoc::CT_BACK );	break;
	case le::CT_FRONT:	lmtDoc.SetCullfaceType( LMTDoc::CT_FRONT );	break;
	}

	// Add shader parameters
	for ( quint32 index = 0, count = parameters.size(); index < count; ++index )
	{
		ShaderParameterPtr			parameter = parameters[ index ];
		LMTParameter				lmtParameter;

		lmtParameter.SetName( parameter->GetName().toStdString() );

		// Set value in shader parameter
		switch ( parameter->GetType() )
		{
		case le::SPT_INT:				lmtParameter.SetValueInt( parameter->GetValueInt() );				break;
		case le::SPT_FLOAT:				lmtParameter.SetValueFloat( parameter->GetValueFloat() );			break;
		case le::SPT_SHADER_FLAG:		lmtParameter.SetValueBool( parameter->GetValueBool() );				break;
		case le::SPT_VECTOR_2D:	
		{
			le::Vector2D_t			value = parameter->GetValueVector2D();
			lmtParameter.SetValueVector2D( LMTVector2D( value.x, value.y ) );
			break;
		}
		case le::SPT_VECTOR_3D:
		{
			le::Vector3D_t			value = parameter->GetValueVector3D();
			lmtParameter.SetValueVector3D( LMTVector3D( value.x, value.y, value.z ) );
			break;
		}
		case le::SPT_VECTOR_4D:
		{
			le::Vector4D_t			value = parameter->GetValueVector4D();
			lmtParameter.SetValueVector4D( LMTVector4D( value.x, value.y, value.z, value.w ) );
			break;
		}
		case le::SPT_COLOR:
		{
			QColor				value = parameter->GetValueColor();
			qreal				r, g, b, a;
			value.getRgbF( &r, &g, &b, &a );

			lmtParameter.SetValueColor( LMTColor( r, g, b, a ) );
			break;
		}
		case le::SPT_TEXTURE:
		{
			ShaderParameter::Texture*		value = &parameter->GetValueTexture();
			lmtParameter.SetValueTexture( value->path.toStdString() );
			break;
		}
		}

		lmtDoc.AddParameter( lmtParameter );
	}

	// Add proxes
	for ( quint32 index = 0, count = proxes.size(); index < count; ++index )
	{
		MaterialProxyPtr			proxy = proxes[ index ];
		auto&						parameters = proxy->GetParameters();
		LMTProxy					lmtProxy;

		lmtProxy.SetName( proxy->GetName().toStdString() );
		for ( quint32 indexParameter = 0, countParameter = parameters.size(); indexParameter < countParameter; ++indexParameter )
		{
			MaterialProxyParameterPtr		proxyParameter = parameters[ indexParameter ];
			LMTProxyParameter				lmtProxyParameter;
			
			lmtProxyParameter.SetName( proxyParameter->GetName().toStdString() );
			switch ( proxyParameter->GetType() )
			{
			case le::MPVT_BOOL:					lmtProxyParameter.SetValueBool( proxyParameter->GetValueBool() );		break;
			case le::MPVT_INT:					lmtProxyParameter.SetValueInt( proxyParameter->GetValueInt() );			break;
			case le::MPVT_FLOAT:				lmtProxyParameter.SetValueFloat( proxyParameter->GetValueFloat() );		break;
			case le::MPVT_SHADER_PARAMETER:
			{
				le::IShaderParameter*			shaderParameter = proxyParameter->GetValueShaderParameter();
				if ( !shaderParameter ) continue;

				lmtProxyParameter.SetValueShaderParameter( shaderParameter->GetName() );
				break;
			}
			case le::MPVT_VECTOR_2D:	
			{
				le::Vector2D_t			value = proxyParameter->GetValueVector2D();
				lmtProxyParameter.SetValueVector2D( LMTVector2D( value.x, value.y ) );			
				break;
			}
			case le::MPVT_VECTOR_3D:
			{
				le::Vector3D_t			value = proxyParameter->GetValueVector3D();
				lmtProxyParameter.SetValueVector3D( LMTVector3D( value.x, value.y, value.z ) );
				break;
			}
			case le::MPVT_VECTOR_4D:
			{
				le::Vector4D_t			value = proxyParameter->GetValueVector4D();
				lmtProxyParameter.SetValueVector4D( LMTVector4D( value.x, value.y, value.z, value.z ) );
				break;
			}
			}
	
			lmtProxy.AddParameter( lmtProxyParameter );
		}

		lmtDoc.AddProxy( lmtProxy );
	}

	return lmtDoc.Save( Path.toStdString() );
}

// ------------------------------------------------------------------------------------ //
// Set surface
// ------------------------------------------------------------------------------------ //
void Material::SetSurface( const QString& Name )
{
	if ( !material ) return;
	material->SetSurfaceName( Name.toLocal8Bit().data() );
}

// ------------------------------------------------------------------------------------ //
// Set shader
// ------------------------------------------------------------------------------------ //
void Material::SetShader( const QString& Name )
{
	if ( !material ) return;
	material->SetShader( Name.toLocal8Bit().data() );
}

// ------------------------------------------------------------------------------------ //
// Enabled depth test
// ------------------------------------------------------------------------------------ //
void Material::EnableDepthTest( bool Enable )
{
	if ( !material ) return;
	material->EnableDepthTest( Enable );
}

// ------------------------------------------------------------------------------------ //
// Enabled depth write
// ------------------------------------------------------------------------------------ //
void Material::EnableDepthWrite( bool Enable )
{
	if ( !material ) return;
	material->EnableDepthWrite( Enable );
}

// ------------------------------------------------------------------------------------ //
// Enabled blend
// ------------------------------------------------------------------------------------ //
void Material::EnableBlend( bool Enable )
{
	if ( !material ) return;
	material->EnableBlend( Enable );
}

// ------------------------------------------------------------------------------------ //
// Enabled cullface
// ------------------------------------------------------------------------------------ //
void Material::EnableCullFace( bool Enable )
{
	if ( !material ) return;
	material->EnableCullFace( Enable );
}

// ------------------------------------------------------------------------------------ //
// Set cullface type
// ------------------------------------------------------------------------------------ //
void Material::SetCullFaceType( le::CULLFACE_TYPE CullFaceType )
{
	if ( !material ) return;
	material->SetCullFaceType( CullFaceType );
}

// ------------------------------------------------------------------------------------ //
// Add parameter
// ------------------------------------------------------------------------------------ //
void Material::AddParameter( const QString& Name, le::SHADER_PARAMETER_TYPE Type )
{
	if ( !material ) return;

	ShaderParameterPtr				shaderParameter = std::make_shared< ShaderParameter >();
	shaderParameter->SetName( Name, Type );

	material->AddParameter( shaderParameter->GetHandle() );
	parameters.push_back( shaderParameter );
}

// ------------------------------------------------------------------------------------ //
// Remove parameter
// ------------------------------------------------------------------------------------ //
void Material::RemoveParameter( quint32 Index )
{
	if ( !material || Index >= parameters.size() ) return;

	ShaderParameterPtr			parameter = parameters[ Index ];

	// Delete this shader parameter in all proxes
	for ( quint32 index = 0, count = proxes.size(); index < count; ++index )
	{
		MaterialProxyPtr			proxy = proxes[ index ];
		for ( quint32 indexParameter = 0; indexParameter < proxy->GetCountParameters(); ++indexParameter )
		{
			MaterialProxyParameterPtr			proxyParameter = proxy->GetParameter( indexParameter );
			if ( proxyParameter->GetType() == le::MPVT_SHADER_PARAMETER )
			{
				le::IShaderParameter*			shaderParameter = proxyParameter->GetValueShaderParameter();
				if ( !shaderParameter ) continue;
				if ( shaderParameter->GetName() == parameter->GetName() )
				{
					proxy->RemoveParameter( indexParameter );
					indexParameter--;
				}
			}
		}
	}

	material->RemoveParameter( Index );
	parameters.erase( parameters.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Has parameter
// ------------------------------------------------------------------------------------ //
bool Material::HasParameter( const QString& Name ) const
{
	for ( quint32 index = 0, count = parameters.size(); index < count; ++index )
		if ( parameters[ index ]->GetName() == Name )
			return true;

	return false;
}

// ------------------------------------------------------------------------------------ //
// Clear material
// ------------------------------------------------------------------------------------ //
void Material::Clear()
{
	if ( !material ) return;

	material->Clear();
	parameters.clear();
	proxes.clear();
}

// ------------------------------------------------------------------------------------ //
// Add proxy
// ------------------------------------------------------------------------------------ //
void Material::AddProxy( const QString& Name )
{
	if ( !material ) return;

	MaterialProxyPtr				proxy = std::make_shared< MaterialProxy >();
	if ( !proxy->Create( Name.toStdString().c_str() ) )
		return;

	material->AddProxy( proxy->GetHandle() );
	proxes.push_back( proxy );
}

// ------------------------------------------------------------------------------------ //
// Remove proxy
// ------------------------------------------------------------------------------------ //
void Material::RemoveProxy( quint32 Index )
{
	if ( Index >= proxes.size() ) return;

	material->RemoveProxy( Index );
	proxes.erase( proxes.begin() + Index );
}