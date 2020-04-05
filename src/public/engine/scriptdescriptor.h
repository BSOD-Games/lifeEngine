//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SCRIPTDESCRIPTOR_H
#define SCRIPTDESCRIPTOR_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ScriptDescriptor
	{
		struct Symbol
		{
			const char*			name;
			void*				value;
		};

		const char*		code;
		UInt32_t		countFunctions;
		Symbol*			functions;
		UInt32_t		countVars;
		Symbol*			vars;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SCRIPTDESCRIPTOR_H
