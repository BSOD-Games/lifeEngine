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
#include "engine/lifeengine.h"
#include "engine/ifactory.h"
#include "engine/camera.h"
#include "studiorender/istudiorender.h"
#include "studiorender/imesh.h"
#include "studiorender/studiovertexelement.h"
#include "global.h"
#include "sprite.h"
#include "consolesystem.h"

//---------------------------------------------------------------------//

struct VertexSprite
{
	le::Vector3D_t		position;
	le::Vector3D_t		normal;
	le::Vector2D_t		texCoord;
};

//---------------------------------------------------------------------//

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Sprite::Sprite() :
    isNeedUpdateTransformation( true ),
    isNeedUpdateBoundingBox( true ),
	mesh( nullptr ),
    type( ST_SPRITE_ROTATING ),
	size( 1.f ),
	position( 0.f ),
	rotation( 1.f, 0.f, 0.f, 0.f ),
    scale( 1.f ),
    countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Sprite::~Sprite()
{
    if ( mesh->GetCountReferences() <= 1 )
        mesh->Release();
    else
        mesh->DecrementReference();
}

// ------------------------------------------------------------------------------------ //
// Update transformation
// ------------------------------------------------------------------------------------ //
void le::Sprite::UpdateTransformation()
{
	if ( !isNeedUpdateTransformation ) return;

	transformation = glm::translate( position ) * glm::mat4_cast( rotation ) * glm::scale( scale );
    isNeedUpdateTransformation = false;
}

// ------------------------------------------------------------------------------------ //
// Обновить ограничивающее тело
// ------------------------------------------------------------------------------------ //
void le::Sprite::UpdateBoundingBox()
{
    if ( !mesh || !mesh->IsCreated() || !isNeedUpdateBoundingBox ) return;

    Vector3D_t		min = ( rotation * mesh->GetMin() * scale ) + position;
    Vector3D_t		max = ( rotation * mesh->GetMax() * scale ) + position;

    this->min = Vector3D_t( glm::min( min.x, max.x ), glm::min( min.y, max.y ), glm::min( min.z, max.z ) );
    this->max = Vector3D_t( glm::max( max.x, min.x ), glm::max( max.y, min.y ), glm::max( max.z, min.z ) );

    isNeedUpdateBoundingBox = false;
}

// ------------------------------------------------------------------------------------ //
// Calculate transformation for rotating to camera
// ------------------------------------------------------------------------------------ //
le::Matrix4x4_t le::Sprite::GetTransformation( ICamera* Camera )
{
	Matrix4x4_t				transformation = glm::translate( position );
	const Matrix4x4_t&		view = Camera->GetViewMatrix();

	transformation[ 0 ].x = view[ 0 ].x;
	transformation[ 0 ].y = view[ 1 ].x;
	transformation[ 0 ].z = view[ 2 ].x;

	if ( type == ST_SPRITE_ROTATING_ONLY_VERTICAL )
	{
		transformation[ 1 ].x = 0;
		transformation[ 1 ].y = 1;
		transformation[ 1 ].z = 0;
	}
	else
	{
		transformation[ 1 ].x = view[ 0 ].y;
		transformation[ 1 ].y = view[ 1 ].y;
		transformation[ 1 ].z = view[ 2 ].y;
	}

	transformation[ 2 ].x = view[ 0 ].z;
	transformation[ 2 ].y = view[ 1 ].z;
	transformation[ 2 ].z = view[ 2 ].z;

	transformation *= glm::mat4_cast( rotation );
	transformation *= glm::scale( scale );

	return transformation;
}

