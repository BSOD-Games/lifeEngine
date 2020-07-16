//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERSCRIPT_�_H
#define PARSERSCRIPT_�_H

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

#endif // !PARSERSCRIPT_�_H
