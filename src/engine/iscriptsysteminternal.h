//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISCRIPTSYSTEMINTERNAL_H
#define ISCRIPTSYSTEMINTERNAL_H

#include "engine/iscriptsystem.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;

	//---------------------------------------------------------------------//

	class IScriptSystemInternal : public IScriptSystem
	{
	public:
		virtual ~IScriptSystemInternal() {}
		virtual bool			Initialize( IEngine* Engine ) = 0;
		virtual void			Update() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISCRIPTSYSTEMINTERNAL_H
