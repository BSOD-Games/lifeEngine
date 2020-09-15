// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef GLSTATE_H
#define GLSTATE_H

#include <unordered_map>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"
#include "Math/Color.h"

namespace le
{
	class GLState
	{
	public:
		/* Bind buffer */
		static void BindBuffer( uint32 InBufferType, uint32 InBuffer );

		/* Bind shader */
		static void BindShader( uint32 InShader );

		/* Bind VAO */
		static void BindVAO( uint32 InVAO );

		/* Bind texture */
		static void BindTexture( uint32 InTextureType, uint32 InTexture );

		/* Bind FBO */
		static void BindFBO( uint32 InFBO );

		/* Set indeces type */
		static void SetIndecesType( uint32 InIndecesType );

		/* Set texture layer */
		static void SetTextureLayer( uint32 InTextureLayer );

		/* Set clear color */
		static void SetClearColor( const SColor& InColor );

		/* Get current buffer */
		FORCEINLINE static uint32 GetBuffer( uint32 InBufferType )
		{
			auto it = buffers.find( InBufferType );
			return it != buffers.end() ? it->second : 0;
		}

		/* Get current texture */
		FORCEINLINE static uint32 GetTexture( uint32 InTextureLayer, uint32* OutTextureType = nullptr )
		{
			auto it = textures.find( InTextureLayer );
			if ( it == textures.end() )		return 0;
			
			if ( OutTextureType )		*OutTextureType = it->second.first;
			return it->second.second;
		}

		/* Get current texture layer */
		FORCEINLINE static uint32 GetTextureLayer()
		{
			return textureLayer;
		}

		/* Get current shader */
		FORCEINLINE static uint32 GetShader()
		{
			return shader;
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

		/* Get current FBO */
		FORCEINLINE static uint32 GetFBO()
		{
			return fbo;
		}

		/* Get clear color */
		FORCEINLINE static const SColor& GetClearColor()
		{
			return clearColor;
		}

	private:
		static uint32														shader;
		static uint32														vao;
		static uint32														indecesType;
		static uint32														textureLayer;
		static uint32														fbo;
		static SColor														clearColor;
		static std::unordered_map< uint32, uint32 >							buffers;
		static std::unordered_map< uint32, std::pair< uint32, uint32 > >	textures;
	};
}

#endif // !GLSTATE_H
