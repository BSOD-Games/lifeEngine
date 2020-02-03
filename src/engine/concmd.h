//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CONCMD_H
#define CONCMD_H

#include <string>
#include "engine/iconcmd.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ConCmd : public IConCmd
	{
	public:
		// IConCmd
		virtual void					Initialize( const char* Name, const char* HelpText, ExecCallbackFn_t ExecCallback );
		virtual void					Exec( UInt32_t CountArguments, const char** Arguments );
		
		virtual void					SetExecCallback( ExecCallbackFn_t ExecCallback );
		virtual void					SetName( const char* Name );
		
		virtual const char*				GetName() const;
		virtual const char*				GetHelpText() const;
		
		// ConCmd
		ConCmd();
		~ConCmd();

	private:
		std::string					name;
		std::string					helpText;
		ExecCallbackFn_t			execCallback;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ICONCMD_H

