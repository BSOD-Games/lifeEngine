// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Math/Math.h"
#include "World/Components/BaseComponent.h"

namespace le
{
	class TransformComponent : public BaseComponent
	{
	public:
		DECLARE_COMPONENT( TransformComponent )

		/* Constructor */
		TransformComponent();

		/* Destructor */
		~TransformComponent();

		/* Move */
		FORCEINLINE void Move( const FVector3D& InFactorMove )
		{
			isNeedUpdate = true;
			position += InFactorMove;
		}

		/* Rotate */
		FORCEINLINE void Rotate( const FQuaternion& InFactorRotate )
		{
			isNeedUpdate = true;
			rotation *= InFactorRotate;
		}

		/* Scale */
		FORCEINLINE void Scale( const FVector3D& InFactorScale )
		{
			isNeedUpdate = true;
			scale += InFactorScale;
		}

		/* Set owner */
		FORCEINLINE void SetOwner( TransformComponent* InOwner )		
		{ 
			if ( owner )		owner->ReleaseRef();

			isNeedUpdate = true;
			owner = InOwner; 

			if ( InOwner )		InOwner->AddRef();
		}

		/* Set position */
		FORCEINLINE void SetPosition( const FVector3D& InPosition ) 
		{ 
			isNeedUpdate = true; 
			position = InPosition;
		}

		/* Set rotation */
		FORCEINLINE void SetRotation( const FQuaternion& InRotation )
		{
			isNeedUpdate = true;
			rotation = InRotation;
		}

		/* Set scale */
		FORCEINLINE void SetScale( const FVector3D& InScale )
		{
			isNeedUpdate = true;
			scale = InScale;
		}

		/* Get owner */
		FORCEINLINE TransformComponent* GetOwner() const				{ return owner; }

		/* Get local position */
		FORCEINLINE const FVector3D& GetLocalPosition() const			{ return position; }

		/* Get global position */
		FORCEINLINE FVector3D GetGlobalPosition() const
		{
			if ( !owner )		return GetLocalPosition();
			return owner->position + position;
		}

		/* Get local rotation */
		FORCEINLINE const FQuaternion& GetLocalRotation() const			{ return rotation; }

		/* Get global rotation */
		FORCEINLINE FQuaternion GetGlobalRotation() const
		{
			if ( !owner )		return GetLocalRotation();
			return owner->rotation * rotation;
		}

		/* Get local scale */
		FORCEINLINE const FVector3D& GetLocalScale() const				{ return scale; }

		/* Get global scale */
		FORCEINLINE FVector3D GetGlobalScale() const
		{
			if ( !owner )		return GetLocalScale();
			return owner->scale * scale;
		}

		/* Get local matrix transformation */
		FORCEINLINE const FMatrix4x4& GetLocalMatrixTransformation() const
		{
			if ( isNeedUpdate )			UpdateMatrix();
			return transformation;
		}

		/* Get global matrix transformation */
		FORCEINLINE FMatrix4x4 GetGlobalMatrixTransformation() const
		{
			if ( !owner )		return GetGlobalMatrixTransformation();
			return owner->transformation * transformation;
		}

	private:
		/* Update matrix transformation */
		void UpdateMatrix() const;

		mutable bool			isNeedUpdate;

		FVector3D				position;
		FQuaternion				rotation;
		FVector3D				scale;
		mutable FMatrix4x4		transformation;

		TransformComponent*		owner;
	};
}

#endif // !TRANSFORMCOMPONENT_H
