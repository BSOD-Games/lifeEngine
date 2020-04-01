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
		virtual void			RegisterSymbol( const char* Name, void* Value );
		virtual void			AddScript( IScript* Script );
		virtual void			RemoveScript( UInt32_t Index );
		virtual void			RemoveScript( IScript* Script );
		virtual void			RemoveAllScripts();

		virtual IFactory*		GetFactory() const;
		virtual UInt32_t		GetCountScripts() const;
		virtual IScript*		GetScript( UInt32_t Index ) const;
		virtual IScript**		GetScrips() const;

		// IScriptSystemInternal
		virtual bool			Initialize( IEngine* Engine );
		virtual void			Update();

		// ScriptSystem
		ScriptSystem();
		~ScriptSystem();

		inline const std::unordered_map< std::string, void* >&			GetSymbols() const
		{
			return symbols;
		}

	private:
		void				RegisterConsoleSystem();
		void				RegisterInputSystem();

		ScriptSystemFactory								factory;
		std::unordered_map< std::string, void* >		symbols;
		std::vector< Script* >							scripts;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SCRIPTSYSTEM_H
