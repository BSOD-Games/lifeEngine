//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPARSERTEXTURE_H
#define IPARSERTEXTURE_H

#include "engine/iparser.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;
	class ITexture;

	//---------------------------------------------------------------------//

	class IParserTexture : public IParser
	{
	public:
		virtual ~IParserTexture() {}
		virtual ITexture*				Read( const char* Path, IFactory* StudioRenderFactory ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPARSERTEXTURE_H
