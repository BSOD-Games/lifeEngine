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

#include <string>
#include <vector>
#include <unordered_map>

#include "engine/iscriptsysteminternal.h"
#include "engine/scriptsystemfactory.h"
#include "engine/scriptdescriptor.h"

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
		virtual void			RegisterFunction( const char* Name, void* Value );
		virtual void			RegisterVar( const char* Name, void* Value );
		virtual void			UnregisterFunction( const char* Name );
		virtual void			UnregisterVar( const char* Name );

		virtual IFactory*		GetFactory() const;

		// IScriptSystemInternal
		virtual bool			Initialize( IEngine* Engine );

		// ScriptSystem
		ScriptSystem();
		~ScriptSystem();

		inline const std::unordered_map< std::string, void* >&			GetFunctions() const
		{
			return functions;
		}

		inline const std::unordered_map< std::string, void* >&			GetVars() const
		{
			return  vars;
		}

	private:
		ScriptSystemFactory								factory;
		std::unordered_map< std::string, void* >		functions;
		std::unordered_map< std::string, void* >		vars;

	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SCRIPTSYSTEM_H
