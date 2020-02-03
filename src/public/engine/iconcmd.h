//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ICONCMD_H
#define ICONCMD_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	typedef	void				( *ExecCallbackFn_t )( UInt32_t CountArguments, const char** Arguments );

	//---------------------------------------------------------------------//

	class IConCmd
	{
	public:
		virtual void					Initialize( const char* Name, const char* HelpText, ExecCallbackFn_t ExecCallback ) = 0;
		virtual void					Exec( UInt32_t CountArguments, const char** Arguments ) = 0;
		
		virtual void					SetExecCallback( ExecCallbackFn_t ExecCallback ) = 0;
		virtual void					SetName( const char* Name ) = 0;
		
		virtual const char*				GetName() const = 0;
		virtual const char*				GetHelpText() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define CONCMD_INTERFACE_VERSION "LE_ConCmd002"

//---------------------------------------------------------------------//

#endif // !ICONCMD_H

