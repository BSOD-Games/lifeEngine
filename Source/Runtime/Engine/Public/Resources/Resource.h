// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"
#include "Misc/Path.h"
#include "Misc/Object.h"

namespace le
{
	enum EResourceType
	{
		RT_Texture2D,
		RT_Config
	};

	class Resource : public Object
	{
	public:
		/* Destructor */
		virtual ~Resource() {}

		/* Serialize resource */
		virtual bool Serialize( const Path& InPath ) = 0;

		/* Deserialize resource */
		virtual bool Deserialize( const Path& InPath ) = 0;

		/* Set name */
		void SetName( const std::string& InName )
		{
			name = InName;
		}

		/* Get type */
		virtual EResourceType GetType() const = 0;

		/* Get name */
		FORCEINLINE const std::string& GetName() const
		{
			return name;
		}

	protected:
		std::string			name;
	};
}

#endif // !RESOURCE_H
