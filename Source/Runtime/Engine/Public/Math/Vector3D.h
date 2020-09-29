// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"

namespace le
{
	struct SVector3D
	{
		/* Constructor */
		SVector3D() :
			x( 0.f ),
			y( 0.f ),
			z( 0.f )
		{}

		/* Constructor */
		SVector3D( float InX, float InY, float InZ ) :
			x( InX ),
			y( InY ),
			z( InZ )
		{}

		/* Constructor for copy */
		SVector3D( const SVector3D& InCopy ) :
			x( InCopy.x ),
			y( InCopy.y ),
			z( InCopy.z )
		{}

		/* Calculate the dot product of two vectors */
		FORCEINLINE static float DotProduct( const SVector3D& InLeft, const SVector3D InRight )				{ return InLeft | InRight; }

		/* Calculate the cross product of two vectors */
		FORCEINLINE static SVector3D CrossProduct( const SVector3D& InLeft, const SVector3D InRight )		{ return InLeft ^ InRight; }

		/* Distance between two 3D points */
		FORCEINLINE static float Distance( const SVector3D& InLeft, const SVector3D InRight )				{ return sqrtf( powf( InRight.x - InLeft.x, 2.f ) + powf( InRight.y - InLeft.y, 2.f ) + powf( InRight.z - InLeft.z, 2.f ) ); }

		/* Returns a vector with the maximum component for each dimension from the pair of vectors */
		FORCEINLINE static const SVector3D& Max( const SVector3D& InLeft, const SVector3D InRight )
		{
			if ( InLeft > InRight )		return InLeft;
			return InRight;
		}

		/* Returns a vector with the minimum component for each dimension from the pair of vectors */
		FORCEINLINE static const SVector3D& Min( const SVector3D& InLeft, const SVector3D InRight )
		{
			if ( InLeft < InRight )		return InLeft;
			return InRight;
		}

		/* Normalize */
		FORCEINLINE void Normalize()
		{
			float		squareSum = x * x + y * y + z * z;
			if ( squareSum == 0.f )		squareSum = 1.f;

			x /= squareSum;
			y /= squareSum;
			z /= squareSum;
		}

		/* Set values */
		FORCEINLINE void Set( float InX, float InY, float InZ )
		{
			x = InX;
			y = InY;
			z = InZ;
		}

		/* Get size */
		FORCEINLINE float GetSize() const									{ return sqrtf( x * x + y * y + z * z ); }

		/* Operator + */
		FORCEINLINE SVector3D operator+( const SVector3D& InRight )	const	{ return SVector3D( x + InRight.x, y + InRight.y, z + InRight.z ); }
		FORCEINLINE SVector3D operator+( float InRight ) const				{ return SVector3D( x + InRight, y + InRight, z + InRight ); }

		/* Operator += */
		FORCEINLINE const SVector3D&		operator+=( const SVector3D& InRight ) 
		{ 
			x += InRight.x;
			y += InRight.y;
			z += InRight.z;
			return *this;
		}

		FORCEINLINE const SVector3D&		operator+=( float InRight )
		{
			x += InRight;
			y += InRight;
			z += InRight;
			return *this;
		}

		/* Operator - */
		FORCEINLINE SVector3D operator-( const SVector3D& InRight )	const		{ return SVector3D( x - InRight.x, y - InRight.y, z - InRight.z ); }
		FORCEINLINE SVector3D operator-( float InRight ) const					{ return SVector3D( x - InRight, y - InRight, z - InRight ); }
		FORCEINLINE SVector3D operator-() const									{ return SVector3D( -x, -y, -z ); }

		/* Operator -= */
		FORCEINLINE const SVector3D& operator-=( const SVector3D& InRight )
		{
			x -= InRight.x;
			y -= InRight.y;
			z -= InRight.z;
			return *this;
		}

		FORCEINLINE const SVector3D& operator-=( float InRight )
		{
			x -= InRight;
			y -= InRight;
			z -= InRight;
			return *this;
		}

		/* Operator * */
		FORCEINLINE SVector3D operator*( const SVector3D& InRight )	const		{ return SVector3D( x * InRight.x, y * InRight.y, z * InRight.z ); }
		FORCEINLINE SVector3D operator*( float InRight ) const					{ return SVector3D( x * InRight, y * InRight, z * InRight ); }

		/* Operator *= */
		FORCEINLINE const SVector3D& operator*=( const SVector3D& InRight )
		{
			x *= InRight.x;
			y *= InRight.y;
			z *= InRight.z;
			return *this;
		}

		FORCEINLINE const SVector3D& operator*=( float InRight )
		{
			x *= InRight;
			y *= InRight;
			z *= InRight;
			return *this;
		}

		/* Operator / */
		FORCEINLINE SVector3D operator/( const SVector3D& InRight ) const		{ return SVector3D( x / InRight.x, y / InRight.y, z / InRight.z ); }
		FORCEINLINE SVector3D operator/( float InRight ) const					{ return SVector3D( x / InRight, y / InRight, z / InRight ); }

		/* Operator /= */
		FORCEINLINE const SVector3D& operator/=( const SVector3D& InRight )
		{
			x /= InRight.x;
			y /= InRight.y;
			z /= InRight.z;
			return *this;
		}

		FORCEINLINE const SVector3D& operator/=( float InRight )
		{
			x /= InRight;
			y /= InRight;
			z /= InRight;
			return *this;
		}

		/* Calculates dot product of this vector and another */
		FORCEINLINE float operator|( const SVector3D& InRight ) const			{ return x * InRight.x + y * InRight.y + z * InRight.z; }

		/* Calculates cross product of this vector and another */
		FORCEINLINE SVector3D operator^( const SVector3D& InRight )	const		{ return SVector3D( y * InRight.z - z * InRight.y, z * InRight.x - x * InRight.z, x * InRight.y - y * InRight.x ); }

		/* Operator == */
		FORCEINLINE bool operator==( const SVector3D& InRight )	const			{ return x == InRight.x && y == InRight.y && z == InRight.z; }

		/* Operator != */
		FORCEINLINE bool operator!=( const SVector3D& InRight ) const			{ return x != InRight.x || y != InRight.y || z != InRight.z; }

		/* Operator < */
		FORCEINLINE bool operator<( const SVector3D& InRight ) const			{ return x < InRight.x && y < InRight.y && z < InRight.z; }
		
		/* Operator <= */
		FORCEINLINE bool operator<=( const SVector3D& InRight )	const			{ return x <= InRight.x && y <= InRight.y && z <= InRight.z; }

		/* Operator > */
		FORCEINLINE bool operator>( const SVector3D& InRight ) const			{ return x > InRight.x && y > InRight.y && z > InRight.z; }
		
		/* Operator > */
		FORCEINLINE bool operator>=( const SVector3D& InRight )	const			{ return x >= InRight.x && y >= InRight.y && z >= InRight.z; }

		/* Operator [] */
		FORCEINLINE float& operator[]( uint32 InIndex )
		{
			LIFEENGINE_ASSERT( InIndex < 3 );
			return ( &x )[ InIndex ];
		}
		FORCEINLINE float operator[]( uint32 InIndex ) const
		{
			LIFEENGINE_ASSERT( InIndex < 3 );
			return ( &x )[ InIndex ];
		}

		float		x;
		float		y;
		float		z;
	};
}

#endif // !VECTOR3D_H
