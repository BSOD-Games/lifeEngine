//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>

#include "engine/lifeengine.h"
#include "global.h"
#include "texture.h"

// ------------------------------------------------------------------------------------ //
// Конвертировать формат тип формата изображения движка в формат OpenGL'a
// ------------------------------------------------------------------------------------ //
inline le::UInt32_t ConvertEngineImageFormat_To_OpenGLPixelFormat( le::IMAGE_FORMAT ImageFormat )
{
	switch ( ImageFormat )
	{
	case le::IF_RGBA:		return GL_RGBA;
	case le::IF_RGB:		return GL_RGB;
	default:				return 0;
	}
}

// ------------------------------------------------------------------------------------ //
// Создать текстуру
// ------------------------------------------------------------------------------------ //
void le::Texture::Initialize( TEXTURE_TYPE TextureType, IMAGE_FORMAT ImageFormat, UInt32_t Width, UInt32_t Height, UInt32_t CountMipmap )
{
	if ( handle ) Delete();

	glGenTextures( 1, &handle );

	type = TextureType;	
	imageFormat = ImageFormat;
	width = Width;
	height = Height;
	countMipmaps = CountMipmap;
	isCreated = true;
}

// ------------------------------------------------------------------------------------ //
// Удалить текстуру
// ------------------------------------------------------------------------------------ //
void le::Texture::Delete()
{
	LIFEENGINE_ASSERT( handle );

	glDeleteTextures( 1, &handle );

	width = 0;
	height = 0;
	handle = 0;
	countMipmaps = 0;
	isCreated = false;
}

// ------------------------------------------------------------------------------------ //
// Активировать текстуру
// ------------------------------------------------------------------------------------ //
void le::Texture::Bind( UInt32_t Layer )
{
	LIFEENGINE_ASSERT( handle );
	
	glActiveTexture( GL_TEXTURE0 + Layer );
	glBindTexture( GL_TEXTURE_2D, handle );	
	layer = Layer;
}

// ------------------------------------------------------------------------------------ //
// Деактивировать текстуру
// ------------------------------------------------------------------------------------ //
void le::Texture::Unbind()
{
	LIFEENGINE_ASSERT( handle );

	glActiveTexture( GL_TEXTURE0 + layer );
	glBindTexture( GL_TEXTURE_2D, 0 );
	layer = 0;
}

// ------------------------------------------------------------------------------------ //
// Сгенерировать лоды для текстур
// ------------------------------------------------------------------------------------ //
void le::Texture::GenerateMipmaps()
{
	LIFEENGINE_ASSERT( handle && layer );

	glGenerateMipmap( GL_TEXTURE_2D );
	countMipmaps = floor( log2( glm::max( width, height ) ) );
}

// ------------------------------------------------------------------------------------ //
// Загрузить данные текстуры
// ------------------------------------------------------------------------------------ //
void le::Texture::Append( const UInt8_t* Data, UInt32_t MipmapLevel )
{
	LIFEENGINE_ASSERT( MipmapLevel >= 0 && MipmapLevel < countMipmaps && handle && layer );

	UInt32_t		format = ConvertEngineImageFormat_To_OpenGLPixelFormat( imageFormat );
	UInt32_t		width = GetWidth( MipmapLevel );
	UInt32_t		height = GetHeight( MipmapLevel );

	glTexImage2D( GL_TEXTURE_2D, MipmapLevel, format, width, height, 0, format, GL_UNSIGNED_BYTE, Data );
}

// ------------------------------------------------------------------------------------ //
// Обновить данные текстуры
// ------------------------------------------------------------------------------------ //
void le::Texture::Update( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height, const UInt8_t* Data, UInt32_t MipmapLevel )
{
	LIFEENGINE_ASSERT( MipmapLevel >= 0 && MipmapLevel < countMipmaps && Width <= GetWidth( MipmapLevel ) && Height <= GetHeight( MipmapLevel ) && handle && layer );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );	
	glTexSubImage2D( GL_TEXTURE_2D, MipmapLevel, X, Y, Width, Height, ConvertEngineImageFormat_To_OpenGLPixelFormat( imageFormat ), GL_UNSIGNED_BYTE, Data );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
}

// ------------------------------------------------------------------------------------ //
// Создана ли текстура
// ------------------------------------------------------------------------------------ //
bool le::Texture::IsCreated() const
{
	return isCreated;
}

// ------------------------------------------------------------------------------------ //
// Получить ширину текстуры
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Texture::GetWidth( UInt32_t MipmapLevel ) const
{
	LIFEENGINE_ASSERT( MipmapLevel >= 0 && MipmapLevel < countMipmaps );

	UInt32_t			width = this->width >> MipmapLevel;
	return width >= 1 ? width : 1;
}

// ------------------------------------------------------------------------------------ //
// Получить высоту текстуры
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Texture::GetHeight( UInt32_t MipmapLevel ) const
{
	LIFEENGINE_ASSERT( MipmapLevel >= 0 && MipmapLevel < countMipmaps );

	UInt32_t			height = this->height >> MipmapLevel;
	return height >= 1 ? height : 1;
}

// ------------------------------------------------------------------------------------ //
// Получить количество лодов текстуры
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Texture::GetCountMipmaps() const
{
	return countMipmaps;
}

// ------------------------------------------------------------------------------------ //
// Получить тип текстуры
// ------------------------------------------------------------------------------------ //
le::TEXTURE_TYPE le::Texture::GetType() const
{
	return type;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Texture::Texture() :
	isCreated( false ),
	handle( 0 ),
	width( 0 ),
	height( 0 ),
	countMipmaps( 0 ),
	layer( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Texture::~Texture()
{
	Delete();
}
