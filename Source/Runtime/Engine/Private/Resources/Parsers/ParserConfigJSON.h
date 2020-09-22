// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef PARSERCONFIGJSON_H
#define PARSERCONFIGJSON_H

#include <string>
#include <vector>

#include "Resources/Parsers/IParserConfig.h"

namespace le
{
	class ParserConfigJSON : public IParserConfig
	{
	public:
		/* Constructor */
		ParserConfigJSON();

		/* Destructor */
		~ParserConfigJSON();

		/* Set file */
		bool SetFile( FFileHandle InFileHandle ) override;

		/* Get supported extensions */
		FORCEINLINE static std::vector< std::string > GetSupportedExtensions()
		{
			return { "json" };
		}

		/* Is oppened */
		FORCEINLINE bool IsOppened() const
		{
			return jsonDocument;
		}

		/* Get groups */
		std::vector< Config::Group > GetGroups() const override;

	private:
		/* Clear */
		void Clear();

		void*			jsonDocument;
	};
}

#endif // !PARSERCONFIGJSON_H
