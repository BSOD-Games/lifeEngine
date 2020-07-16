//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERGPUPROGRAM_SHADER_H
#define PARSERGPUPROGRAM_SHADER_H

#include "engine/iparsergpuprogram.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ParserGPUProgramShader : public IParserGPUProgram
	{
	public:
		// IParser
		virtual const char*				GetVersion() const;
		virtual const char*				GetName() const;
		virtual UInt32_t				GetCountFileExtensions() const;
		virtual const char**			GetFileExtensions() const;
		virtual const char*				GetAuthor() const;

		// IParserGPUProgram		
		virtual IGPUProgram*			Read( const char* Directory, const char* Path, UInt32_t CountDefines, const char** Defines, IFactory* StudioRenderFactory );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERGPUPROGRAM_SHADER_H