// ------------------------------------------------------------------------------------ //
// Initialize sprite
// ------------------------------------------------------------------------------------ //
bool le::Sprite::Initialize( const Vector2D_t& Size, IMaterial* Material, SPRITE_TYPE SpriteType )
{
	LIFEENGINE_ASSERT( g_studioRender );
    if ( IsCreated() )
    {
        if ( mesh->GetCountReferences() <= 1 )
            mesh->Release();
        else
            mesh->DecrementReference();
    }

	// Filling vertices array for sprite mesh 
	std::vector< VertexSprite >				verteces =
	{
		{ Vector3D_t( -Size.x, -Size.y, 0.0f ), Vector3D_t( -1.f, -1.f, 1.0f ), Vector2D_t( 0.0f, 0.0f ) },
		{ Vector3D_t( -Size.x, Size.y, 0.0f ), Vector3D_t( -1.f, 1.f, 1.0f ), Vector2D_t( 0.0f, 1.0f ) },
		{ Vector3D_t( Size.x, Size.y, 0.0f ), Vector3D_t( 1.f, 1.f, 1.0f ), Vector2D_t( 1.0f, 1.0f ) },
		{ Vector3D_t( Size.x, -Size.y, 0.0f ), Vector3D_t( 1.f, -1.f, 1.0f ), Vector2D_t( 1.0f, 0.0f ) }
	};

	// Filling indeces array for sprite mesh
	std::vector< UInt32_t >					indeces = { 3, 2, 1, 0 };

	// Filling information about vertex element in sprite mesh
	std::vector< StudioVertexElement >		vertexElements =
	{
		{ 3, VET_FLOAT },
		{ 3, VET_FLOAT },
		{ 2, VET_FLOAT }
	};

	// Filling information about surface in mesh
	MeshSurface					surface;
	surface.materialID = surface.lightmapID = surface.startIndex = surface.startVertexIndex = 0;
	surface.countIndeces = indeces.size();

	// Filling information about sprite mesh
	MeshDescriptor				meshDescriptor;
	meshDescriptor.countIndeces = indeces.size();
	meshDescriptor.countMaterials = 1;
	meshDescriptor.countLightmaps = 0;
	meshDescriptor.countSurfaces = 1;
	meshDescriptor.sizeVerteces = verteces.size() * sizeof( VertexSprite );

	meshDescriptor.indeces = indeces.data();
	meshDescriptor.materials = &Material;
	meshDescriptor.lightmaps = nullptr;
	meshDescriptor.surfaces = &surface;
	meshDescriptor.verteces = verteces.data();

    meshDescriptor.min = Vector3D_t( -Size.x, -Size.y, 0.0f );
    meshDescriptor.max = Vector3D_t( Size.x, Size.y, 0.0f );
	meshDescriptor.primitiveType = le::PT_TRIANGLE_FAN;
	meshDescriptor.countVertexElements = vertexElements.size();
	meshDescriptor.vertexElements = vertexElements.data();

	mesh = ( le::IMesh* ) g_studioRender->GetFactory()->Create( MESH_INTERFACE_VERSION );
	if ( !mesh ) 		return false;

    mesh->IncrementReference();
	mesh->Create( meshDescriptor );

    if ( !mesh->IsCreated() )
    {
        mesh->Release();
        return false;
    }

    isNeedUpdateBoundingBox = true;
	size = Size;
	type = SpriteType;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Set material
// ------------------------------------------------------------------------------------ //
void le::Sprite::SetMaterial( IMaterial* Material )
{
	if ( !IsCreated() )		return;
    LIFEENGINE_ASSERT( Material );

    // TODO: Implement this
}

// ------------------------------------------------------------------------------------ //
// Set size
// ------------------------------------------------------------------------------------ //
void le::Sprite::SetSize( const Vector2D_t& Size )
{
    size = Size;
}

// ------------------------------------------------------------------------------------ //
// Set type sprite
// ------------------------------------------------------------------------------------ //
void le::Sprite::SetType( SPRITE_TYPE SpriteType )
{
    type = SpriteType;
}

// ------------------------------------------------------------------------------------ //
// Is sprite created
// ------------------------------------------------------------------------------------ //
bool le::Sprite::IsCreated() const
{
	return mesh && mesh->IsCreated();
}

// ------------------------------------------------------------------------------------ //
// Get material
// ------------------------------------------------------------------------------------ //
le::IMaterial* le::Sprite::GetMaterial() const
{
	if ( !IsCreated() )		return nullptr;
    return mesh->GetMaterial( 0 );
}

// ------------------------------------------------------------------------------------ //
// Get mesh
// ------------------------------------------------------------------------------------ //
le::IMesh* le::Sprite::GetMesh() const
{
	return mesh;
}

// ------------------------------------------------------------------------------------ //
// Get size
// ------------------------------------------------------------------------------------ //
const le::Vector2D_t& le::Sprite::GetSize() const
{
    return size;
}

// ------------------------------------------------------------------------------------ //
// Get type
// ------------------------------------------------------------------------------------ //
le::SPRITE_TYPE le::Sprite::GetType() const
{
    return type;
}

// ------------------------------------------------------------------------------------ //
// Move model
// ------------------------------------------------------------------------------------ //
void le::Sprite::Move( const Vector3D_t& FactorMove )
{
	position += FactorMove;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Rotate sprite
// ------------------------------------------------------------------------------------ //
void le::Sprite::Rotate( const Vector3D_t& FactorRotate )
{
	Vector3D_t			axis( sin( FactorRotate.x / 2.f ), sin( FactorRotate.y / 2.f ), sin( FactorRotate.z / 2.f ) );
	Vector3D_t			rotations( cos( FactorRotate.x / 2.f ), cos( FactorRotate.y / 2.f ), cos( FactorRotate.z / 2.f ) );

	rotation *= Quaternion_t( rotations.x, axis.x, 0, 0 ) * 
		Quaternion_t( rotations.y, 0, axis.y, 0 ) * 
		Quaternion_t( rotations.z, 0, 0, axis.z );

	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Rotate sprite
// ------------------------------------------------------------------------------------ //
void le::Sprite::Rotate( const Quaternion_t& FactorRotate )
{
	rotation *= FactorRotate;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Scale sprite
// ------------------------------------------------------------------------------------ //
void le::Sprite::Scale( const Vector3D_t& FactorScale )
{
	scale += FactorScale;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Set position sprite
// ------------------------------------------------------------------------------------ //
void le::Sprite::SetPosition( const Vector3D_t& Position )
{
	position = Position;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Set rotation sprite
// ------------------------------------------------------------------------------------ //
void le::Sprite::SetRotation( const Vector3D_t& Rotation )
{
	Vector3D_t			axis( sin( Rotation.x / 2.f ), sin( Rotation.y / 2.f ), sin( Rotation.z / 2.f ) );
	Vector3D_t			rotations( cos( Rotation.x / 2.f ), cos( Rotation.y / 2.f ), cos( Rotation.z / 2.f ) );

	rotation = Quaternion_t( rotations.x, axis.x, 0, 0 ) *
		Quaternion_t( rotations.y, 0, axis.y, 0 ) *
		Quaternion_t( rotations.z, 0, 0, axis.z );

	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Set rotation sprite
// ------------------------------------------------------------------------------------ //
void le::Sprite::SetRotation( const Quaternion_t& Rotation )
{
	rotation = Rotation;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Set scale sprite
// ------------------------------------------------------------------------------------ //
void le::Sprite::SetScale( const Vector3D_t& Scale )
{
	scale = Scale;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Sprite::GetPosition() const
{
	return position;
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
const le::Quaternion_t& le::Sprite::GetRotation() const
{
	return rotation;
}

// ------------------------------------------------------------------------------------ //
// Get scale
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Sprite::GetScale() const
{
	return scale;
}

// ------------------------------------------------------------------------------------ //
// Get transformation
// ------------------------------------------------------------------------------------ //
const le::Matrix4x4_t& le::Sprite::GetTransformation()
{
	if ( isNeedUpdateTransformation )	
		UpdateTransformation();

	return transformation;
}

// ------------------------------------------------------------------------------------ //
// Получить минимальную вершину
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Sprite::GetMin()
{
    if ( isNeedUpdateBoundingBox )
        UpdateBoundingBox();

    return min;
}

// ------------------------------------------------------------------------------------ //
// Получить максимальную вершину
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Sprite::GetMax()
{
    if ( isNeedUpdateBoundingBox )
        UpdateBoundingBox();

    return max;
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Sprite::IncrementReference()
{
    ++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Sprite::DecrementReference()
{
    --countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::Sprite::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Sprite::GetCountReferences() const
{
    return countReferences;
}
