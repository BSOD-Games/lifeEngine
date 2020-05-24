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
#include "studiorender/itexture.h"
#include "studiorender/imesh.h"
#include "studiorender/studiovertexelement.h"

#include "global.h"
#include "shaderparameter.h"
#include "material.h"
#include "consolesystem.h"
#include "text.h"

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
	mesh( nullptr ),
	materialParam_color( nullptr ),
	characterSize( 45 ),
	lineSpacingFactor( 1.f ),
	letterSpacingFactor( 1.f ),
	color( 255.f, 255.f, 255.f ),
    textureSize( 0.f ),
    countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Text::~Text()
{
	Delete();
}

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
// Удалить текст
// ------------------------------------------------------------------------------------ //
void le::Text::Delete()
{
    if ( font )
    {
        if ( font->GetCountReferences() <= 1 )      font->Release();
        else                                        font->DecrementReference();

        font = nullptr;
    }

	if ( mesh )
	{
        if ( mesh->GetCountReferences() <= 1 )  mesh->Release();
        else                                    mesh->DecrementReference();

        mesh = nullptr;
	}

	if ( material )
	{
        if ( material->GetCountReferences() <= 1 )      material->Release();
        else                                            material->DecrementReference();

		if ( materialParam_color )
		{
			if ( materialParam_color->GetCountReferences() <= 1 )       materialParam_color->Release();
			else                                                        materialParam_color->DecrementReference();

			materialParam_color = nullptr;
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Обновить геометрию текста
// ------------------------------------------------------------------------------------ //
void le::Text::UpdateGeometry()
{
	// Если шрифт не указан - удаляем меш и материал, если есть
	if ( !font )
	{
		Delete();
		return;
	}

	// Если геометрия текста с размером текстуры не менялись, то ничего не делаем	
	ITexture*			font_texture = font->GetTexture( characterSize );
	Vector2D_t			font_textureSize;
	if ( font_texture )
	{
		font_textureSize = Vector2D_t( font_texture->GetWidth(), font_texture->GetHeight() );
		if( !isNeedUpdateGeometry && textureSize == font_textureSize )
			return;
	}

	std::vector< VertexText >		buffer;
	std::vector< UInt32_t >			indeces;
	float							lineSpacing = font->GetLineSpacing( characterSize ) * lineSpacingFactor;
	float							whitespaceWidth = font->GetGlyph( ' ', characterSize ).advance;
	float							letterSpacing = ( whitespaceWidth / 3.f ) * ( letterSpacingFactor - 1.f );
	float							localPositionX = 0;
	float							localPositionY = 0;	

	whitespaceWidth += letterSpacing;

	// Обновляем геометрию текста
	for ( uint32_t index = 0, vertexIndex = 0, count = text.size(); index < count; ++index )
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

		// Записываем вершины текста
		buffer.push_back( VertexText( Vector3D_t( xPosition, yPosition + glyph.bounds.height, 0.f ), Vector2D_t( u1, v1 ) ) );
		buffer.push_back( VertexText( Vector3D_t( xPosition, yPosition, 0.f ), Vector2D_t( u1, v2 ) ) );
		buffer.push_back( VertexText( Vector3D_t( xPosition + glyph.bounds.width, yPosition, 0.f ), Vector2D_t( u2, v2 ) ) );
		buffer.push_back( VertexText( Vector3D_t( xPosition + glyph.bounds.width, yPosition + glyph.bounds.height, 0.f ), Vector2D_t( u2, v1 ) ) );

		// Записываем индексы вершин
		indeces.push_back( vertexIndex ); 
		indeces.push_back( vertexIndex + 1 ); 
		indeces.push_back( vertexIndex + 2 ); 
		indeces.push_back( vertexIndex ); 
		indeces.push_back( vertexIndex + 2 ); 
		indeces.push_back( vertexIndex + 3 ); 

		vertexIndex += 4;
		localPositionX += glyph.advance + letterSpacing;
	}	

	// Если размеры текстуры менялись - запоминаем новые размеры
	if ( !font_texture )
	{
		font_texture = font->GetTexture( characterSize );
		textureSize = Vector2D_t( font_texture->GetWidth(), font_texture->GetHeight() );
	}
	else if ( textureSize != font_textureSize )
		textureSize = font_textureSize;
		
	// Проходимся по буферу и нормализуем текстурные координаты в пределах [0..1], 
	// а так же записываем индексы вершин
	for ( UInt32_t index = 0, count = buffer.size(); index < count; ++index )
		buffer[ index ].texCoords /= textureSize;

	// Если ранее меш текста был создан - обновляем данные, иначе создаем
	if ( mesh && mesh->IsCreated() )
	{
        mesh->Update( buffer.data(), buffer.size() * sizeof( VertexText ), 0 );
	}
	else
	{
		IFactory*		studioRenderFactory = g_studioRender->GetFactory();

		// Если класс меша не создан - создаем
		if ( !mesh )	
			mesh = ( IMesh* ) studioRenderFactory->Create( MESH_INTERFACE_VERSION );
		
		// Если класс материала не создан - создаем
		if ( !material )
		{
			material = new Material();
			materialParam_color = new ShaderParameter();

            materialParam_color->IncrementReference();
			materialParam_color->SetName( "color" );
			materialParam_color->SetValueVector3D( color / 255.f );		

			material->SetShader( "TextGeneric" );
			material->AddParameter( materialParam_color );
            material->IncrementReference();
		}

		// Заполняем массив формата вершин
		std::vector< StudioVertexElement >		vertexElements =
		{
			{ 3, VET_FLOAT },
			{ 2, VET_FLOAT }
		};	

		// Заполняем информацию об поверхности меша
		MeshSurface					surface;
		surface.materialID = surface.lightmapID = surface.startIndex = surface.startVertexIndex = 0;
		surface.countIndeces = indeces.size();

		// Заполняем описание меша для его создания
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

        mesh->IncrementReference();
		mesh->Create( meshDescriptor );
	}

	isNeedUpdateGeometry = false;
}

// ------------------------------------------------------------------------------------ //
// Set material
// ------------------------------------------------------------------------------------ //
void le::Text::SetMaterial( IMaterial* Material )
{
	if ( material )
	{
		if ( material->GetCountReferences() <= 1 )      material->Release();
		else                                            material->DecrementReference();

		if ( materialParam_color )
		{
			if ( materialParam_color->GetCountReferences() <= 1 )       materialParam_color->Release();
			else                                                        materialParam_color->DecrementReference();
		}

		materialParam_color = nullptr;
	}

	material = Material;
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
    if ( font )
    {
        if ( font->GetCountReferences() <= 1 )
            font->Release();
        else
            font->DecrementReference();
    }

	font = Font;
    font->IncrementReference();
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
// Get material
// ------------------------------------------------------------------------------------ //
le::IMaterial* le::Text::GetMaterial() const
{
	return material;
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
	UpdateGeometry();
    return mesh;
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Text::IncrementReference()
{
    ++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Text::DecrementReference()
{
    --countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::Text::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get mesh
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Text::GetCountReferences() const
{
    return countReferences;
}
