// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef RHISHADER_H
#define RHISHADER_H

#include "Misc/Types.h"
#include "Rendering/RHI/IRHIShader.h"

namespace le
{
	class RHIShader : public IRHIShader
	{
	public:
		/* Constructor */
		RHIShader();

		/* Destructor */
		~RHIShader();

		/* Compile shader */
		bool Compile( const std::string& InCode, EShaderType InShaderType, std::vector< std::string >* InDefines = nullptr, std::string* OutError = nullptr ) override;
		
		/* Link shader */
		bool Link( std::string* OutError = nullptr ) override;
		
		/* Destroy shader */
		void Destroy() override;

		/* Is loaded shader */
		bool IsLoaded() const override;

		/* Get handle shader */
		FORCEINLINE uint32 GetHandle() const
		{
			return gpuProgram;
		}

	private:
		/* Insert defines to shader code */
		void InsertDefinesToCode( std::string& InOutCode, const std::string& InDefinesSection );

		/* Destroy shader */
		void Destroy( EShaderType InShaderType );

		/* Get error compilation */
		void GetErrorCompilation( EShaderType InShaderType, std::string& OutError );

		uint32		vertexShader;
		uint32		geometryShader;
		uint32		pixelShader;
		uint32		gpuProgram;
	};
}

#endif // !RHISHADER_H
