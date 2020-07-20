//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_SYSTEM_H
#define MATERIAL_SYSTEM_H

#include <vector>

#include "engine/imaterialsysteminternal.h"
#include "engine/materialproxyfactory.h"
#include "engine/shaderfactory.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

	class Material;

	//---------------------------------------------------------------------//

    class MaterialSystem : public IMaterialSystemInternal
    {
    public:
		// IMaterialSystem
		virtual IMaterialProxyFactory*			GetMaterialProxyFactory() const;
		virtual IShaderFactory*					GetShaderFactory() const;

		// IMaterialSystemInternal
		virtual bool							Initialize( IEngine* Engine );
		virtual void							Update();
		virtual void							SubmitUpdate( IMaterial* Material );

		// MaterialSystem
		MaterialSystem();
		~MaterialSystem();

	private:
		MaterialProxyFactory				materialProxyFactory;
		ShaderFactory						shaderFactory;

		std::vector< Material* >			updateMaterials;
	};

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_SYSTEM_H
