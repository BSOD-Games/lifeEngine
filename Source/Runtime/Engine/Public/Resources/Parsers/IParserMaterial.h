// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IPARSERMATERIAL_H
#define IPARSERMATERIAL_H

#include <string>
#include <vector>

#include "IParser.h"

namespace le
{
	class ShaderVar;

	class IParserMaterial : public IParser
	{
	public:
		/* Destructor */
		virtual ~IParserMaterial() {}

		/* Get shader name */
		virtual std::string GetNameShader() const = 0;

		/* Get shader vars */
		virtual std::vector< ShaderVar > GetShaderVars() const = 0;
	};
}

#endif // !IPARSERMATERIAL_H
