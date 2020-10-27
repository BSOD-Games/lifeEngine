// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef IRHIBUFFER_H
#define IRHIBUFFER_H

#include "Misc/Types.h"
#include "Misc/Object.h"
#include "Misc/RefCounted.h"

namespace le
{
	enum EBufferType
	{
		BT_Unknown,
		BT_Vertex,
		BT_Index
	};

	enum EUsageBufferType
	{
		UBT_Static,
		UBT_Dynamic,
		UBT_Stream
	};

	class IRHIBuffer : public Object, public RefCounted
	{
	public:
		/* Destructor */
		virtual ~IRHIBuffer() {}

		/* Allocate data in buffer */
		virtual void Allocate( const void* InData, uint32 InSize ) = 0;

		/* Update data in buffer */
		virtual void Update( const void* InData, uint32 InSize, uint32 InOffset ) = 0;

		/* Get size buffer */
		virtual uint32 GetSize() const = 0;

		/* Get type buffer */
		virtual EBufferType GetType() const = 0;

		/* Get usage type */
		virtual EUsageBufferType GetUsageType() const = 0;
	};
}

#endif // !IRHIBUFFER_H
