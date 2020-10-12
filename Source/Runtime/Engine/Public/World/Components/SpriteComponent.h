// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Misc/EngineDefines.h"
#include "World/Components/BaseComponent.h"
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
		/* Constructor */
		SpriteComponent();

		/* Destructor */
		~SpriteComponent();

		/* Render */
		void Render();

		/* Set type */
		FORCEINLINE void SetType( ESpriteType InType )							{ type = InType; }

		/* Set material */
		FORCEINLINE void SetMaterial( Material* InMaterial )
		{
			if ( material )			material->ReleaseRef();

			material = InMaterial;
			if ( InMaterial )		InMaterial->AddRef();
		}

		/* Set size */
		FORCEINLINE void SetSize( const FVector2D& InSize )						{ size = InSize; }

		/* Set texture rect. He must be from 0 to 1 in dependencies texture size */
		FORCEINLINE void SetTextureRect( const FSRectFloat& InTextureRect )		{ textureRect = InTextureRect; }

		/* Get type */
		FORCEINLINE ESpriteType GetType() const									{ return type; }

		/* Get material */
		FORCEINLINE Material* GetMaterial() const								{ return material; }

		/* Get size */
		FORCEINLINE const FVector2D& GetSize() const							{ return size; }

		/* Get texture rect */
		FORCEINLINE const FSRectFloat& GetTextureRect() const					{ return textureRect; }

	private:
		ESpriteType		type;
		FSRectFloat		textureRect;
		Material*		material;
		FVector2D		size;
	};
}

#endif // !SPRITECOMPONENT_H
