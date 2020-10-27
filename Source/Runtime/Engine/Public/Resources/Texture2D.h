// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "Misc/Types.h"
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
		FORCEINLINE FIRHITexture2DConstRef& GetHandle() const
		{
			return handle;
		}

	private:
		FIRHITexture2DRef		handle;
	};
}

#endif // !TEXTURE_2D_H
