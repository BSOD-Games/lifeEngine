// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RHIBUFFER_H
#define RHIBUFFER_H

#include <vector>

#include "Misc/EngineDefines.h"
#include "Rendering/RHI/IRHIBuffer.h"
#include "GLState.h"
#include "Types.h"

namespace le
{
	class RHIGeometry;

	class RHIBuffer : public IRHIBuffer
	{
	public:
		/* Constructor */
		RHIBuffer( EBufferType InBufferType, EUsageBufferType InUsageBufferType );

		/* Destructor */
		~RHIBuffer();

		/* Bind buffer */
		FORCEINLINE void Bind()
		{
			LIFEENGINE_ASSERT( handle > 0 );
			GLState::BindBuffer( type, handle );
		}
		
		/* Unbind buffer */
		FORCEINLINE void Unbind()
		{
			GLState::BindBuffer( type, 0 );
		}

		/* Allocate data in buffer */
		void Allocate( const void* InData, uint32 InSize ) override;

		/* Update data in buffer */
		void Update( const void* InData, uint32 InSize, uint32 InOffset ) override;

		/* Usage in geometry */
		void UsageInGeometry( FRHIGeometryConstRef& InGeometry );

		/* Notify of destroy geometry */
		void NotifyDestroyGeometry( FRHIGeometryConstRef& InGeometry );

		/* Get handle buffer */
		FORCEINLINE uint32 GetHandle() const
		{
			return handle;
		}

		/* Get size buffer */
		uint32 GetSize() const override;

		/* Get type buffer */
		EBufferType GetType() const override;

		/* Get usage type */
		EUsageBufferType GetUsageType() const override;

	private:
		uint32		type;
		uint32		usageType;
		uint32		size;
		uint32		handle;

		std::vector< FRHIGeometryRef >		usagedInGeometries;
	};
}

#endif // !RHIBUFFER_H
