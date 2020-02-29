//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef BITSET_H
#define BITSET_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Bitset
	{
	public:
		Bitset();
		~Bitset();

        void			Resize( UInt32_t Count );
		void			Set( int Byte );
		int				On( int Byte );
		void			Clear( int Byte );
		void			ClearAll();

	private:
        UInt32_t*		bits;
        UInt32_t		size;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !BITSET_H

