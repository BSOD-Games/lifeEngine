//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef BASEPRIMITIVE_H
#define BASEPRIMITIVE_H

#include "common/types.h"
#include "studiorender/mesh.h"

//----------------------------------------------------------------------//

namespace le
{
	//----------------------------------------------------------------------//

	class BasePrimitive
	{
	public:
		BasePrimitive();
		~BasePrimitive();
        
		virtual void		Create() = 0;
		inline void			Delete()
		{
			if ( !mesh.IsCreated() ) return;
			mesh.Delete();
		}

		inline void			Bind()
		{
			if ( !mesh.IsCreated() ) return;
			mesh.GetVertexArrayObject().Bind();
		}

		inline void			Unbind()
		{
			VertexArrayObject::Unbind();
		}

		inline bool			IsCreated() const 
		{ 
			return mesh.IsCreated(); 
		}

		virtual UInt32_t		GetCountIndeces() const = 0;
		virtual UInt32_t		GetStartIndex() const = 0;

	protected:
		Mesh		mesh;
	};

	//----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !BASEPRIMITIVE_H
