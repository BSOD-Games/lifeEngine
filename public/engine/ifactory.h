//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
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
        virtual ~IFactory() {}
		virtual void*			Create( const char* NameInterface ) = 0;
		virtual void			Delete( void* Object ) = 0;
	};

	//---------------------------------------------------------------------//
}

#endif // !IFACTORY_H

