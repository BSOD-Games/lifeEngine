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

#include "engine/iscript.h"

//---------------------------------------------------------------------//

struct lua_State;

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
		virtual void            Start();
		virtual void            Update();

		// Script
		Script();
		~Script();

		bool					Load( const char* Path );
		void					Unload();

	private:
		bool					isExistStart;
		bool					isExistUpdate;
		UInt32_t				countReferences;
		lua_State*				luaState;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SCRIPT_H
