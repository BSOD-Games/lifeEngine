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
		// IStudioRenderPass
		virtual void				AddParameter( IShaderParameter* Parameter );
		virtual void				RemoveParameter( UInt32_t Index );
		virtual void				Clear(); 
		
		virtual void				SetShader( const char* NameShader );
	
		virtual const char*			GetNameShader() const;
		virtual UInt32_t			GetCountParameters() const;
		virtual IShaderParameter**	GetParameters() const;
		virtual IShaderParameter*	GetParameter( UInt32_t Index ) const;

		// StudioRenderPass
		StudioRenderPass();
		~StudioRenderPass();

		void						Apply( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );
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
		
		IShader*							shader;
		std::vector< ShaderParameter* >		parameters;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !STUDIORENDERPASS_H

