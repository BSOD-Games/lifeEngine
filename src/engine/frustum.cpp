//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "frustum.h"

enum FRUSTUM_SIDE
{
	FS_RIGHT,
	FS_LEFT,
	FS_BOTTOM,
	FS_TOP,
	FS_BACK,
	FS_FRONT
};

// ------------------------------------------------------------------------------------ //
// Обновить усеченую пирамиду видимости
// ------------------------------------------------------------------------------------ //
void le::Frustum::Update( const Matrix4x4_t& Projection, const Matrix4x4_t& View )
{
	Matrix4x4_t				clippingPlane = Projection * View;

	planes[ FS_RIGHT ].x = clippingPlane[ 0 ].w - clippingPlane[ 0 ].x;
	planes[ FS_RIGHT ].y = clippingPlane[ 1 ].w - clippingPlane[ 1 ].x;
	planes[ FS_RIGHT ].z = clippingPlane[ 2 ].w - clippingPlane[ 2 ].x;
	planes[ FS_RIGHT ].w = clippingPlane[ 3 ].w - clippingPlane[ 3 ].x;
	
	planes[ FS_LEFT ].x = clippingPlane[ 0 ].w + clippingPlane[ 0 ].x;
	planes[ FS_LEFT ].y = clippingPlane[ 1 ].w + clippingPlane[ 1 ].x;
	planes[ FS_LEFT ].z = clippingPlane[ 2 ].w + clippingPlane[ 2 ].x;
	planes[ FS_LEFT ].w = clippingPlane[ 3 ].w + clippingPlane[ 3 ].x;
	
	planes[ FS_BOTTOM ].x = clippingPlane[ 0 ].w + clippingPlane[ 0 ].y;
	planes[ FS_BOTTOM ].y = clippingPlane[ 1 ].w + clippingPlane[ 1 ].y;
	planes[ FS_BOTTOM ].z = clippingPlane[ 2 ].w + clippingPlane[ 2 ].y;
	planes[ FS_BOTTOM ].w = clippingPlane[ 3 ].w + clippingPlane[ 3 ].y;
	
	planes[ FS_TOP ].x = clippingPlane[ 0 ].w - clippingPlane[ 0 ].y;
	planes[ FS_TOP ].y = clippingPlane[ 1 ].w - clippingPlane[ 1 ].y;
	planes[ FS_TOP ].z = clippingPlane[ 2 ].w - clippingPlane[ 2 ].y;
	planes[ FS_TOP ].w = clippingPlane[ 3 ].w - clippingPlane[ 3 ].y;
	
	planes[ FS_BACK ].x = clippingPlane[ 0 ].w + clippingPlane[ 0 ].z;
	planes[ FS_BACK ].y = clippingPlane[ 1 ].w + clippingPlane[ 1 ].z;
	planes[ FS_BACK ].z = clippingPlane[ 2 ].w + clippingPlane[ 2 ].z;
	planes[ FS_BACK ].w = clippingPlane[ 3 ].w + clippingPlane[ 3 ].z;
	
	planes[ FS_FRONT ].x = clippingPlane[ 0 ].w - clippingPlane[ 0 ].z;
	planes[ FS_FRONT ].y = clippingPlane[ 1 ].w - clippingPlane[ 1 ].z;
	planes[ FS_FRONT ].z = clippingPlane[ 2 ].w - clippingPlane[ 2 ].z;
	planes[ FS_FRONT ].w = clippingPlane[ 3 ].w - clippingPlane[ 3 ].z;

	NormalizePlanes();
}

// ------------------------------------------------------------------------------------ //
// Попал ли паралеллепипед в фокус камеры
// ------------------------------------------------------------------------------------ //
bool le::Frustum::IsVisible( const Vector3D_t& MinPosition, const Vector3D_t& MaxPosition ) const
{
	for ( UInt32_t index = 0; index < 6; ++index )
	{
		if ( planes[ index ].x * MinPosition.x + planes[ index ].y * MinPosition.y + planes[ index ].z * MinPosition.z + planes[ index ].w > 0 )  continue;
		if ( planes[ index ].x * MaxPosition.x + planes[ index ].y * MinPosition.y + planes[ index ].z * MinPosition.z + planes[ index ].w > 0 )  continue;
		if ( planes[ index ].x * MaxPosition.x + planes[ index ].y * MaxPosition.y + planes[ index ].z * MinPosition.z + planes[ index ].w > 0 )  continue;
		if ( planes[ index ].x * MinPosition.x + planes[ index ].y * MaxPosition.y + planes[ index ].z * MinPosition.z + planes[ index ].w > 0 )  continue;
		if ( planes[ index ].x * MinPosition.x + planes[ index ].y * MinPosition.y + planes[ index ].z * MaxPosition.z + planes[ index ].w > 0 )  continue;
		if ( planes[ index ].x * MaxPosition.x + planes[ index ].y * MinPosition.y + planes[ index ].z * MaxPosition.z + planes[ index ].w > 0 )  continue;
		if ( planes[ index ].x * MaxPosition.x + planes[ index ].y * MaxPosition.y + planes[ index ].z * MaxPosition.z + planes[ index ].w > 0 )  continue;
		if ( planes[ index ].x * MinPosition.x + planes[ index ].y * MaxPosition.y + planes[ index ].z * MaxPosition.z + planes[ index ].w > 0 )  continue;

		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Попал ли паралеллепипед в фокус камеры
// ------------------------------------------------------------------------------------ //
bool le::Frustum::IsVisible( const Vector3DInt_t& MinPosition, const Vector3DInt_t& MaxPosition ) const
{
	return IsVisible( ( Vector3D_t ) MinPosition, ( Vector3D_t ) MaxPosition );
}

// ------------------------------------------------------------------------------------ //
// Попала ли сфера в фокус камеры
// ------------------------------------------------------------------------------------ //
bool le::Frustum::IsVisible( const Vector3DInt_t& Position, float Radius ) const
{
	for ( UInt32_t index = 0; index < 6; ++index )
		if ( planes[ index ].x * Position.x + planes[ index ].y * Position.y +
			 planes[ index ].z * Position.z + planes[ index ].w <= -Radius )
			return false;

	return true;
}

// ------------------------------------------------------------------------------------ //
// Нормализовать плоскости усеченой пирамиды видимости
// ------------------------------------------------------------------------------------ //
void le::Frustum::NormalizePlanes()
{
	for ( UInt32_t side = 0; side < 6; ++side )
	{
		float		magnitude = sqrt( planes[ side ].x * planes[ side ].x +
									  planes[ side ].y * planes[ side ].y +
									  planes[ side ].z * planes[ side ].z );

		planes[ side ].x /= magnitude;
		planes[ side ].y /= magnitude;
		planes[ side ].z /= magnitude;
		planes[ side ].w /= magnitude;
	}
}
