//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERPHYSICSMODEL_PHY_H
#define PARSERPHYSICSMODEL_PHY_H

#include "engine/iparserphysicsmodel.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ParserPhysicsModelPHY : public IParserPhysicsModel
	{
	public:
		// IParser
		virtual const char*				GetVersion() const;
		virtual const char*				GetName() const;
		virtual UInt32_t				GetCountFileExtensions() const;
		virtual const char**			GetFileExtensions() const;
		virtual const char*				GetAuthor() const;

		// IParserPhysicsModel
		virtual IPhysicsModel*			Read( const char* Path, IFactory* PhysicsSystemFactory );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERPHYSICSMODEL_PHY_H
