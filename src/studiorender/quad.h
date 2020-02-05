//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef QUAD_H
#define QUAD_H

#include "baseprimitive.h"

//----------------------------------------------------------------------//

namespace le
{
	//----------------------------------------------------------------------//

	class Quad : public BasePrimitive
	{
	public:
		Quad();
		~Quad();

		virtual void		Create();

		virtual UInt32_t	GetCountIndeces() const;
		virtual UInt32_t	GetStartIndex() const;
	};

	//----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !QUAD_H
