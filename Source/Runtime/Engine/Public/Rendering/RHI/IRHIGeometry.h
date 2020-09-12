// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IRHIGEOMETRY_H
#define IRHIGEOMETRY_H

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

	class IRHIGeometry
	{
	public:
		/* Destructor */
		virtual ~IRHIGeometry() {}

		/* Set vertex buffer */
		virtual void SetVertexBuffer( IRHIBuffer* InVertexBuffer ) = 0;

		/* Set vertex format */
		virtual void SetVertexFormat( IRHIVertexFormat* InVertexFormat ) = 0;

		/* Set index buffer */
		virtual void SetIndexBuffer( IRHIBuffer* InIndexBuffer, EIndecesType InIndecesType ) = 0;

		/* Get indeces type */
		virtual EIndecesType GetIndecesType() const = 0;
	};
}

#endif // !IRHIGEOMETRY_H
