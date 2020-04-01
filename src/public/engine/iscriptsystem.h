//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISCRIPTSYSTEM_H
#define ISCRIPTSYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;
	class IScript;

	//---------------------------------------------------------------------//

	class IScriptSystem
	{
	public:
		virtual ~IScriptSystem() {}
		virtual void			RegisterSymbol( const char* Name, void* Value ) = 0;
		virtual void			AddScript( IScript* Script ) = 0;
		virtual void			RemoveScript( UInt32_t Index ) = 0;
		virtual void			RemoveScript( IScript* Script ) = 0;
		virtual void			RemoveAllScripts() = 0;

		virtual IFactory*		GetFactory() const = 0;
		virtual UInt32_t		GetCountScripts() const = 0;
		virtual IScript*		GetScript( UInt32_t Index ) const = 0;
		virtual IScript**		GetScrips() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISCRIPTSYSTEM_H
