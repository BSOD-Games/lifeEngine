// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RHIVERTEXFORMAT_H
#define RHIVERTEXFORMAT_H

#include <GL/glew.h>
#include <vector>
#include <unordered_set>

#include "Misc/EngineDefines.h"
#include "Rendering/RHI/IRHIVertexFormat.h"

namespace le
{
	class RHIGeometry;

	struct SVertexElement
	{
		FORCEINLINE static uint32 GetSizeOfType( uint32 Type )
		{
			switch ( Type )
			{
			case GL_FLOAT:
			case GL_UNSIGNED_INT:		return 4;
			case GL_UNSIGNED_BYTE:		return 1;
			}

			LIFEENGINE_ASSERT( false );
			return 0;
		}

		uint32			type;
		uint32			count;
		uint8			normalized;
	};

	class RHIVertexFormat : public IRHIVertexFormat
	{
	public:
		/* Constructor */
		RHIVertexFormat();

		/* Destructor */
		~RHIVertexFormat();

		/* Usage in geometry */
		void UsageInGeometry( RHIGeometry* InGeometry );

		/* Notify of destroy geometry */
		void NotifyDestroyGeometry( RHIGeometry* InGeometry );

		/* Push float type */
		void PushFloat( uint32 InCount ) override;

		/* Push unsigned int type */
		void PushUInt( uint32 InCount ) override;

		/* Push unsigned byte type */
		void PushUByte( uint32 InCount ) override;

		/* Clear vertex format */
		void Clear() override;

		/* Get stride */
		const uint32& GetStride() const;

		/* Get elements */
		const std::vector< SVertexElement >& GetElements() const;

	private:
		uint32										stride;
		std::vector< SVertexElement >				elements;
		std::unordered_set< RHIGeometry* >			usagedInGeometries;
	};
}

#endif // !RHIVERTEXFORMAT_H