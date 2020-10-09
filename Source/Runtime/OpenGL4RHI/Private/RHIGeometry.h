// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RHIGEOMETRY_H
#define RHIGEOMETRY_H

#include <GL/glew.h>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"
#include "Rendering/RHI/IRHIGeometry.h"
#include "GLState.h"

namespace le
{
	class RHIBuffer;
	class RHIVertexFormat;

	class RHIGeometry : public IRHIGeometry
	{
	public:
		friend class RHIBuffer;
		friend class RHIVertexFormat;

		/* Constructor */
		RHIGeometry();

		/* Destructor */
		~RHIGeometry();

		/* Bind geometry for draw */
		void Bind();

		/* Unbind geometry */
		FORCEINLINE static void Unbind()
		{
			GLState::BindVAO( 0 );
		}

		/* Notify of destroy */
		void NotifyDestroy( RHIBuffer* InBuffer );
		void NotifyDestroy( RHIVertexFormat* InVertexFormat );

		/* Set vertex buffer */
		void SetVertexBuffer( IRHIBuffer* InVertexBuffer ) override;

		/* Set vertex format */
		void SetVertexFormat( IRHIVertexFormat* InVertexFormat ) override;

		/* Set index buffer */
		void SetIndexBuffer( IRHIBuffer* InIndexBuffer, EIndecesType InIndecesType ) override;

		/* Is valid */
		FORCEINLINE bool IsValid() const
		{
			return vao > 0 || ( vertexBuffer && vertexFormat );
		}

		/* Is indexed */
		FORCEINLINE bool IsIndexed() const
		{
			return indexBuffer;
		}

		/* Get indeces type */
		EIndecesType GetIndecesType() const override;

	private:
		/* Create */
		void Create();

		/* Destroy */
		FORCEINLINE void Destroy()
		{
			if ( !vao )		return;
			glDeleteVertexArrays( 1, &vao );
		}

		bool					isNeedRefresh;
		uint32					vao;
		EIndecesType			indecesType;

		RHIBuffer*				vertexBuffer;
		RHIVertexFormat*		vertexFormat;
		RHIBuffer*				indexBuffer;
	};
}

#endif // !RHIGEOMETRY_H
