//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/text.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Text::Text() :
    isNeedUpdateTransformation( true ),
	position( 0.f ),
	rotation( 1.f, 0.f, 0.f, 0.f ),
	scale( 1.f )
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
{}

// ------------------------------------------------------------------------------------ //
// Set character size
// ------------------------------------------------------------------------------------ //
void le::Text::SetCharacterSize( UInt32_t CharacterSize )
{}

// ------------------------------------------------------------------------------------ //
// Set text
// ------------------------------------------------------------------------------------ //
void le::Text::SetText( const char* Text )
{}

// ------------------------------------------------------------------------------------ //
// Set color
// ------------------------------------------------------------------------------------ //
void le::Text::SetColor( const Vector3D_t& Color )
{}

// ------------------------------------------------------------------------------------ //
// Set letter spacing factor
// ------------------------------------------------------------------------------------ //
void le::Text::SetLetterSpacingFactor( float LetterSpacingFactor )
{}

// ------------------------------------------------------------------------------------ //
// Set line spacing factor
// ------------------------------------------------------------------------------------ //
void le::Text::SetLineSpacingFactor( float LineSpacingFactor )
{}

// ------------------------------------------------------------------------------------ //
// Get font
// ------------------------------------------------------------------------------------ //
le::IFont* le::Text::GetFont() const
{
    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Get character size
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Text::GetCharacterSize() const
{
    return 0;
}

// ------------------------------------------------------------------------------------ //
// Get text
// ------------------------------------------------------------------------------------ //
const char* le::Text::GetText() const
{
    return "";
}

// ------------------------------------------------------------------------------------ //
// Get color
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Text::GetColor() const
{
    return Vector3D_t();
}

// ------------------------------------------------------------------------------------ //
// Get letter spacing factor
// ------------------------------------------------------------------------------------ //
float le::Text::GetLetterSpacingFactor() const
{
    return 0.f;
}

// ------------------------------------------------------------------------------------ //
// Get line spacing factor
// ------------------------------------------------------------------------------------ //
float le::Text::GetLineSpacingFactor() const
{
    return 0.f;
}

// ------------------------------------------------------------------------------------ //
// Get mesh
// ------------------------------------------------------------------------------------ //
le::IMesh* le::Text::GetMesh() const
{
    return nullptr;
}