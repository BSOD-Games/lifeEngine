// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Misc/EngineGlobals.h"
#include "Logging/Logger.h"
#include "System/FileSystem.h"
#include "System/InputSystem.h"
#include "System/Window.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/ShaderFactory.h"
#include "Resources/ResourceSystem.h"
#include "Resources/Parsers/ParsersTexture2DFactory.h"
#include "Resources/Parsers/ParsersConfigFactory.h"
#include "Resources/Parsers/ParsersMaterialFactory.h"
#include "World/World.h"
#include "Engine.h"

le::Logger*							le::GLogger = new le::Logger();
le::IRHI*							le::GRHI = nullptr;
le::RenderSystem*					le::GRenderSystem = new le::RenderSystem();
le::ResourceSystem*					le::GResourceSystem = new le::ResourceSystem();
le::FileSystem*						le::GFileSystem = new le::FileSystem();
le::InputSystem*					le::GInputSystem = new le::InputSystem();
le::Window*							le::GWindow = new le::Window();
le::Engine*							le::GEngine = new le::Engine();
le::ShaderFactory*					le::GShaderFactory = new le::ShaderFactory();
le::World*							le::GWorld = new le::World();
le::ParsersTexture2DFactory*		le::GParsersTexture2DFactory = new le::ParsersTexture2DFactory();
le::ParsersConfigFactory*			le::GParsersConfigFactory = new le::ParsersConfigFactory();
le::ParsersMaterialFactory*			le::GParsersMaterialFactory = new le::ParsersMaterialFactory();
