//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STUDIORENDER_H
#define STUDIORENDER_H

#include <unordered_map>
#include <vector>

#include "studiorender/istudiorenderinternal.h"
#include "rendercontext.h"
#include "studiorenderfactory.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class VertexArrayObject;
	class IMaterialInternal;

	//---------------------------------------------------------------------//

	struct RenderObject
	{
		VertexArrayObject*		vertexArrayObject;
		IMaterialInternal*		material;
		UInt32_t				startIndex;
		UInt32_t				countIndeces;
		UInt32_t				primitiveType;
		Matrix4x4_t				transformation;
	};

	//---------------------------------------------------------------------//

	class StudioRender : public IStudioRenderInternal
	{
	public:
		// IStudioRenderInternal
		virtual bool			Initialize( IEngine* Engine );
		virtual void			RenderFrame();

		// IStudioRender
		virtual void			ResizeViewport( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height );
		virtual void			Draw( IMesh* Mesh, const Matrix4x4_t& Transformation, ICamera* Camera );

		virtual void			SetVerticalSyncEnabled( bool IsEnabled = true );
		virtual IFactory*		GetFactory() const;
		
		// StudioRender
		StudioRender();
		~StudioRender();

	private:
		bool													isInitialize;

		RenderContext											renderContext;
		StudioRenderFactory										studioRenderFactory;

		std::unordered_map< ICamera*, std::vector< RenderObject > >			renderObjects;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_INTERNAL_H
