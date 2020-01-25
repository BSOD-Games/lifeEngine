//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISTUDIORENDERTECHNIQUE_H
#define ISTUDIORENDERTECHNIQUE_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//
	
	class IStudioRenderPass;
	
	enum RENDER_TECHNIQUE
	{
		RT_UNKNOW,
		RT_DEFFERED_SHADING
	};

	//---------------------------------------------------------------------//

	class IStudioRenderTechnique
	{
	public:
		virtual ~IStudioRenderTechnique() {}
		virtual void					AddPass( IStudioRenderPass* Pass ) = 0;
		virtual void					RemovePass( UInt32_t Index ) = 0;
		virtual void					Clear() = 0;

		virtual void					SetType( RENDER_TECHNIQUE Technique ) = 0;		

		virtual RENDER_TECHNIQUE		GetType() const = 0;
		virtual UInt32_t				GetCountPasses() const = 0;
		virtual IStudioRenderPass**		GetPasses() const = 0;
		virtual IStudioRenderPass*		GetPass( UInt32_t Index ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define TECHNIQUE_INTERFACE_VERSION "LE_StudioRenderTechnique001"

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDERTECHNIQUE_H

