//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPARSERPHYSICSMODEL_H
#define IPARSERPHYSICSMODEL_H

#include "engine/iparser.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;
	class IPhysicsModel;

	//---------------------------------------------------------------------//

	class IParserPhysicsModel : public IParser
	{
	public:
		virtual ~IParserPhysicsModel() {}
		virtual IPhysicsModel*				Read( const char* Path, IFactory* PhysicsSystemFactory ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPARSERPHYSICSMODEL_H
