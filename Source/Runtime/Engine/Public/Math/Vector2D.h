// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"

namespace le
{
	struct SVector2D
	{
		/* Constructor */
		SVector2D() :
			x( 0.f ),
			y( 0.f )
		{}

		/* Constructor */
		SVector2D( float InX, float InY ) :
			x( InX ),
			y( InY )
		{}

		/* Constructor for copy */
		SVector2D( const SVector2D& InCopy ) :
			x( InCopy.x ),
			y( InCopy.y )
		{}

		/* Calculate the dot product of two vectors */
		FORCEINLINE static float DotProduct( const SVector2D& InLeft, const SVector2D InRight )
		{
			return InLeft | InRight;
		}

		/* Calculate the cross product of two vectors */
		FORCEINLINE static float CrossProduct( const SVector2D& InLeft, const SVector2D InRight )
		{
			return InLeft ^ InRight;
		}

		/* Distance between two 2D points */
		FORCEINLINE static float Distance( const SVector2D& InLeft, const SVector2D InRight )
		{}

		/* Returns a vector with the maximum component for each dimension from the pair of vectors */
		FORCEINLINE static const SVector2D& Max( const SVector2D& InLeft, const SVector2D InRight )
		{
			if ( InLeft > InRight )		return InLeft;
			return InRight;
		}

		/* Returns a vector with the minimum component for each dimension from the pair of vectors */
		FORCEINLINE static const SVector2D& Min( const SVector2D& InLeft, const SVector2D InRight )
		{
			if ( InLeft < InRight )		return InLeft;
			return InRight;
		}

		/* Normalize */
		FORCEINLINE void Normalize()
		{
			// TODO: Implement this
		}

		/* Set values */
		FORCEINLINE void Set( float InX, float InY )
		{
			x = InX;
			y = InY;
		}

		/* Get size */
		FORCEINLINE float GetSize() const
		{
			return static_cast< float >( sqrt( x * x + y * y ) );
		}

		/* Operator + */
		FORCEINLINE SVector2D operator+( const SVector2D& InRight )	const	{ return SVector2D( x + InRight.x, y + InRight.y ); }
		FORCEINLINE SVector2D operator+( float InRight ) const				{ return SVector2D( x + InRight, y + InRight ); }

		/* Operator += */
		FORCEINLINE const SVector2D&		operator+=( const SVector2D& InRight ) 
		{ 
			x += InRight.x;
			y += InRight.y;
			return *this;
		}

		FORCEINLINE const SVector2D&		operator+=( float InRight )
		{
			x += InRight;
			y += InRight;
			return *this;
		}

		/* Operator - */
		FORCEINLINE SVector2D operator-( const SVector2D& InRight )	const		{ return SVector2D( x - InRight.x, y - InRight.y ); }
		FORCEINLINE SVector2D operator-( float InRight ) const					{ return SVector2D( x - InRight, y - InRight ); }
		FORCEINLINE SVector2D operator-() const									{ return SVector2D( -x, -y ); }

		/* Operator -= */
		FORCEINLINE const SVector2D& operator-=( const SVector2D& InRight )
		{
			x -= InRight.x;
			y -= InRight.y;
			return *this;
		}

		FORCEINLINE const SVector2D& operator-=( float InRight )
		{
			x -= InRight;
			y -= InRight;
			return *this;
		}

		/* Operator * */
		FORCEINLINE SVector2D operator*( const SVector2D& InRight )	const		{ return SVector2D( x * InRight.x, y * InRight.y ); }
		FORCEINLINE SVector2D operator*( float InRight ) const					{ return SVector2D( x * InRight, y * InRight ); }

		/* Operator *= */
		FORCEINLINE const SVector2D& operator*=( const SVector2D& InRight )
		{
			x *= InRight.x;
			y *= InRight.y;
			return *this;
		}

		FORCEINLINE const SVector2D& operator*=( float InRight )
		{
			x *= InRight;
			y *= InRight;
			return *this;
		}

		/* Operator / */
		FORCEINLINE SVector2D operator/( const SVector2D& InRight ) const		{ return SVector2D( x / InRight.x, y / InRight.y ); }
		FORCEINLINE SVector2D operator/( float InRight ) const					{ return SVector2D( x / InRight, y / InRight );	}

		/* Operator /= */
		FORCEINLINE const SVector2D& operator/=( const SVector2D& InRight )
		{
			x /= InRight.x;
			y /= InRight.y;
			return *this;
		}

		FORCEINLINE const SVector2D& operator/=( float InRight )
		{
			x /= InRight;
			y /= InRight;
			return *this;
		}

		/* Calculates dot product of this vector and another */
		FORCEINLINE float operator|( const SVector2D& InRight ) const			{ return x * InRight.x + y * InRight.y; }

		/* Calculates cross product of this vector and another */
		FORCEINLINE float operator^( const SVector2D& InRight )	const			{ return x * InRight.y - y * InRight.x; }

		/* Operator == */
		FORCEINLINE bool operator==( const SVector2D& InRight )	const			{ return x == InRight.x && y == InRight.y; }

		/* Operator != */
		FORCEINLINE bool operator!=( const SVector2D& InRight ) const			{ return x != InRight.x || y != InRight.y; }

		/* Operator < */
		FORCEINLINE bool operator<( const SVector2D& InRight ) const			{ return x < InRight.x && y < InRight.y; }
		
		/* Operator <= */
		FORCEINLINE bool operator<=( const SVector2D& InRight )	const			{ return x <= InRight.x && y <= InRight.y; }

		/* Operator > */
		FORCEINLINE bool operator>( const SVector2D& InRight ) const			{ return x > InRight.x && y > InRight.y; }
		
		/* Operator > */
		FORCEINLINE bool operator>=( const SVector2D& InRight )	const			{ return x >= InRight.x && y >= InRight.y; }

		/* Operator [] */
		FORCEINLINE float& operator[]( uint32 InIndex )
		{
			LIFEENGINE_ASSERT( InIndex < 2 );
			return InIndex == 0 ? x : y;
		}
		FORCEINLINE float operator[]( uint32 InIndex ) const
		{
			LIFEENGINE_ASSERT( InIndex < 2 );
			return InIndex == 0 ? x : y;
		}

		float		x;
		float		y;
	};
}

#endif // !VECTOR2D_H
