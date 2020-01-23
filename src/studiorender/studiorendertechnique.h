//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STUDIORENDERTECHNIQUE_H
#define STUDIORENDERTECHNIQUE_H

#include <vector>

#include "studiorender/istudiorendertechnique.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//
	
	class StudioRenderPass;

	//---------------------------------------------------------------------//

	class StudioRenderTechnique : public IStudioRenderTechnique
	{
	public:
		// IStudioRenderTechnique
		virtual void					AddPass( IStudioRenderPass* Pass );
		virtual void					RemovePass( UInt32_t Index );
		virtual void					Clear(); 
		
		virtual void					SetType( RENDER_TECHNIQUE Technique );

		virtual RENDER_TECHNIQUE		GetType() const ;
		virtual UInt32_t				GetCountPasses() const;
		virtual IStudioRenderPass**		GetPasses() const;
		virtual IStudioRenderPass*		GetPass( UInt32_t Index ) const;

		// StudioRenderTechnique
		StudioRenderTechnique();
		~StudioRenderTechnique();

	private:
		RENDER_TECHNIQUE						type;
		std::vector< StudioRenderPass* >		passes;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !STUDIORENDERTECHNIQUE_H

