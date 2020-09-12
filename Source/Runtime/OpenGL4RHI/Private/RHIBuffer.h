// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef RHIBUFFER_H
#define RHIBUFFER_H

#include <unordered_set>

#include "Misc/EngineDefines.h"
#include "Rendering/RHI/IRHIBuffer.h"
#include "GLState.h"

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
		void UsageInGeometry( RHIGeometry* InGeometry );

		/* Notify of destroy geometry */
		void NotifyDestroyGeometry( RHIGeometry* InGeometry );

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

		std::unordered_set< RHIGeometry* >		usagedInGeometries;
	};
}

#endif // !RHIBUFFER_H
