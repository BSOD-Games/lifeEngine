//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IFACTORY_H
#define IFACTORY_H

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory
	{
	public:
		virtual void*			Create( const char* NameInterface ) = 0;
		virtual void			Delete( const char* NameInterface, void* Object ) = 0;
	};

	//---------------------------------------------------------------------//
}

#endif // !IFACTORY_H

