//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IAPPSYSTEM_H
#define IAPPSYSTEM_H

#include "engine/ifactory.h"

namespace le
{
	//---------------------------------------------------------------------//

	class IAppSystem
	{
	public:
		virtual bool				Connect( IFactory* Factory ) = 0;
		virtual void				Disconnect() = 0;
		virtual void				Shutdown() = 0;
		virtual bool				Initialize() = 0;

		virtual IFactory*			GetFactory() const = 0;
	};

	//---------------------------------------------------------------------//
}

#endif // !IAPPSYSTEM_H

