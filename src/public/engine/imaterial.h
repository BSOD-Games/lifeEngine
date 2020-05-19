//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIAL_H
#define IMATERIAL_H

#include "common/types.h"
#include "engine/ireferenceobject.h"
#include "studiorender/istudiorender.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ICamera;
	class ITexture;
	class IShader;
	class IShaderParameter;
	class IMaterialProxy;

	//---------------------------------------------------------------------//

    class IMaterial : public IReferenceObject
	{
	public:
		virtual ~IMaterial() {}
		virtual void                        AddParameter( IShaderParameter* Parameter ) = 0;
		virtual void                        RemoveParameter( UInt32_t Index ) = 0;
		virtual void                        AddProxy( IMaterialProxy* MaterialProxy ) = 0;
		virtual void                        RemoveProxy( UInt32_t Index ) = 0;
		virtual void                        Clear() = 0;
		virtual void                        EnableDepthTest( bool Enable = true ) = 0;
		virtual void                        EnableDepthWrite( bool Enable = true ) = 0;
		virtual void                        EnableBlend( bool Enable = true ) = 0;
		virtual void                        EnableCullFace( bool Enable = true ) = 0;
		virtual IShaderParameter*           FindParameter( const char* Name ) const = 0;
		virtual void						OnBind( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr ) = 0;

		virtual void                        SetShader( const char* Name ) = 0;
		virtual void                        SetCullFaceType( CULLFACE_TYPE CullFaceType ) = 0;
		virtual void						SetSurfaceName( const char* Name ) = 0;

		virtual bool                        IsDepthTest() const = 0;
		virtual bool                        IsDepthWrite() const = 0;
		virtual bool                        IsBlend() const = 0;
		virtual bool                        IsCullFace() const = 0;
		virtual const char*					GetShader() const = 0;
		virtual CULLFACE_TYPE               GetCullFaceType() const = 0;
		virtual UInt32_t                    GetCountParameters() const = 0;
		virtual IShaderParameter**          GetParameters() const = 0;
		virtual IShaderParameter*           GetParameter( UInt32_t Index ) const = 0;
		virtual UInt32_t                    GetCountProxes() const = 0;
		virtual IMaterialProxy**            GetProxes() const = 0;
		virtual IMaterialProxy*             GetProxy( UInt32_t Index ) const = 0;
		virtual const char*					GetSurfaceName() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define MATERIAL_INTERFACE_VERSION "LE_Material004"

//---------------------------------------------------------------------//

#endif // !IMATERIAL_H
