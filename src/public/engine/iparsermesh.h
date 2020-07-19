//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPARSERMESH_H
#define IPARSERMESH_H

#include "engine/iparser.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;
	class IResourceSystem;
	class IMesh;

	//---------------------------------------------------------------------//

	class IParserMesh : public IParser
	{
	public:
		virtual ~IParserMesh() {}
		virtual IMesh*				Read( const char* Path, IResourceSystem* ResourceSystem, IFactory* StudioRenderFactory ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPARSERMESH_H
