//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <exception>
#include <vector>

#include "common/meshsurface.h"
#include "common/meshdescriptor.h"
#include "engine/ifactory.h"
#include "materialsystem/imaterial.h"
#include "studiorender/studiovertexelement.h"
#include "studiorender/itexture.h"
#include "studiorender/istudiorender.h"
#include "studiorender/studiorendersampler.h"

#include "global.h"
#include "consolesystem.h"
#include "resourcesystem.h"
#include "level.h"

// ------------------------------------------------------------------------------------ //
// Изменить гаму карты освещения
// ------------------------------------------------------------------------------------ //
void Lightmap_ChangeGamma( le::Byte_t* ImageBits, uint32_t Size, float Factor )
{	
	// Проходим по каждому пикселю в карте освещения
	for ( int i = 0; i < Size / 3; i++, ImageBits += 3 )
	{
		float scale = 1.0f, temp = 0.0f;
		float r = 0, g = 0, b = 0;

		// Извлекаем текущий цвет пикселя
		r = ( float ) ImageBits[ 0 ];
		g = ( float ) ImageBits[ 1 ];
		b = ( float ) ImageBits[ 2 ];

		// Умножаем коэффициент на значение RGB, сохраняя при этом соотношение 255
		r = r * Factor / 255.0f;
		g = g * Factor / 255.0f;
		b = b * Factor / 255.0f;

		// Проверяем, прошли ли значения выше самого высокого значения
		if ( r > 1.0f && ( temp = ( 1.0f / r ) ) < scale ) scale = temp;
		if ( g > 1.0f && ( temp = ( 1.0f / g ) ) < scale ) scale = temp;
		if ( b > 1.0f && ( temp = ( 1.0f / b ) ) < scale ) scale = temp;

		// Получаем масштаб для этого пикселя и умножаем его на значения пикселей
		scale *= 255.0f;
		r *= scale;	g *= scale;	b *= scale;

		// Запоминаем новый цвет пикселя
		ImageBits[ 0 ] = ( le::Byte_t ) r;
		ImageBits[ 1 ] = ( le::Byte_t ) g;
		ImageBits[ 2 ] = ( le::Byte_t ) b;
	}
}

// ------------------------------------------------------------------------------------ //
// Создать карту освещения
// ------------------------------------------------------------------------------------ //
le::ITexture* Lightmap_Create( le::Byte_t* ImageBits, uint32_t Width, uint32_t Height )
{
	le::ITexture*			texture = ( le::ITexture* ) le::g_studioRender->GetFactory()->Create( TEXTURE_INTERFACE_VERSION );
	if ( !texture )		return nullptr;

	texture->Initialize( le::TT_2D, le::IF_RGB, Width, Height );
	texture->Bind();
	texture->Append( ImageBits );
	texture->GenerateMipmaps();

	le::StudioRenderSampler			sampler;
	sampler.minFilter = le::SF_LINEAR_MIPMAP_LINEAR;
	sampler.magFilter = le::SF_LINEAR;
	texture->SetSampler( sampler );

	texture->Unbind();

	return texture;
}

