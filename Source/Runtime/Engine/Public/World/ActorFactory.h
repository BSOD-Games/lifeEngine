// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef ACTORFACTORY_H
#define ACTORFACTORY_H

#include <string>
#include <unordered_map>

#include "Misc/EngineGlobals.h"
#include "Misc/Object.h"
#include "Misc/Types.h"

namespace le
{
	typedef FActorRef		( *FCreateActorFn )( );

	class ActorFactory : public Object
	{
	public:
		/* Register actor */
		void Register( const std::string& InName, FCreateActorFn InCreateActorFunction );

		/* Create actor */
		FActorRef Create( const std::string& InName ) const;

	private:
		std::unordered_map< std::string, FCreateActorFn >		actors;
	};
}

#endif // !ACTORFACTORY_H
