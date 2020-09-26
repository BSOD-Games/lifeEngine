// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef ENGINEGLOBALS_H
#define ENGINEGLOBALS_H

namespace le
{
	class Logger;
	class RenderSystem;
	class ResourceSystem;
	class FileSystem;
	class InputSystem;
	class Window;
	class Engine;

	class ShaderFactory;
	class ParsersTexture2DFactory;
	class ParsersConfigFactory;

	/* Logger */
	extern Logger*						GLogger;

	/* Render system */
	extern RenderSystem*				GRenderSystem;

	/* Resource system */
	extern ResourceSystem*				GResourceSystem;

	/* File system */
	extern FileSystem*					GFileSystem;

	/* Input system */
	extern InputSystem*					GInputSystem;

	/* Window */
	extern Window*						GWindow;

	/* Engine */
	extern Engine*						GEngine;

	/* Shader factory */
	extern ShaderFactory*				GShaderFactory;

	/* Parsers Texture2D factory */
	extern ParsersTexture2DFactory*		GParsersTexture2DFactory;

	/* Parsers Config factory */
	extern ParsersConfigFactory*		GParsersConfigFactory;
}

#endif // !ENGINEGLOBALS_H
