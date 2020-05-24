//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_INTERNAL_H
#define MATERIAL_INTERNAL_H

#include <string>
#include <vector>
#include <unordered_map>

#include "engine/imaterial.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IShader;

	//---------------------------------------------------------------------//

	class IMaterialInternal : public IMaterial
	{
	public:
        // IMaterialInternal
		virtual ~IMaterialInternal() {}
		virtual void				Refresh() = 0;

		virtual IShader*			GetShader() const = 0;
    };

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_INTERNAL_H

