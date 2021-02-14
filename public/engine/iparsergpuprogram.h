//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
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
