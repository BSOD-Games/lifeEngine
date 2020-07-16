//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERMATERIAL_LMT_H
#define PARSERMATERIAL_LMT_H

#include "engine/iparsermaterial.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ParserMaterialLMT : public IParserMaterial
	{
	public:
		// IParser
		virtual const char*					GetVersion() const;
		virtual const char*					GetName() const;
		virtual UInt32_t					GetCountFileExtensions() const;
		virtual const char**				GetFileExtensions() const;
		virtual const char*					GetAuthor() const;

		// IParserMaterial
		virtual IMaterial*					Read( const char* Path, IResourceSystem* ResourceSystem, IMaterialSystem* MaterialSystem, IFactory* EngineFactory );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERMATERIAL_LMT_H
