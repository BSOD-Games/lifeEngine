//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <vector>

#include "common/meshdescriptor.h"
#include "common/meshsurface.h"
#include "engine/ifactory.h"
#include "engine/ifont.h"
#include "studiorender/istudiorender.h"
#include "studiorender/istudiorendertechnique.h"
#include "studiorender/istudiorenderpass.h"
#include "studiorender/itexture.h"
#include "studiorender/imesh.h"
#include "studiorender/ishaderparameter.h"
#include "studiorender/studiovertexelement.h"

#include "global.h"
#include "engine/consolesystem.h"
#include "engine/material.h"
#include "engine/text.h"

//---------------------------------------------------------------------//

struct VertexText
{
	VertexText( const le::Vector3D_t& Position = le::Vector3D_t( 0.f ), const le::Vector2D_t& TexCoords = le::Vector2D_t( 0.f ) ) :
		position( Position ),
		texCoords( TexCoords )
	{}

	le::Vector3D_t		position;
	le::Vector2D_t		texCoords;
};


//---------------------------------------------------------------------//

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Text::Text() :
    isNeedUpdateTransformation( true ),
	isNeedUpdateGeometry( true ),
	position( 0.f ),
	rotation( 1.f, 0.f, 0.f, 0.f ),
	scale( 1.f ),
	font( nullptr ),
	material( nullptr ),
	renderTechique( nullptr ),
	renderPass( nullptr ),
	mesh( nullptr ),
	texture( nullptr ),
	characterSize( 45 ),
	lineSpacingFactor( 1.f ),
	letterSpacingFactor( 1.f ),
	color( 1.f )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Text::~Text()
{}

// ------------------------------------------------------------------------------------ //
// Update transformation
// ------------------------------------------------------------------------------------ //
void le::Text::UpdateTransformation()
{
	if ( !isNeedUpdateTransformation ) return;

	transformation = glm::translate( position ) * glm::mat4_cast( rotation ) * glm::scale( scale );
	isNeedUpdateTransformation = false;
}

