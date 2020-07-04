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
#include "engine/iresourcesysteminternal.h"
#include "engine/imaterialsysteminternal.h"
#include "studiorender/istudiorenderinternal.h"
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

    inline le::IEngineInternal*			GetEngine() const				{ return engine; }
    inline le::IResourceSystemInternal* GetResourceSystem() const		{ return resourceSystem; }
    inline le::IConsoleSystem*			GetConsoleSystem() const		{ return consoleSystem; }
	inline le::IStudioRenderInternal*	GetStudioRender() const			{ return studioRender; }
	inline le::IMaterialSystemInternal*	GetMaterialSystem() const		{ return materialSystem; }
	inline QString						GetErrorString() const			{ return errorString; }

private:
	QLibrary							engineDLL;
	QString								errorString;

	le::IEngineInternal*				engine;
    le::IResourceSystemInternal*        resourceSystem;
    le::IConsoleSystem*                 consoleSystem;
	le::IStudioRenderInternal*			studioRender;
	le::IMaterialSystemInternal*		materialSystem;

	le::LE_CreateEngineFn_t				LE_CreateEngine;
	le::LE_SetCriticalErrorFn_t			LE_SetCriticalError;
	le::LE_DeleteEngineFn_t				LE_DeleteEngine;
};

//---------------------------------------------------------------------//

#endif // ENGINEAPI_H
