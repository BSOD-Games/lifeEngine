//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STUDIORENDERPASS_H
#define STUDIORENDERPASS_H

#include <vector>

#include "studiorender/istudiorenderpass.h"
#include "studiorender/shaderparameter.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ICamera;

	//---------------------------------------------------------------------//

    class StudioRenderPass : public IStudioRenderPass
	{
	public:
        // IReferenceObject
        virtual void                        IncrementReference();
        virtual void                        DecrementReference();
        virtual void                        Release();
        virtual UInt32_t                    GetCountReferences() const;

		// IStudioRenderPass
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

        virtual void                        SetShader( const char* NameShader );
        virtual void                        SetCullFaceType( CULLFACE_TYPE CullFaceType );

        virtual bool                        IsDepthTest() const;
        virtual bool                        IsDepthWrite() const;
        virtual bool                        IsBlend() const;
        virtual bool                        IsCullFace() const;
        virtual CULLFACE_TYPE               GetCullFaceType() const;
        virtual const char*                 GetNameShader() const;
        virtual UInt32_t                    GetCountParameters() const;
        virtual IShaderParameter**          GetParameters() const;
        virtual IShaderParameter*           GetParameter( UInt32_t Index ) const;
        virtual UInt32_t                    GetCountProxes() const;
        virtual IMaterialProxy**            GetProxes() const;
        virtual IMaterialProxy*             GetProxy(UInt32_t Index) const;

		// StudioRenderPass
		StudioRenderPass();
		~StudioRenderPass();

		void						Apply( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );
		void						InitStates();
		bool						Refrash();

        inline void					NeadRefrash()
        {
            isNeadRefrash = true;
        }

        inline bool					IsNeadRefrash() const
        {
            return isNeadRefrash;
        }

	private:
		bool								isNeadRefrash;
		bool								isDepthTest;
		bool								isDepthWrite;
		bool								isBlend;
		bool								isCullFace;
		CULLFACE_TYPE						cullFaceType;

		IShader*							shader;
        std::vector< ShaderParameter* >		parameters;
        std::vector< IMaterialProxy* >      materialProxes;
        UInt32_t                            countReferences;
    };

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !STUDIORENDERPASS_H

