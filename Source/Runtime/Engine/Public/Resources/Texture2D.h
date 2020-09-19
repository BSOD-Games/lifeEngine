// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "Resources/Resource.h"

namespace le
{
	class Texture2D : public Resource
	{
	public:
		/* Constructor */
		Texture2D();

		/* Destructor */
		~Texture2D();

		/* Serialize resource */
		bool Serialize( const std::string& InPath ) override;

		/* Deserialize resource */
		bool Deserialize( const std::string& InPath ) override;

		/* Get resource type */
		EResourceType GetType() const override;
	};
}

#endif // !TEXTURE_2D_H