// ------------------------------------------------------------------------------------ //
// Update geometry
// ------------------------------------------------------------------------------------ //
void le::Text::UpdateGeometry()
{
	// TODO: Отрефакторить этот говнокод

	if ( !isNeedUpdateGeometry )	return;
	LIFEENGINE_ASSERT( g_studioRender && g_studioRender->GetFactory() );
	IFactory*		studioRenderFactory = g_studioRender->GetFactory();

	if ( !font )
	{
		if ( mesh )
		{
			studioRenderFactory->Delete( mesh );
			mesh = nullptr;
		}

		return;
	}
	
	std::vector< VertexText >	buffer;
	std::vector< UInt32_t >		indeces;
	float						lineSpacing = font->GetLineSpacing( characterSize ) * lineSpacingFactor;
	float						whitespaceWidth = font->GetGlyph( ' ', characterSize ).advance;
	float						letterSpacing = ( whitespaceWidth / 3.f ) * ( letterSpacingFactor - 1.f );
	float						localPositionX = 0.f;
	float						localPositionY = 0.f;

	whitespaceWidth += letterSpacing;

	for ( UInt32_t index = 0, count = text.size(); index < count; ++index )
	{
		switch ( text[ index ] )
		{
		case ' ':
			localPositionX += whitespaceWidth;
			continue;

		case '\t':
			localPositionX += whitespaceWidth * 4.f;
			continue;

		case '\n':
			localPositionY -= lineSpacing;
			localPositionX = 0;
			continue;
		}

		IFont::Glyph	glyph = font->GetGlyph( text[ index ], characterSize );
		
		float			xPosition = localPositionX + glyph.bounds.left;
		float			yPosition = localPositionY - ( glyph.bounds.height - glyph.bounds.top );

		float			u1 = static_cast< float >( glyph.textureRect.left );
		float			v1 = static_cast< float >( glyph.textureRect.top );
		float			u2 = static_cast< float >( glyph.textureRect.left + glyph.textureRect.width ) ;
		float			v2 = static_cast< float >( glyph.textureRect.top + glyph.textureRect.height );

		buffer.push_back( VertexText( Vector3D_t( xPosition,							yPosition + glyph.bounds.height,	1 ), Vector2D_t( u1, v1 ) ) );
		buffer.push_back( VertexText( Vector3D_t( xPosition,							yPosition,							1 ), Vector2D_t( u1, v2 ) ) );
		buffer.push_back( VertexText( Vector3D_t( xPosition + glyph.bounds.width,		yPosition,							1 ), Vector2D_t( u2, v2 ) ) );
		buffer.push_back( VertexText( Vector3D_t( xPosition,							yPosition + glyph.bounds.height,	1 ), Vector2D_t( u1, v1 ) ) );
		buffer.push_back( VertexText( Vector3D_t( xPosition + glyph.bounds.width,		yPosition,							1 ), Vector2D_t( u2, v2 ) ) );
		buffer.push_back( VertexText( Vector3D_t( xPosition + glyph.bounds.width,		yPosition + glyph.bounds.height,	1 ), Vector2D_t( u2, v1 ) ) );

		localPositionX += glyph.advance + letterSpacing;
	}

	for ( UInt32_t index = 0, count = buffer.size(); index < count; ++index )
		indeces.push_back( index );

	if ( !texture )
	{
		texture = font->GetTexture( characterSize );
		sizeTextureFont = Vector2D_t( texture->GetWidth(), texture->GetHeight() );
	}
	else if ( sizeTextureFont != Vector2D_t( texture->GetWidth(), texture->GetHeight() ) )		
		sizeTextureFont = Vector2D_t( texture->GetWidth(), texture->GetHeight() );

	for ( UInt32_t index = 0, count = buffer.size(); index < count; ++index )
		buffer[ index ].texCoords /= sizeTextureFont;

	if ( mesh && mesh->IsCreated() )
	{
		// TODO: Add update mesh
	}
	else
	{
		mesh = ( IMesh* ) studioRenderFactory->Create( MESH_INTERFACE_VERSION );
		renderTechique = ( IStudioRenderTechnique* ) studioRenderFactory->Create( TECHNIQUE_INTERFACE_VERSION );
		renderPass = ( IStudioRenderPass* ) studioRenderFactory->Create( PASS_INTERFACE_VERSION );
		IShaderParameter*		shaderParamTexture = ( IShaderParameter* ) studioRenderFactory->Create( SHADERPARAMETER_INTERFACE_VERSION );
		IShaderParameter*		shaderParamColor = ( IShaderParameter* ) studioRenderFactory->Create( SHADERPARAMETER_INTERFACE_VERSION );
		material = new Material();

		if ( !mesh || !renderTechique || !renderPass || !shaderParamTexture || !shaderParamColor ) 
			return;

		shaderParamTexture->SetName( "basetexture" );
		shaderParamTexture->SetValueTexture( texture );
		shaderParamColor->SetName( "color" );
		shaderParamColor->SetValueVector3D( color );

		renderTechique->SetType( RT_DEFFERED_SHADING );
		renderTechique->AddPass( renderPass );
		renderPass->SetShader( "TextGeneric" );
		renderPass->AddParameter( shaderParamTexture );
		renderPass->AddParameter( shaderParamColor );

		material->AddTechnique( renderTechique );

		// Filling information about vertex element in sprite mesh
		std::vector< StudioVertexElement >		vertexElements =
		{
			{ 3, VET_FLOAT },
			{ 2, VET_FLOAT }
		};

		MeshSurface					surface;
		surface.materialID = surface.lightmapID = surface.startIndex = surface.startVertexIndex = 0;
		surface.countIndeces = indeces.size();

		MeshDescriptor			meshDescriptor;
		meshDescriptor.countIndeces = indeces.size();
		meshDescriptor.countMaterials = 1;
		meshDescriptor.countLightmaps = 0;
		meshDescriptor.countSurfaces = 1;
		meshDescriptor.sizeVerteces = buffer.size() * sizeof( VertexText );

		meshDescriptor.indeces = indeces.data();
		meshDescriptor.materials = &material;
		meshDescriptor.lightmaps = nullptr;
		meshDescriptor.surfaces = &surface;
		meshDescriptor.verteces = buffer.data();

		meshDescriptor.min = Vector3D_t();
		meshDescriptor.max = Vector3D_t();
		meshDescriptor.primitiveType = le::PT_TRIANGLES;
		meshDescriptor.countVertexElements = vertexElements.size();
		meshDescriptor.vertexElements = vertexElements.data();

		mesh->Create( meshDescriptor );
	}

	isNeedUpdateGeometry = false;
}

