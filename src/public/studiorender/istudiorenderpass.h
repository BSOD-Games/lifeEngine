//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISTUDIORENDERPASS_H
#define ISTUDIORENDERPASS_H

#include "common/types.h"
#include "studiorender/istudiorender.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IShader;
	class IShaderParameter;

	//---------------------------------------------------------------------//

	class IStudioRenderPass
	{
	public:
		virtual ~IStudioRenderPass() {}
		virtual void					AddParameter( IShaderParameter* Parameter ) = 0;
		virtual void					RemoveParameter( UInt32_t Index ) = 0;
		virtual void					Clear() = 0;		
		virtual void					EnableDepthTest( bool Enable = true ) = 0;
		virtual void					EnableDepthWrite( bool Enable = true ) = 0;
		virtual void					EnableBlend( bool Enable = true ) = 0;
		virtual void					EnableCullFace( bool Enable = true ) = 0;

		virtual void					SetShader( const char* NameShader ) = 0;
		virtual void					SetCullFaceType( CULLFACE_TYPE CullFaceType ) = 0;

		virtual bool					IsDepthTest() const = 0;
		virtual bool					IsDepthWrite() const = 0;
		virtual bool					IsBlend() const = 0;
		virtual bool					IsCullFace() const = 0;
		virtual CULLFACE_TYPE			GetCullFaceType() const = 0;
		virtual const char*				GetNameShader() const = 0;	
		virtual UInt32_t				GetCountParameters() const = 0;
		virtual IShaderParameter**		GetParameters() const = 0;
		virtual IShaderParameter*		GetParameter( UInt32_t Index ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define PASS_INTERFACE_VERSION "LE_StudioRenderPass002"

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDERPASS_H

