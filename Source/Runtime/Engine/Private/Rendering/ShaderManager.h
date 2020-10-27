// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>
#include <vector>
#include <string>

#include "Misc/Object.h"
#include "Misc/Types.h"
#include "Misc/Path.h"
#include "Rendering/RHI/IRHIShader.h"

namespace le
{
	struct SShaderPaths
	{
		Path			vertexShaderPath;
		Path			geometryShaderPath;
		Path			pixelShaderPath;
	};

	class ShaderManager : public Object
	{
	public:
		/* Find shader */
		FIRHIShaderRef FindShader( const std::string& InName, const SShaderPaths& InShaderPaths, std::vector< std::string >* InDefines = nullptr, uint32 InFlags = 0 );

		/* Unload shader */
		void UnloadShader( const std::string& InName, uint32 InFlags = 0 );

		/* Unload shaders */
		void UnloadShaders();

	private:
		std::unordered_map< std::string, std::unordered_map< uint32, FIRHIShaderRef > >		shaders;
	};
}

#endif // !SHADERMANAGER_H
