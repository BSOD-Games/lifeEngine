// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"
#include "Misc/Path.h"
#include "Misc/Object.h"
#include "Misc/RefCounted.h"
#include "System/EventChannel.h"

#include "Logging/LogMacros.h"

namespace le
{
	enum EResourceType
	{
		RT_Texture2D,
		RT_Config,
		RT_Material,
		RT_World
	};

	class Resource : public Object, public RefCounted
	{
	public:
		struct EventDelete
		{
			Resource*		resource;
		};

		/* Destructor */
		virtual ~Resource() 
		{
			eventDelete.Emit( EventDelete{ this } );
		}

		/* Constructor */
		Resource() {}

		/* Constructor of copy */
		Resource( const Resource& InCopy ) :
			name( InCopy.name )
		{}

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

		/* Get channel event 'Delete' */
		FORCEINLINE TEventChannel< EventDelete >& GetEventChannelDelete()		{ return eventDelete; }

	protected:
		TEventChannel< EventDelete >		eventDelete;
		std::string							name;
	};
}

#endif // !RESOURCE_H
