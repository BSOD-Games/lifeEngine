//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIALVAR_H
#define IMATERIALVAR_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum MATERIAL_VAR_TYPE
	{
		MVT_TEXTURE,
		MVT_FLOAT,
		MVT_INT,
		MVT_BOOL,
		MVT_VECTOR_2D,
		MVT_VECTOR_3D,
		MVT_VECTOR_4D,
		MVT_MATRIX,
		MVT_STRING,
		MVT_SHADER_FLAG
	};

	//---------------------------------------------------------------------//

	class IMaterialVar
	{
	public:
		virtual void					Clear() = 0;

		virtual void					SetName( const char* Name ) = 0;
		virtual void					SetValueInt( int Value ) = 0;
		virtual void					SetValueFloat( float Value ) = 0;
		virtual void					SetValueBool( bool Value ) = 0;
		virtual void					SetValueVector2D( const Vector2D_t& Value ) = 0;
		virtual void					SetValueVector3D( const Vector3D_t& Value ) = 0;
		virtual void					SetValueVector4D( const Vector4D_t& Value ) = 0;
		virtual void					SetValueMatrix( const Matrix4x4_t& Value ) = 0;
		virtual void					SetValueString( const char* Value ) = 0;

		virtual bool					IsDefined() const = 0;
		virtual const char*				GetName() const = 0;
		virtual MATERIAL_VAR_TYPE		GetType() const = 0;
		virtual int						GetValueInt() const = 0;
		virtual float					GetValueFloat() const = 0;
		virtual bool					GetValueBool() const = 0;
		virtual const Vector2D_t&		GetValueVector2D() const = 0;
		virtual const Vector3D_t&		GetValueVector3D() const = 0;
		virtual const Vector4D_t&		GetValueVector4D() const = 0;
		virtual const Matrix4x4_t&		GetValueMatrix() const = 0;
		virtual const char*				GetValueString() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define MATERIALVAR_INTERFACE_VERSION "LE_MaterialVar001"

//---------------------------------------------------------------------//

#endif // !IMATERIALVAR_H

