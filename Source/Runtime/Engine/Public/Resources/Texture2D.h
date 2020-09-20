// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "Resources/Resource.h"

namespace le
{
	class IRHITexture2D;

	class Texture2D : public Resource
	{
	public:
		/* Constructor */
		Texture2D();

		/* Destructor */
		~Texture2D();

		/* Serialize resource */
		bool Serialize( const Path& InPath ) override;

		/* Deserialize resource */
		bool Deserialize( const Path& InPath ) override;

		/* Get resource type */
		EResourceType GetType() const override;

		/* Get handle */
		FORCEINLINE IRHITexture2D* GetHandle() const
		{
			return handle;
		}

	private:
		IRHITexture2D*			handle;
	};
}

#endif // !TEXTURE_2D_H
