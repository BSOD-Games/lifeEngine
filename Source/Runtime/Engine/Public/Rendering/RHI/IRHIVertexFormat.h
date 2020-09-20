// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IRHIVERTEXFORMAT_H
#define IRHIVERTEXFORMAT_H

#include "Misc/Types.h"
#include "Misc/Object.h"

namespace le
{
	class IRHIVertexFormat : public Object
	{
	public:
		/* Destructor */
		virtual ~IRHIVertexFormat() {}

		/* Push float type */
		virtual void PushFloat( uint32 InCount ) = 0;

		/* Push unsigned int type */
		virtual void PushUInt( uint32 InCount ) = 0;

		/* Push unsigned byte type */
		virtual void PushUByte( uint32 InCount ) = 0;

		/* Clear vertex format */
		virtual void Clear() = 0;
	};
}

#endif // !IRHIVERTEXFORMAT_H
