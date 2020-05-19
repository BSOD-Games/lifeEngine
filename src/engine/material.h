//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include <unordered_map>

#include "engine/imaterial.h"
#include "engine/shaderparameter.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Material : public IMaterial
	{
	public:
        // IReferenceObject
        virtual void                        IncrementReference();
        virtual void                        DecrementReference();
        virtual void                        Release();
        virtual UInt32_t                    GetCountReferences() const;

		// IMaterial
		virtual void                        AddParameter( IShaderParameter* Parameter );
		virtual void                        RemoveParameter( UInt32_t Index );
		virtual void                        AddProxy( IMaterialProxy* MaterialProxy );
		virtual void                        RemoveProxy( UInt32_t Index );
		virtual void                        Clear();
		virtual void                        EnableDepthTest( bool Enable = true );
		virtual void                        EnableDepthWrite( bool Enable = true );
		virtual void                        EnableBlend( bool Enable = true );
		virtual void                        EnableCullFace( bool Enable = true );
		virtual IShaderParameter*           FindParameter( const char* Name ) const;
		virtual void						OnBind( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );

		virtual void                        SetShader( const char* Name );
		virtual void                        SetCullFaceType( CULLFACE_TYPE CullFaceType );
		virtual void						SetSurfaceName( const char* Name );

		virtual bool                        IsDepthTest() const;
		virtual bool                        IsDepthWrite() const;
		virtual bool                        IsBlend() const;
		virtual bool                        IsCullFace() const;
		virtual CULLFACE_TYPE               GetCullFaceType() const;
		virtual const char*					GetShader() const;
		virtual UInt32_t                    GetCountParameters() const;
		virtual IShaderParameter**          GetParameters() const;
		virtual IShaderParameter*           GetParameter( UInt32_t Index ) const;
		virtual UInt32_t                    GetCountProxes() const;
		virtual IMaterialProxy**            GetProxes() const;
		virtual IMaterialProxy*             GetProxy( UInt32_t Index ) const;
		virtual const char*					GetSurfaceName() const;

		// Material
		Material();
		~Material();
		
		bool								UpdateShader();
		void								UpdateMaterialProxy();
		inline void							NeadUpdateShader()
		{
			isNeadUpdateShader = true;
		}
		
		inline void							NeadUpdateMaterialProxy()
		{
			isNeadUpdateMaterialProxy = true;
		}

	private:	 
		bool								isNeadUpdateShader;
		bool								isNeadUpdateMaterialProxy;
		bool								isDepthTest;
		bool								isDepthWrite;
		bool								isBlend;
		bool								isCullFace;
		CULLFACE_TYPE						cullFaceType;

		UInt32_t                            countReferences;
		IShader*							shader;
		std::string							surface;
		std::vector< ShaderParameter* >		parameters;
		std::vector< IMaterialProxy* >      materialProxes;
    };

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_H

