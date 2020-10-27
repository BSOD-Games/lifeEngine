// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RHIGEOMETRY_H
#define RHIGEOMETRY_H

#include <GL/glew.h>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"
#include "Rendering/RHI/IRHIGeometry.h"
#include "GLState.h"
#include "Types.h"

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
		void NotifyDestroy( FRHIBufferConstRef& InBuffer );
		void NotifyDestroy( FRHIVertexFormatConstRef& InVertexFormat );

		/* Set vertex buffer */
		void SetVertexBuffer( FIRHIBufferConstRef& InVertexBuffer ) override;

		/* Set vertex format */
		void SetVertexFormat( FIRHIVertexFormatConstRef& InVertexFormat ) override;

		/* Set index buffer */
		void SetIndexBuffer( FIRHIBufferConstRef& InIndexBuffer, EIndecesType InIndecesType ) override;

		/* Is valid */
		FORCEINLINE bool IsValid() const
		{
			return vao > 0 || ( vertexBuffer.IsValid() && vertexFormat.IsValid() );
		}

		/* Is indexed */
		FORCEINLINE bool IsIndexed() const
		{
			return indexBuffer.IsValid();
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

		FRHIBufferRef			vertexBuffer;
		FRHIVertexFormatRef		vertexFormat;
		FRHIBufferRef			indexBuffer;
	};
}

#endif // !RHIGEOMETRY_H