// ------------------------------------------------------------------------------------ //
// Move model
// ------------------------------------------------------------------------------------ //
void le::Text::Move( const Vector3D_t& FactorMove )
{
	position += FactorMove;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Rotate text
// ------------------------------------------------------------------------------------ //
void le::Text::Rotate( const Vector3D_t& FactorRotate )
{
	Vector3D_t			axis( sin( FactorRotate.x / 2.f ), sin( FactorRotate.y / 2.f ), sin( FactorRotate.z / 2.f ) );
	Vector3D_t			rotations( cos( FactorRotate.x / 2.f ), cos( FactorRotate.y / 2.f ), cos( FactorRotate.z / 2.f ) );

	rotation *= Quaternion_t( rotations.x, axis.x, 0, 0 ) * 
		Quaternion_t( rotations.y, 0, axis.y, 0 ) * 
		Quaternion_t( rotations.z, 0, 0, axis.z );

	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Rotate text
// ------------------------------------------------------------------------------------ //
void le::Text::Rotate( const Quaternion_t& FactorRotate )
{
	rotation *= FactorRotate;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Scale text
// ------------------------------------------------------------------------------------ //
void le::Text::Scale( const Vector3D_t& FactorScale )
{
	scale += FactorScale;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Set position text
// ------------------------------------------------------------------------------------ //
void le::Text::SetPosition( const Vector3D_t& Position )
{
	position = Position;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Set rotation text
// ------------------------------------------------------------------------------------ //
void le::Text::SetRotation( const Vector3D_t& Rotation )
{
	Vector3D_t			axis( sin( Rotation.x / 2.f ), sin( Rotation.y / 2.f ), sin( Rotation.z / 2.f ) );
	Vector3D_t			rotations( cos( Rotation.x / 2.f ), cos( Rotation.y / 2.f ), cos( Rotation.z / 2.f ) );

	rotation = Quaternion_t( rotations.x, axis.x, 0, 0 ) *
		Quaternion_t( rotations.y, 0, axis.y, 0 ) *
		Quaternion_t( rotations.z, 0, 0, axis.z );

	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Set rotation text
// ------------------------------------------------------------------------------------ //
void le::Text::SetRotation( const Quaternion_t& Rotation )
{
	rotation = Rotation;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Set scale text
// ------------------------------------------------------------------------------------ //
void le::Text::SetScale( const Vector3D_t& Scale )
{
	scale = Scale;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Text::GetPosition() const
{
	return position;
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
const le::Quaternion_t& le::Text::GetRotation() const
{
	return rotation;
}

// ------------------------------------------------------------------------------------ //
// Get scale
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Text::GetScale() const
{
	return scale;
}

// ------------------------------------------------------------------------------------ //
// Get transformation
// ------------------------------------------------------------------------------------ //
const le::Matrix4x4_t& le::Text::GetTransformation()
{
	if ( isNeedUpdateTransformation )	
		UpdateTransformation();

	return transformation;
}

// ------------------------------------------------------------------------------------ //
// Set font
// ------------------------------------------------------------------------------------ //
void le::Text::SetFont( IFont* Font )
{
	font = Font;
	isNeedUpdateGeometry = true;
}

// ------------------------------------------------------------------------------------ //
// Set character size
// ------------------------------------------------------------------------------------ //
void le::Text::SetCharacterSize( UInt32_t CharacterSize )
{
	characterSize = CharacterSize;
	isNeedUpdateGeometry = true;
}

// ------------------------------------------------------------------------------------ //
// Set text
// ------------------------------------------------------------------------------------ //
void le::Text::SetText( const char* Text )
{
	text = Text;
	isNeedUpdateGeometry = true;
}

// ------------------------------------------------------------------------------------ //
// Set color
// ------------------------------------------------------------------------------------ //
void le::Text::SetColor( const Vector3D_t& Color )
{
	color = Color / 255.f;
}

// ------------------------------------------------------------------------------------ //
// Set letter spacing factor
// ------------------------------------------------------------------------------------ //
void le::Text::SetLetterSpacingFactor( float LetterSpacingFactor )
{
	letterSpacingFactor = LetterSpacingFactor;
	isNeedUpdateGeometry = true;
}

// ------------------------------------------------------------------------------------ //
// Set line spacing factor
// ------------------------------------------------------------------------------------ //
void le::Text::SetLineSpacingFactor( float LineSpacingFactor )
{
	lineSpacingFactor = LineSpacingFactor;
	isNeedUpdateGeometry = true;
}

// ------------------------------------------------------------------------------------ //
// Get font
// ------------------------------------------------------------------------------------ //
le::IFont* le::Text::GetFont() const
{
    return font;
}

// ------------------------------------------------------------------------------------ //
// Get character size
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Text::GetCharacterSize() const
{
    return characterSize;
}

// ------------------------------------------------------------------------------------ //
// Get text
// ------------------------------------------------------------------------------------ //
const char* le::Text::GetText() const
{
    return text.c_str();
}

// ------------------------------------------------------------------------------------ //
// Get color
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Text::GetColor() const
{
    return color;
}

// ------------------------------------------------------------------------------------ //
// Get letter spacing factor
// ------------------------------------------------------------------------------------ //
float le::Text::GetLetterSpacingFactor() const
{
    return letterSpacingFactor;
}

// ------------------------------------------------------------------------------------ //
// Get line spacing factor
// ------------------------------------------------------------------------------------ //
float le::Text::GetLineSpacingFactor() const
{
    return lineSpacingFactor;
}

// ------------------------------------------------------------------------------------ //
// Get mesh
// ------------------------------------------------------------------------------------ //
le::IMesh* le::Text::GetMesh()
{
	if ( isNeedUpdateGeometry )
		UpdateGeometry();

    return mesh;
}