//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISHADERPARAMETER_H
#define ISHADERPARAMETER_H

#include "common/types.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ITexture;

	//---------------------------------------------------------------------//

	enum SHADER_PARAMETER_TYPE
	{
		SPT_TEXTURE,
		SPT_FLOAT,
		SPT_INT,
		SPT_SHADER_FLAG,
		SPT_VECTOR_2D,
		SPT_VECTOR_3D,
		SPT_VECTOR_4D,
		SPT_MATRIX
	};

	//---------------------------------------------------------------------//

    class IShaderParameter : public IReferenceObject
	{
	public:
        virtual ~IShaderParameter() {}
		virtual void					Clear() = 0;

		virtual void					SetName( const char* Name ) = 0;
		virtual void					SetValueInt( int Value ) = 0;
		virtual void					SetValueFloat( float Value ) = 0;
		virtual void					SetValueShaderFlag( bool Value ) = 0;
		virtual void					SetValueVector2D( const Vector2D_t& Value ) = 0;
		virtual void					SetValueVector3D( const Vector3D_t& Value ) = 0;
		virtual void					SetValueVector4D( const Vector4D_t& Value ) = 0;
		virtual void					SetValueMatrix( const Matrix4x4_t& Value ) = 0;
		virtual void					SetValueTexture( ITexture* Texture ) = 0;

		virtual bool					IsDefined() const = 0;
		virtual const char*				GetName() const = 0;
		virtual SHADER_PARAMETER_TYPE	GetType() const = 0;
		virtual int						GetValueInt() const = 0;
		virtual float					GetValueFloat() const = 0;
		virtual bool					GetValueShaderFlag() const = 0;
		virtual const Vector2D_t&		GetValueVector2D() const = 0;
		virtual const Vector3D_t&		GetValueVector3D() const = 0;
		virtual const Vector4D_t&		GetValueVector4D() const = 0;
		virtual const Matrix4x4_t&		GetValueMatrix() const = 0;
		virtual ITexture*				GetValueTexture() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define SHADERPARAMETER_INTERFACE_VERSION "LE_ShaderParameter001"

//---------------------------------------------------------------------//

#endif // !ISHADERPARAMETER_H

