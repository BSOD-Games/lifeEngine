//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef TYPES_H
#define TYPES_H

#include "mathlib/glm.hpp"
#include "mathlib/gtx/quaternion.hpp"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	typedef		signed char			Int8_t;
	typedef		short				Int16_t;
	typedef		int					Int32_t;
	typedef		long long			Int64_t;
	typedef		unsigned char		UInt8_t;
	typedef		unsigned short		UInt16_t;
	typedef		unsigned int		UInt32_t;
	typedef		unsigned long long	UInt64_t;

	typedef		glm::vec2			Vector2D_t;
	typedef		glm::vec3			Vector3D_t;
	typedef		glm::vec4			Vector4D_t;
	typedef		glm::quat			Quaternion_t;
	typedef		glm::mat4			Matrix4x4_t;

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !TYPES_H

