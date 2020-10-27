// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef IRHIGEOMETRY_H
#define IRHIGEOMETRY_H

#include "Misc/Types.h"
#include "Misc/Object.h"
#include "Misc/RefCounted.h"

namespace le
{
	class IRHIBuffer;
	class IRHIVertexFormat;

	enum EIndecesType
	{
		IT_UByte,
		IT_UShort,
		IT_UInt
	};

	class IRHIGeometry : public Object, public RefCounted
	{
	public:
		/* Destructor */
		virtual ~IRHIGeometry() {}

		/* Set vertex buffer */
		virtual void SetVertexBuffer( FIRHIBufferConstRef& InVertexBuffer ) = 0;

		/* Set vertex format */
		virtual void SetVertexFormat( FIRHIVertexFormatConstRef& InVertexFormat ) = 0;

		/* Set index buffer */
		virtual void SetIndexBuffer( FIRHIBufferConstRef& InIndexBuffer, EIndecesType InIndecesType ) = 0;

		/* Get indeces type */
		virtual EIndecesType GetIndecesType() const = 0;
	};
}

#endif // !IRHIGEOMETRY_H
