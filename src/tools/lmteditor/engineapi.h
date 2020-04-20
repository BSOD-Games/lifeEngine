//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ENGINEAPI_H
#define ENGINEAPI_H

#include <qlibrary.h>

#include "engine/lifeengine.h"
#include "engine/iengineinternal.h"
#include "tsingleton.h"

//---------------------------------------------------------------------//

class EngineAPI : public TSingleton<EngineAPI>
{
public:
	friend void Engine_CriticalError( const char* Message );

	// EngineAPI
	EngineAPI();
	~EngineAPI();

	bool								Load();
	void								Unload();

	le::IEngineInternal*				GetEngine() const;
	le::IConsoleSystem*					GetConsoleSystem() const;
	inline QString						GetErrorString() const		{ return errorString; }

private:
	QLibrary							engineDLL;
	QString								errorString;

	le::IEngineInternal*				engine;
	le::LE_CreateEngineFn_t				LE_CreateEngine;
	le::LE_SetCriticalErrorFn_t			LE_SetCriticalError;
	le::LE_DeleteEngineFn_t				LE_DeleteEngine;
};

//---------------------------------------------------------------------//

#endif // ENGINEAPI_H
