// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef GLSTATE_H
#define GLSTATE_H

#include <unordered_map>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"

namespace le
{
	class GLState
	{
	public:
		/* Bind buffer */
		static void BindBuffer( uint32 InBufferType, uint32 InBuffer );

		/* Bind shader */
		static void BindShader( uint32 InGpuProgram );

		/* Bind VAO */
		static void BindVAO( uint32 InVAO );

		/* Set indeces type */
		static void SetIndecesType( uint32 InIndecesType );

		/* Get current buffer */
		FORCEINLINE static uint32 GetBuffer( uint32 InBufferType )
		{
			auto it = buffers.find( InBufferType );
			return it != buffers.end() ? it->second : 0;
		}

		/* Get current GPU program */
		FORCEINLINE static uint32 GetGPUProgram()
		{
			return gpuProgram;
		}

		/* Get current VAO */
		FORCEINLINE static uint32 GetVAO()
		{
			return vao;
		}

		/* Get indeces type */
		FORCEINLINE static uint32 GetIndecesType()
		{
			return indecesType;
		}

	private:
		static uint32										gpuProgram;
		static uint32										vao;
		static uint32										indecesType;
		static std::unordered_map< uint32, uint32 >			buffers;
	};
}

#endif // !GLSTATE_H
