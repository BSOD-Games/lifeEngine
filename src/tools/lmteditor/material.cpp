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
	return false;
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
		LMTParameter			lmtParameter;

		lmtParameter.SetName( parameter->GetName().toStdString() );

		// Set value in shader parameter
		switch ( parameter->GetType() )
		{
		case le::SPT_INT:				lmtParameter.SetValueInt( parameter->GetValueInt() );				break;
		case le::SPT_FLOAT:				lmtParameter.SetValueFloat( parameter->GetValueFloat() );			break;
		case le::SPT_SHADER_FLAG:		lmtParameter.SetValueBool( parameter->GetValueBool() );				break;
		case le::SPT_VECTOR_2D:	
		{
			le::Vector2D_t*			value = &parameter->GetValueVector2D();
			lmtParameter.SetValueVector2D( LMTVector2D( value->x, value->y ) );
			break;
		}
		case le::SPT_VECTOR_3D:
		{
			le::Vector3D_t*			value = &parameter->GetValueVector3D();
			lmtParameter.SetValueVector3D( LMTVector3D( value->x, value->y, value->z ) );
			break;
		}
		case le::SPT_VECTOR_4D:
		{
			le::Vector4D_t*			value = &parameter->GetValueVector4D();
			lmtParameter.SetValueVector4D( LMTVector4D( value->x, value->y, value->z, value->w ) );
			break;
		}
		case le::SPT_COLOR:
		{
			QColor*				value = &parameter->GetValueColor();
			qreal				r, g, b, a;
			value->getRgbF( &r, &g, &b, &a );

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

	return lmtDoc.Save( Path.toStdString() );
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
	if ( Index >= parameters.size() ) return;

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