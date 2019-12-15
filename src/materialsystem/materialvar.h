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

	class Material;

	//---------------------------------------------------------------------//

	class MaterialVar : public IMaterialVar
	{
	public:
		// IMaterialVar
		virtual void					Clear();

		virtual void					SetName( const char* Name );
		virtual void					SetValueInt( int Value );
		virtual void					SetValueFloat( float Value );
		virtual void					SetValueShaderFlag( bool Value );
		virtual void					SetValueVector2D( const Vector2D_t& Value );
		virtual void					SetValueVector3D( const Vector3D_t& Value );
		virtual void					SetValueVector4D( const Vector4D_t& Value );
		virtual void					SetValueMatrix( const Matrix4x4_t& Value );
		virtual void					SetValueTexture( ITexture* Texture );

		virtual bool					IsDefined() const;
		virtual const char*				GetName() const;
		virtual MATERIAL_VAR_TYPE		GetType() const;
		virtual int						GetValueInt() const;
		virtual float					GetValueFloat() const;
		virtual bool					GetValueShaderFlag() const;
		virtual const Vector2D_t&		GetValueVector2D() const;
		virtual const Vector3D_t&		GetValueVector3D() const;
		virtual const Vector4D_t&		GetValueVector4D() const;
		virtual const Matrix4x4_t&		GetValueMatrix() const;
		virtual ITexture*				GetValueTexture() const;

		// MaterialVar
		MaterialVar( Material* Material );
		~MaterialVar();

	private:
		bool					isDefined;

		std::string				name;
		MATERIAL_VAR_TYPE		type;
		Material*				material;

		union
		{
			int					value_int;
			float				value_float;
			bool				value_shaderFlag;
			Vector2D_t			value_vector2D;
			Vector3D_t			value_vector3D;
			Vector4D_t			value_vector4D;
			Matrix4x4_t			value_matrix4x4;
			ITexture*			value_texture;
		};
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIALVAR_H

