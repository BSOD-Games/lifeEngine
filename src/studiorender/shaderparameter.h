//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H

#include <string>

#include "studiorender/ishaderparameter.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class StudioRenderPass;

	//---------------------------------------------------------------------//

	class ShaderParameter : public IShaderParameter
	{
	public:
        // IReferenceObject
        virtual void                    IncrementReference();
        virtual void                    DecrementReference();
        virtual void                    Release();
        virtual UInt32_t                GetCountReferences() const;

		// IShaderParameter
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
		virtual SHADER_PARAMETER_TYPE	GetType() const;
		virtual int						GetValueInt() const;
		virtual float					GetValueFloat() const;
		virtual bool					GetValueShaderFlag() const;
		virtual const Vector2D_t&		GetValueVector2D() const;
		virtual const Vector3D_t&		GetValueVector3D() const;
		virtual const Vector4D_t&		GetValueVector4D() const;
		virtual const Matrix4x4_t&		GetValueMatrix() const;
		virtual ITexture*				GetValueTexture() const;

		// ShaderParameter
		ShaderParameter();
		~ShaderParameter();

		inline void							SetPass( StudioRenderPass* Pass )
		{
			studioRenderPass = Pass;
		}

	private:
		bool					isDefined;

		std::string				name;
		SHADER_PARAMETER_TYPE	type;
		StudioRenderPass*		studioRenderPass;
        void*                   value;
        UInt32_t                countReferences;
    };

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADERPARAMETER_H
