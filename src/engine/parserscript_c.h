//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERSCRIPT_С_H
#define PARSERSCRIPT_С_H

#include "engine/iparserscript.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ParserScriptC : public IParserScript
	{
	public:
		// IParser
		virtual const char*			GetVersion() const;
		virtual const char*			GetName() const;
		virtual UInt32_t			GetCountFileExtensions() const;
		virtual const char**		GetFileExtensions() const;
		virtual const char*			GetAuthor() const;

		// IParserScript	
		virtual IScript*			Read( const char* Path, UInt32_t CountFunctions, ScriptDescriptor::Symbol* Functions, UInt32_t CountVars, ScriptDescriptor::Symbol* Vars, IFactory* ScriptSystemFactory );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERSCRIPT_С_H