// ------------------------------------------------------------------------------------ //
// Загрузить уровень
// ------------------------------------------------------------------------------------ //
bool le::Level::Load( const char* Path, IFactory* GameFactory )
{
	std::ifstream			file( Path, std::ios::binary );
	
	try
	{
		if ( !file.is_open() )		throw std::exception( "Level not found" );

		BSPHeader						bspHeader;
		BSPLump							bspLumps[ BL_MAX_LUMPS ];
		BSPEntities						bspEntities;

		std::vector< BSPVertex >		arrayVerteces;
		std::vector< UInt32_t >			arrayIndices;
		std::vector< BSPFace >			arrayFaces;
		std::vector < BSPLightmap >		arrayBspLightmaps;
		std::vector< IMaterial* >		arrayMaterials;
		std::vector< ITexture* >		arrayLightmaps;
		std::vector< MeshSurface >		arrayMeshSurfaces;

		// Читаем заголовок и куски файла
		file.read( ( char* ) &bspHeader, sizeof( BSPHeader ) );
		file.read( ( char* ) &bspLumps, BL_MAX_LUMPS * sizeof( BSPLump ) );

		if ( std::string( bspHeader.strID, 4 ) != "IBSP" || bspHeader.version != 46 )
			throw std::exception( "Not supported format bsp or version" );

		// Вычисляем необходимые размеры массивов прд данные
		bspEntities.entitiesData = new char[ bspLumps[ BL_ENTITIES ].length ];

		arrayVerteces.resize( bspLumps[ BL_VERTICES ].length / sizeof( BSPVertex ) );
		arrayIndices.resize( bspLumps[ BL_INDICES ].length / sizeof( UInt32_t ) );
		arrayBspTextures.resize( bspLumps[ BL_TEXTURES ].length / sizeof( BSPTexture ) );
		arrayFaces.resize( bspLumps[ BL_FACES ].length / sizeof( BSPFace ) );
		arrayBspLightmaps.resize( bspLumps[ BL_LIGHT_MAPS ].length / sizeof( BSPLightmap ) );

		// Считываем информацию энтити-объектов
		file.seekg( bspLumps[ BL_ENTITIES ].offset, std::ios::beg );
		file.read( ( char* ) &bspEntities.entitiesData[ 0 ], bspLumps[ BL_ENTITIES ].length );	

		// Считываем вершины
		file.seekg( bspLumps[ BL_VERTICES ].offset, std::ios::beg );

		for ( UInt32_t index = 0, count = arrayVerteces.size(); index < count; ++index )
		{
			file.read( ( char* ) &arrayVerteces[ index ], sizeof( BSPVertex ) );
			BSPVertex*			vertex = &arrayVerteces[ index ];

			// Меняем значения Y и Z, и отрицаем новый Z, чтобы Y был вверх.
			float Temp = vertex->position.y;
			vertex->position.y = vertex->position.z;
			vertex->position.z = -Temp;

			Temp = vertex->normal.y;
			vertex->normal.y = vertex->normal.z;
			vertex->normal.z = -Temp;

			vertex->textureCoord.y = -vertex->textureCoord.y;
		}

		// Считываем индексы вершин
		file.seekg( bspLumps[ BL_INDICES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayIndices[ 0 ], arrayIndices.size() * sizeof( UInt32_t ) );

		// Считываем информацию о текстурах
		file.seekg( bspLumps[ BL_TEXTURES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayBspTextures[ 0 ], arrayBspTextures.size() * sizeof( BSPTexture ) );
		
		// Считываем информацию о поверхностях
		file.seekg( bspLumps[ BL_FACES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayFaces[ 0 ], arrayFaces.size() * sizeof( BSPFace ) );

		// Считываем карту освещения
		if ( arrayBspLightmaps.size() == 0 )
		{
			arrayLightmaps.push_back( Lightmap_Create( nullptr, 1, 1 ) );

			for ( UInt32_t index = 0, count = arrayFaces.size(); index < count; ++index )
				arrayFaces[ index ].lightmapID = 0;
		}
		else
		{
			file.seekg( bspLumps[ BL_LIGHT_MAPS ].offset, std::ios::beg );

			for ( UInt32_t index = 0, count = arrayBspLightmaps.size(); index < count; ++index )
			{
				file.read( ( char* ) &arrayBspLightmaps[ index ], sizeof( BSPLightmap ) );
				arrayLightmaps.push_back( Lightmap_Create( ( Byte_t* ) arrayBspLightmaps[ index ].imageBits, 128, 128 ) );
			}
		}

		// Загружаем все текстуры
		for ( UInt32_t index = 0, count = arrayBspTextures.size(); index < count; ++index )
		{
			BSPTexture*			bspTexture = &arrayBspTextures[ index ];
			arrayMaterials.push_back( g_resourceSystem->LoadMaterial( bspTexture->strName, ( std::string( bspTexture->strName ) + ".lmt" ).c_str() ) );
		}

		// Инициализируем плоскости
		for ( UInt32_t index = 0, count = arrayFaces.size(); index < count; ++index )
		{
			BSPFace*		bspFace = &arrayFaces[ index ];
			MeshSurface		meshSurface;
		
			meshSurface.materialID = bspFace->textureID;
			meshSurface.lightmapID = bspFace->lightmapID;
			meshSurface.startVertexIndex = bspFace->startVertIndex;
			meshSurface.startIndex = bspFace->startIndex;
			meshSurface.countIndeces = bspFace->numOfIndices;
			arrayMeshSurfaces.push_back( meshSurface );
		}
	
		// Создаем описание для формата вершин
		std::vector< le::StudioVertexElement >			vertexElements =
		{
			{ 3, VET_FLOAT },
			{ 2, VET_FLOAT },
			{ 2, VET_FLOAT },
			{ 3, VET_FLOAT },
			{ 4, VET_UNSIGNED_BYTE }
		};
		
		// Создаем описание меша для загрузки его в модуль рендера
		le::MeshDescriptor				meshDescriptor;
		meshDescriptor.countIndeces = arrayIndices.size();
		meshDescriptor.countMaterials = arrayMaterials.size();
		meshDescriptor.countLightmaps = arrayLightmaps.size();
		meshDescriptor.countSurfaces = arrayMeshSurfaces.size();
		meshDescriptor.sizeVerteces = arrayVerteces.size() * sizeof( BSPVertex );

		meshDescriptor.indeces = arrayIndices.data();
		meshDescriptor.materials = arrayMaterials.data();
		meshDescriptor.lightmaps = arrayLightmaps.data();
		meshDescriptor.surfaces = arrayMeshSurfaces.data();
		meshDescriptor.verteces = arrayVerteces.data();

		meshDescriptor.primitiveType = le::PT_TRIANGLES;
		meshDescriptor.countVertexElements = vertexElements.size();
		meshDescriptor.vertexElements = vertexElements.data();
		
		// Загружаем меш в GPU
		mesh = ( le::IMesh* ) g_studioRender->GetFactory()->Create( MESH_INTERFACE_VERSION );
		if ( !mesh )				std::exception( "Mesh interface version not found in factory studiorender" );
	
		mesh->Create( meshDescriptor );
		if ( !mesh->IsCreated() ) std::exception( "Mesh not created" );
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( "Level not loaded: %s", Exception.what() );
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Обновить уровень
// ------------------------------------------------------------------------------------ //
void le::Level::Update( UInt32_t DeltaTime )
{
	if ( !mainCamera || !mesh || !mesh->IsCreated() ) return;

	g_studioRender->BeginScene( mainCamera );
		g_studioRender->SetDepthTestEnabled( true );
		g_studioRender->SetCullFaceEnabled( true );
		g_studioRender->SetCullFaceType( CT_FRONT );

		g_studioRender->SubmitMesh( mesh, Matrix4x4_t( 1.f ) );
	g_studioRender->EndScene();
}

// ------------------------------------------------------------------------------------ //
// Очистить уровень
// ------------------------------------------------------------------------------------ //
void le::Level::Clear()
{}

// ------------------------------------------------------------------------------------ //
// Добавить камеру на уровень
// ------------------------------------------------------------------------------------ //
void le::Level::AddCamera( ICamera* Camera )
{
	mainCamera = ( le::Camera* ) Camera;
}

// ------------------------------------------------------------------------------------ //
// Удалить камеру с уровня
// ------------------------------------------------------------------------------------ //
void le::Level::RemoveCamera( ICamera* Camera )
{
	mainCamera = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Удалить камеру с уровня
// ------------------------------------------------------------------------------------ //
void le::Level::RemoveCamera( UInt32_t Index )
{
	mainCamera = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Загружен ли уровень
// ------------------------------------------------------------------------------------ //
bool le::Level::IsLoaded() const
{
	return mesh;
}

// ------------------------------------------------------------------------------------ //
// Получить название формата
// ------------------------------------------------------------------------------------ //
const char* le::Level::GetNameFormat() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Получить количество камер на уровне
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Level::GetCountCameras() const
{
	return mainCamera ? 1 : 0;
}

// ------------------------------------------------------------------------------------ //
// Получить камеру
// ------------------------------------------------------------------------------------ //
le::ICamera* le::Level::GetCamera( UInt32_t Index ) const
{
	return mainCamera;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Level::Level() :
	mesh( nullptr ),
	mainCamera( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Level::~Level()
{}
