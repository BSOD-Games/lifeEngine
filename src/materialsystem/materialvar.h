//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIALVAR_H
#define MATERIALVAR_H

#include <string>

#include "materialsystem/imaterialvar.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class MaterialVar : public IMaterialVar
	{
	public:
		// IMaterialVar
		virtual void					Clear();

		virtual void					SetName( const char* Name );
		virtual void					SetValueInt( int Value );
		virtual void					SetValueFloat( float Value );
		virtual void					SetValueBool( bool Value );
		virtual void					SetValueVector2D( const Vector2D_t& Value );
		virtual void					SetValueVector3D( const Vector3D_t& Value );
		virtual void					SetValueVector4D( const Vector4D_t& Value );
		virtual void					SetValueMatrix( const Matrix4x4_t& Value );
		virtual void					SetValueString( const char* Value );

		virtual bool					IsDefined() const;
		virtual const char*				GetName() const;
		virtual MATERIAL_VAR_TYPE		GetType() const;
		virtual int						GetValueInt() const;
		virtual float					GetValueFloat() const;
		virtual bool					GetValueBool() const;
		virtual const Vector2D_t&		GetValueVector2D() const;
		virtual const Vector3D_t&		GetValueVector3D() const;
		virtual const Vector4D_t&		GetValueVector4D() const;
		virtual const Matrix4x4_t&		GetValueMatrix() const;
		virtual const char*				GetValueString() const;

		// MaterialVar
		MaterialVar();
		~MaterialVar();

	private:
		bool					isDefined;

		std::string				name;
		MATERIAL_VAR_TYPE		type;

		union
		{
			int					value_int;
			float				value_float;
			bool				value_bool;
			Vector2D_t			value_vector2D;
			Vector3D_t			value_vector3D;
			Vector4D_t			value_vector4D;
			Matrix4x4_t			value_matrix4x4;
			std::string			value_string;
		};
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIALVAR_H

