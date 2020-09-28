// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"
#include "Math/Vector2D.h"
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

	class SpriteComponent : public Object
	{
	public:
		/* Constructor */
		SpriteComponent();

		/* Destructor */
		~SpriteComponent();

		/* Render */
		void Render();

		/* Set type */
		FORCEINLINE void SetType( ESpriteType InType )			{ type = InType; }

		/* Set material */
		FORCEINLINE void SetMaterial( Material* InMaterial )
		{
			if ( material )			material->ReleaseRef();

			material = InMaterial;
			if ( InMaterial )		InMaterial->AddRef();
		}

		/* Set size */
		FORCEINLINE void SetSize( const SVector2D& InSize )		{ size = InSize; }

		/* Get type */
		FORCEINLINE ESpriteType GetType() const					{ return type; }

		/* Get material */
		FORCEINLINE Material* GetMaterial() const				{ return material; }

		/* Get size */
		FORCEINLINE const SVector2D& GetSize() const			{ return size; }

	private:
		ESpriteType		type;
		Material*		material;
		SVector2D		size;
	};
}

#endif // !SPRITECOMPONENT_H
