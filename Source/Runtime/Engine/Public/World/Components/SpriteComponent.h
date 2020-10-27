// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Misc/EngineDefines.h"
#include "World/Components/BaseComponent.h"
#include "World/Components/TransformComponent.h"
#include "Resources/Material.h"

namespace le
{
	class Material;

	enum ESpriteType
	{
		ST_Static,
		ST_Rotating,
		ST_RotatingOnlyVertical
	};

	class SpriteComponent : public BaseComponent
	{
	public:
		DECLARE_COMPONENT( SpriteComponent )

		/* Constructor */
		SpriteComponent();

		/* Destructor */
		~SpriteComponent();

		/* Render */
		void Render() const;

		/* Set type */
		FORCEINLINE void SetType( ESpriteType InType )							{ type = InType; }

		/* Set material */
		FORCEINLINE void SetMaterial( FMaterialConstRef& InMaterial )			{ material = InMaterial; }

		/* Set size */
		FORCEINLINE void SetSize( const FVector2D& InSize )						{ size = InSize; }

		/* Set texture rect. He must be from 0 to 1 in dependencies texture size */
		FORCEINLINE void SetTextureRect( const FSRectFloat& InTextureRect )		{ textureRect = InTextureRect; }

		/* Get type */
		FORCEINLINE ESpriteType GetType() const									{ return type; }

		/* Get material */
		FORCEINLINE FMaterialRef GetMaterial() const							{ return material; }

		/* Get size */
		FORCEINLINE const FVector2D& GetSize() const							{ return size; }

		/* Get texture rect */
		FORCEINLINE const FSRectFloat& GetTextureRect() const					{ return textureRect; }

		/* Get transformation component */
		FORCEINLINE FTransformComponentConstRef& GetTransformComponent() const		{ return transformComponent; }
		FORCEINLINE FTransformComponentRef& GetTransformComponent()					{ return transformComponent; }

	private:
		ESpriteType					type;
		FSRectFloat					textureRect;
		FMaterialRef				material;
		FVector2D					size;
		FTransformComponentRef		transformComponent;
	};
}

#endif // !SPRITECOMPONENT_H
