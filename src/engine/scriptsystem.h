//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "engine/iscriptsysteminternal.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Script;

	//---------------------------------------------------------------------//

	class ScriptSystem : public IScriptSystemInternal
	{
	public:
		// IScriptSystem
		virtual IScript*		CreateScript( const char* Path ) const;

		// IScriptSystemInternal
		virtual bool			Initialize( IEngine* Engine );

		// ScriptSystem
		ScriptSystem();
		~ScriptSystem();

		static void				RegisterEngineAPI( lua_State* LuaVM );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SCRIPTSYSTEM_H
