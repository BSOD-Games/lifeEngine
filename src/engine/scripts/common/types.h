//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_TYPES_H
#define API_TYPES_H

//---------------------------------------------------------------------//

typedef enum
{
    B_FALSE,
    B_TRUE
} bool_t;

//---------------------------------------------------------------------//

typedef		signed char			int8_t;
typedef		short				int16_t;
typedef		int					int32_t;
typedef		long long			int64_t;
typedef		unsigned char		uint8_t;
typedef		unsigned short		uint16_t;
typedef		unsigned int		uint32_t;
typedef		unsigned long long	uint64_t;
typedef		unsigned char		byte_t;

//---------------------------------------------------------------------//

typedef 	void*			material_t;
typedef 	void*			text_t;
typedef 	void*			font_t;
typedef		void*			texture_t;
typedef 	void*			sprite_t;
typedef 	void*			mesh_t;
typedef 	void*			model_t;
typedef		void*			materialProxyVar_t;
typedef		void*			materialProxy_t;
typedef		void*			level_t;
typedef		void*			camera_t;
typedef		void*			body_t;
typedef		void*			charcterController_t;
typedef		void*			collider_t;
typedef		void*			physicsModel_t;
typedef		void*			shaderParameter_t;

//---------------------------------------------------------------------//

#endif // !API_TYPES_H
