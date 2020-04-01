//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISCRIPT_H
#define ISCRIPT_H

#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ScriptDescriptor;

	//---------------------------------------------------------------------//

	class IScript : public IReferenceObject
	{
	public:
		virtual ~IScript() {}
		virtual bool			Load( const ScriptDescriptor& ScriptDescriptor ) = 0;
		virtual void			Unload() = 0;
		virtual void			Update() = 0;

		virtual bool			IsLoaded() const = 0;
		virtual void*			GetSymbol( const char* Name ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define SCRIPT_INTERFACE_VERSION "LE_Script001"

//---------------------------------------------------------------------//

#endif // !ISCRIPT_H
