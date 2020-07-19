//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPARSERGPUPROGRAM_H
#define IPARSERGPUPROGRAM_H

#include "engine/iparser.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;
	class IGPUProgram;

	//---------------------------------------------------------------------//

	class IParserGPUProgram : public IParser
	{
	public:
		virtual ~IParserGPUProgram() {}
		virtual IGPUProgram*		Read( const char* Directory, const char* Path, UInt32_t CountDefines, const char** Defines, IFactory* StudioRenderFactory ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPARSERGPUPROGRAM_H
