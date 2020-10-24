// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef MATH_H
#define MATH_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Color.h"
#include "Rect.h"

namespace le
{
	typedef		glm::vec2		FVector2D;
	typedef		glm::vec3		FVector3D;
	typedef		glm::vec4		FVector4D;
	typedef		glm::quat		FQuaternion;
	typedef		glm::mat4		FMatrix4x4;

	/* Converting from cartesian to isometric coordinates */
	FORCEINLINE FVector2D MathCartesianToIsometric( float InCartesianX, float InCartesianY )											{ return FVector2D( InCartesianX - InCartesianY, ( InCartesianX + InCartesianY ) / 2.f ); }
	FORCEINLINE FVector3D MathCartesianToIsometric( float InCartesianX, float InCartesianY, float InCartesianZ )						{ return FVector3D( MathCartesianToIsometric( InCartesianX, InCartesianY ), InCartesianZ ); }
	FORCEINLINE FVector4D MathCartesianToIsometric( float InCartesianX, float InCartesianY, float InCartesianZ, float InCartesianW )	{ return FVector4D( MathCartesianToIsometric( InCartesianX, InCartesianY ), InCartesianZ, InCartesianW ); }
	FORCEINLINE FVector2D MathCartesianToIsometric( const FVector2D& InCartesianCoords )												{ return MathCartesianToIsometric( InCartesianCoords.x, InCartesianCoords.y ); }
	FORCEINLINE FVector3D MathCartesianToIsometric( const FVector3D& InCartesianCoords )												{ return MathCartesianToIsometric(InCartesianCoords.x, InCartesianCoords.y, InCartesianCoords.z ); }
	FORCEINLINE FVector4D MathCartesianToIsometric( const FVector4D& InCartesianCoords )												{ return MathCartesianToIsometric( InCartesianCoords.x, InCartesianCoords.y, InCartesianCoords.z, InCartesianCoords.w ); }

	/* Converting from isometric to cartesian coordinates */
	FORCEINLINE FVector2D MathIsometricToCartesian( float InIsometricX, float InIsometricY )											{ return FVector2D( ( 2.f * InIsometricY + InIsometricX ) / 2.f, ( 2.f * InIsometricY - InIsometricX ) / 2.f ); }
	FORCEINLINE FVector3D MathIsometricToCartesian( float InIsometricX, float InIsometricY, float InIsometricZ )						{ return FVector3D( MathIsometricToCartesian( InIsometricX, InIsometricY ), InIsometricZ ); }
	FORCEINLINE FVector4D MathIsometricToCartesian( float InIsometricX, float InIsometricY, float InIsometricZ, float InIsometricW )	{ return FVector4D( MathIsometricToCartesian( InIsometricX, InIsometricY ), InIsometricZ, InIsometricW ); }
	FORCEINLINE FVector2D MathIsometricToCartesian( const FVector2D& InIsometricCoords )												{ return MathIsometricToCartesian( InIsometricCoords.x, InIsometricCoords.y ); }
	FORCEINLINE FVector3D MathIsometricToCartesian( const FVector3D& InIsometricCoords )												{ return MathIsometricToCartesian( InIsometricCoords.x, InIsometricCoords.y, InIsometricCoords.z ); }
	FORCEINLINE FVector4D MathIsometricToCartesian( const FVector4D& InIsometricCoords )												{ return MathIsometricToCartesian( InIsometricCoords.x, InIsometricCoords.y, InIsometricCoords.z, InIsometricCoords.w ); }

	/* Converting Euler angles to Quaternion */
	FORCEINLINE FQuaternion MathEulerAnglesToQuaternion( float InEulerAngleX, float InEulerAngleY, float InEulerAngleZ )				{ return glm::angleAxis( InEulerAngleX, FVector3D( 1.f, 0.f, 0.f ) ) * glm::angleAxis( InEulerAngleY, FVector3D( 0.f, 1.f, 0.f ) ) * glm::angleAxis( InEulerAngleZ, FVector3D( 0.f, 0.f, 1.f ) ); }
	FORCEINLINE FQuaternion MathEulerAnglesToQuaternion( const FVector3D& InEulerAngles )												{ return MathEulerAnglesToQuaternion( InEulerAngles.x, InEulerAngles.y, InEulerAngles.z ); }
}

#endif // !MATH_H
