// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef ENGINETYPES_H
#define ENGINETYPES_H

namespace le
{
	// Base types
	typedef		signed char			int8;
	typedef		short				int16;
	typedef		int					int32;
	typedef		long long			int64;
	typedef		unsigned char		uint8;
	typedef		unsigned short		uint16;
	typedef		unsigned int		uint32;
	typedef		unsigned long long	uint64;
	typedef		unsigned char		byte;

	// RHI types
	template< typename PtrType > class TRefCountPtr;
	class IRHIShader;
	class IRHIBuffer;
	class IRHIVertexFormat;
	class IRHIGeometry;
	class IRHITexture2D;
	class IRHIRenderTarget;

	typedef TRefCountPtr< IRHIShader >					FIRHIShaderRef;	
	typedef const TRefCountPtr< IRHIShader >			FIRHIShaderConstRef;
	typedef TRefCountPtr< IRHIBuffer >					FIRHIBufferRef;
	typedef const TRefCountPtr< IRHIBuffer >			FIRHIBufferConstRef;
	typedef TRefCountPtr< IRHIVertexFormat >			FIRHIVertexFormatRef;
	typedef const TRefCountPtr< IRHIVertexFormat >		FIRHIVertexFormatConstRef;
	typedef TRefCountPtr< IRHIGeometry >				FIRHIGeometryRef;
	typedef const TRefCountPtr< IRHIGeometry >			FIRHIGeometryConstRef;
	typedef TRefCountPtr< IRHITexture2D >				FIRHITexture2DRef;
	typedef const TRefCountPtr< IRHITexture2D >			FIRHITexture2DConstRef;
	typedef TRefCountPtr< IRHIRenderTarget >			FIRHIRenderTargetRef;
	typedef const TRefCountPtr< IRHIRenderTarget >		FIRHIRenderTargetConstRef;

	// Shader types
	class BaseShader;
	typedef TRefCountPtr< BaseShader >					FBaseShaderRef;
	typedef const TRefCountPtr< BaseShader >			FBaseShaderConstRef;

	// Parser types
	class IParserConfig;
	class IParserMaterial;
	class IParserTexture2D;
	class IParserWorld;

	typedef TRefCountPtr< IParserConfig >				FIParserConfigRef;
	typedef const TRefCountPtr< IParserConfig >			FIParserConfigConstRef;
	typedef TRefCountPtr< IParserMaterial >				FIParserMaterialRef;
	typedef const TRefCountPtr< IParserMaterial >		FIParserMaterialConstRef;
	typedef TRefCountPtr< IParserTexture2D >			FIParserTexture2DRef;
	typedef const TRefCountPtr< IParserTexture2D >		FIParserTexture2DConstRef;
	typedef TRefCountPtr< IParserWorld >				FIParserWorldRef;
	typedef const TRefCountPtr< IParserWorld >			FIParserWorldConstRef;

	// Resource types
	class Resource;
	class Config;
	class Material;
	class Texture2D;
	class World;

	typedef TRefCountPtr< Resource >					FResourceRef;
	typedef const TRefCountPtr< Resource >				FResourceConstRef;
	typedef TRefCountPtr< Config >						FConfigRef;
	typedef const TRefCountPtr< Config >				FConfigConstRef;
	typedef TRefCountPtr< Material >					FMaterialRef;
	typedef const TRefCountPtr< Material >				FMaterialConstRef;
	typedef TRefCountPtr< Texture2D >					FTexture2DRef;
	typedef const TRefCountPtr< Texture2D >				FTexture2DConstRef;
	typedef TRefCountPtr< World >						FWorldRef;
	typedef const TRefCountPtr< World >					FWorldConstRef;

	// Component types
	class BaseComponent;
	class CameraComponent;
	class SpriteComponent;
	class TransformComponent;

	typedef TRefCountPtr< BaseComponent >				FBaseComponentRef;
	typedef const TRefCountPtr< BaseComponent >			FBaseComponentConstRef;
	typedef TRefCountPtr< CameraComponent >				FCameraComponentRef;
	typedef const TRefCountPtr< CameraComponent >		FCameraComponentConstRef;
	typedef TRefCountPtr< SpriteComponent >				FSpriteComponentRef;
	typedef const TRefCountPtr< SpriteComponent >		FSpriteComponentConstRef;
	typedef TRefCountPtr< TransformComponent >			FTransformComponentRef;
	typedef const TRefCountPtr< TransformComponent >	FTransformComponentConstRef;

	// Actor types
	class Actor;

	typedef TRefCountPtr< Actor >						FActorRef;
	typedef const TRefCountPtr< Actor >					FActorConstRef;
}

#endif // !ENGINETYPES_H