// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IRHISHADER_H
#define IRHISHADER_H

#include <string>
#include <vector>

namespace le
{
	enum EShaderType
	{
		ST_Vertex,
		ST_Geometry,
		ST_Pixel
	};

	class IRHIShader
	{
	public:
		/* Destructor */
		virtual ~IRHIShader() {}

		/* Compile shader */
		virtual bool Compile( const std::string& InCode, EShaderType InShaderType, std::vector< std::string >* InDefines = nullptr, std::string* OutError = nullptr ) = 0;

		/* Link shader */
		virtual bool Link( std::string* OutError = nullptr ) = 0;

		/* Destroy shader */
		virtual void Destroy() = 0;

		/* Set uniform */
		virtual void SetUniform( const std::string& InName, int InValue ) = 0;
		virtual void SetUniform( const std::string& InName, float InValue ) = 0;
		virtual void SetUniform( const std::string& InName, bool InValue ) = 0;

		/* Is loaded shader */
		virtual bool IsLoaded() const = 0;
	};
}

#endif // !IRHISHADER_H
