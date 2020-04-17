//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>
#include <vector>
#include <unordered_map>

#include "engine/iscript.h"

//---------------------------------------------------------------------//

struct TCCState;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Script : public IScript
	{
	public:
		// IReferenceObject
		virtual void			IncrementReference();
		virtual void			DecrementReference();
		virtual void			Release();

		virtual UInt32_t		GetCountReferences() const;

		// IScript
		virtual bool			Load( const ScriptDescriptor& ScriptDescriptor );
		virtual void			Unload();

		virtual bool			IsLoaded() const;
		virtual void*			GetFunction( const char* Name );
		virtual void*			GetVar( const char* Name );

		// Script
		Script();
		~Script();

	private:
		UInt32_t										countReferences;
		TCCState*										tccContext;

		std::unordered_map< std::string, void* >		functionsCache;
		std::unordered_map< std::string, void* >		varsCache;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SCRIPT_H
