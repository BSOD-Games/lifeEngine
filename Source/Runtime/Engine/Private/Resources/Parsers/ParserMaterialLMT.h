// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef PARSERMATERIALLMT_H
#define PARSERMATERIALLMT_H

#include <string>
#include <vector>

#include "Resources/Parsers/IParserMaterial.h"

namespace le
{
	class ParserMaterialLMT : public IParserMaterial
	{
	public:
		/* Constructor */
		ParserMaterialLMT();

		/* Destructor */
		~ParserMaterialLMT();

		/* Set file */
		bool SetFile( FFileHandle InFileHandle ) override;

		/* Is oppened */
		FORCEINLINE bool IsOppened() const
		{
			return jsonDocument;
		}

		/* Get supported extensions */
		FORCEINLINE static std::vector< std::string > GetSupportedExtensions()
		{
			return { "lmt" };
		}

		/* Get shader name */
		std::string GetNameShader() const override;

		/* Get shader vars */
		std::vector< ShaderVar > GetShaderVars() const override;

	private:
		/* Clear */
		void Clear();

		void*			jsonDocument;
	};
}

#endif // !PARSERMATERIALLMT_H
