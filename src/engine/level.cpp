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
#include <unordered_map>

#include "common/meshsurface.h"
#include "common/meshdescriptor.h"
#include "engine/ifactory.h"
#include "engine/ientity.h"
#include "engine/ientity.h"
#include "studiorender/studiovertexelement.h"
#include "studiorender/itexture.h"
#include "studiorender/istudiorender.h"
#include "studiorender/studiorendersampler.h"
#include "studiorender/ipointlight.h"
#include "studiorender/ispotlight.h"
#include "studiorender/idirectionallight.h"
#include "physics/iphysicssystem.h"
#include "physics/ibody.h"
#include "physics/icollider.h"
#include "physics/shapeconvexhulldescriptor.h"

#include "global.h"
#include "consolesystem.h"
#include "resourcesystem.h"
#include "level.h"
#include "model.h"
#include "material.h"
#include "sprite.h"

// ------------------------------------------------------------------------------------ //
// Is point inside planes
// ------------------------------------------------------------------------------------ //
bool BSP_IsPointInsidePlanes( const std::vector< le::BSPPlane >& PlaneEquations, const le::Vector3D_t& Point, float Margin )
{
	le::UInt32_t			countPlanesEquations = PlaneEquations.size();
	for ( le::UInt32_t i = 0; i < countPlanesEquations; ++i )
	{
		const le::BSPPlane&			n1 = PlaneEquations[ i ];
		float		distance =  glm::dot( Point, n1.normal ) + n1.distance - Margin;
		if ( distance > 0.f )		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Get verteces from plane equations
// ------------------------------------------------------------------------------------ //
void BSP_GetVertecesFromPlaneEquations( const std::vector< le::BSPPlane >& PlaneEquations, std::vector< le::Vector3D_t >& Verteces )
{
	le::UInt32_t			countPlanesEquations = PlaneEquations.size();
	for ( le::UInt32_t i = 0; i < countPlanesEquations; ++i )
	{
		const le::BSPPlane&			n1 = PlaneEquations[ i ];
		for ( le::UInt32_t j = i+1; j < countPlanesEquations; ++j )
		{
			const le::BSPPlane&		n2 = PlaneEquations[ j ];
			for ( le::UInt32_t k = j+1; k < countPlanesEquations; ++k )
			{
				const le::BSPPlane&	n3 = PlaneEquations[ k ];

				le::Vector3D_t		n2n3 = glm::cross( n2.normal, n3.normal );
				le::Vector3D_t		n3n1 = glm::cross( n3.normal, n1.normal );
				le::Vector3D_t		n1n2 = glm::cross( n1.normal, n2.normal );

				if ( glm::length2( n2n3 ) > 0.0001f &&
					 glm::length2( n3n1 ) > 0.0001f &&
					 glm::length2( n1n2 ) > 0.0001f )
				{
					// Point P out of 3 plane equations:

					//	d1 ( N2 * N3 ) + d2 ( N3 * N1 ) + d3 ( N1 * N2 )
					//P =  -------------------------------------------------------------------------
					//   N1 . ( N2 * N3 )

					float		quotient = glm::dot( n1.normal, n2n3 );
					if ( glm::abs( quotient ) > 0.000001f )
					{
						quotient = -1.f / quotient;
						n2n3 *= n1.distance;
						n3n1 *= n2.distance;
						n1n2 *= n3.distance;

						le::Vector3D_t			potentialVertex = n2n3;
						potentialVertex += n3n1;
						potentialVertex += n1n2;
						potentialVertex *= quotient;

						// Check if inside, and replace supportingVertexOut if needed
						if ( BSP_IsPointInsidePlanes( PlaneEquations, potentialVertex, 0.01f ) )
							Verteces.push_back( potentialVertex );
					}
				}
			}
		}
	}
}

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
		if ( r > 1.0f && ( temp = ( 1.0f / r ) ) < scale )		scale = temp;
		if ( g > 1.0f && ( temp = ( 1.0f / g ) ) < scale )		scale = temp;
		if ( b > 1.0f && ( temp = ( 1.0f / b ) ) < scale )		scale = temp;

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
	le::ITexture* texture = ( le::ITexture* ) le::g_studioRender->GetFactory()->Create( TEXTURE_INTERFACE_VERSION );
	if ( !texture )		return nullptr;

	texture->Initialize( le::TT_2D, le::IF_RGB_8UNORM, Width, Height );
	texture->Bind();
	texture->Append( ImageBits );
	texture->GenerateMipmaps();

	le::StudioRenderSampler			sampler;
	sampler.minFilter = le::SF_LINEAR_MIPMAP_LINEAR;
	sampler.magFilter = le::SF_LINEAR;
	texture->SetSampler( sampler );

	texture->Unbind();
	texture->IncrementReference();

	return texture;
}

// ------------------------------------------------------------------------------------ //
// Загрузить уровень
// ------------------------------------------------------------------------------------ //
bool le::Level::Load( const char* Path, IFactory* GameFactory )
{
	try
	{
		std::ifstream			file( Path, std::ios::binary );

		if ( !file.is_open() )		throw std::runtime_error( "Level not found" );
		if ( isLoaded )				Clear();

		BSPHeader						bspHeader;
		BSPLump							bspLumps[ BL_MAX_LUMPS ];
		BSPEntities						bspEntities;

		std::vector< BSPVertex >		arrayVerteces;
		std::vector< UInt32_t >			arrayIndices;
		std::vector< BSPFace >			arrayFaces;
		std::vector < BSPLightmap >		arrayBspLightmaps;
		std::vector< BSPTexture >		arrayBspTextures;
		std::vector< BSPModel >			arrayBspModels;
		std::vector< IMaterial* >		arrayMaterials;
		std::vector< MeshSurface >		arrayMeshSurfaces;
		std::vector< BSPBrush >			arrayBrushes;
		std::vector< BSPBrushSide >		arrayBrusheSides;
		std::vector< int >				arrayLeafsFaces;
		std::vector< int >				arrayLeafsBrushes;

		// Читаем заголовок и куски файла
		file.read( ( char* ) &bspHeader, sizeof( BSPHeader ) );
		file.read( ( char* ) &bspLumps, BL_MAX_LUMPS * sizeof( BSPLump ) );

		if ( std::string( bspHeader.strID, 4 ) != "IBSP" || bspHeader.version != 46 )
			throw std::runtime_error( "Not supported format bsp or version" );

		// Вычисляем необходимые размеры массивов прд данные
		bspEntities.entitiesData = new char[ bspLumps[ BL_ENTITIES ].length ];

		arrayVerteces.resize( bspLumps[ BL_VERTICES ].length / sizeof( BSPVertex ) );
		arrayIndices.resize( bspLumps[ BL_INDICES ].length / sizeof( UInt32_t ) );
		arrayFaces.resize( bspLumps[ BL_FACES ].length / sizeof( BSPFace ) );
		arrayBspLightmaps.resize( bspLumps[ BL_LIGHT_MAPS ].length / sizeof( BSPLightmap ) );
		arrayLeafsFaces.resize( bspLumps[ BL_LEAF_FACES ].length / sizeof( int ) );
		arrayBspTextures.resize( bspLumps[ BL_TEXTURES ].length / sizeof( BSPTexture ) );
		arrayBspModels.resize( bspLumps[ BL_MODELS ].length / sizeof( BSPModel ) );
		arrayBspLeafs.resize( bspLumps[ BL_LEAFS ].length / sizeof( BSPLeaf ) );
		arrayBspNodes.resize( bspLumps[ BL_NODES ].length / sizeof( BSPNode ) );
		arrayBspPlanes.resize( bspLumps[ BL_PLANES ].length / sizeof( BSPPlane ) );
		arrayLeafsBrushes.resize( bspLumps[ BL_LEAF_BRUSHES ].length / sizeof( int ) );
		arrayBrushes.resize( bspLumps[ BL_BRUSHES ].length / sizeof( BSPBrush ) );
		arrayBrusheSides.resize( bspLumps[ BL_BRUSH_SIDES ].length / sizeof( BSPBrushSide ) );

		// Считываем информацию энтити-объектов
		file.seekg( bspLumps[ BL_ENTITIES ].offset, std::ios::beg );
		file.read( ( char* ) &bspEntities.entitiesData[ 0 ], bspLumps[ BL_ENTITIES ].length );

		// Считываем вершины
		file.seekg( bspLumps[ BL_VERTICES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayVerteces[ 0 ], arrayVerteces.size() * sizeof( BSPVertex ) );

		for ( UInt32_t index = 0, count = arrayVerteces.size(); index < count; ++index )
		{
			BSPVertex* vertex = &arrayVerteces[ index ];

			// Меняем значения Y и Z, и отрицаем новый Z, чтобы Y был вверх.
			float temp = vertex->position.y;
			vertex->position.y = vertex->position.z;
			vertex->position.z = -temp;

			temp = vertex->normal.y;
			vertex->normal.y = vertex->normal.z;
			vertex->normal.z = -temp;

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

		// Считываем информацию о ветках BSP дерева
		file.seekg( bspLumps[ BL_NODES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayBspNodes[ 0 ], arrayBspNodes.size() * sizeof( BSPNode ) );

		// Считываем информацию о моделях уровня
		file.seekg( bspLumps[ BL_MODELS ].offset, std::ios::beg );
		file.read( ( char* ) &arrayBspModels[ 0 ], arrayBspModels.size() * sizeof( BSPModel ) );

		for ( UInt32_t index = 0, count = arrayBspModels.size(); index < count; ++index )
		{
			BSPModel* bspModel = &arrayBspModels[ index ];

			float			temp = bspModel->min.y;
			bspModel->min.y = bspModel->min.z;
			bspModel->min.z = -temp;

			temp = bspModel->max.y;
			bspModel->max.y = bspModel->max.z;
			bspModel->max.z = -temp;
		}

		// Считываем информацию о листьях BSP дерева
		file.seekg( bspLumps[ BL_LEAFS ].offset, std::ios::beg );
		file.read( ( char* ) &arrayBspLeafs[ 0 ], arrayBspLeafs.size() * sizeof( BSPLeaf ) );

		// Меняем ось Z и Y местами
		for ( UInt32_t index = 0, count = arrayBspLeafs.size(); index < count; ++index )
		{
			BSPLeaf* bspLeaf = &arrayBspLeafs[ index ];

			int			temp = bspLeaf->min.y;
			bspLeaf->min.y = bspLeaf->min.z;
			bspLeaf->min.z = -temp;

			temp = bspLeaf->max.y;
			bspLeaf->max.y = bspLeaf->max.z;
			bspLeaf->max.z = -temp;
		}

		// Считываем информацию о ветках BSP дерева
		file.seekg( bspLumps[ BL_LEAF_FACES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayLeafsFaces[ 0 ], arrayLeafsFaces.size() * sizeof( int ) );

		// Reading info by leaf brushes
		file.seekg( bspLumps[ BL_LEAF_BRUSHES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayLeafsBrushes[ 0 ], arrayLeafsBrushes.size() * sizeof( int ) );

		// Reading brushes
		file.seekg( bspLumps[ BL_BRUSHES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayBrushes[ 0 ], arrayBrushes.size() * sizeof( BSPBrush ) );

		// Reading brush sides
		file.seekg( bspLumps[ BL_BRUSH_SIDES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayBrusheSides[ 0 ], arrayBrusheSides.size() * sizeof( BSPBrushSide ) );

		// Убираем индексы фейсов относящиеся к движ. части уровня
		int			faceStart = arrayBspModels[ 0 ].startFaceIndex;
		int			faceEnd = arrayBspModels[ 0 ].startFaceIndex + arrayBspModels[ 0 ].numOfFaces - 1;

		for ( UInt32_t index = 0, count = arrayBspLeafs.size(); index < count; ++index )
		{
			BSPLeaf* bspLeaf = &arrayBspLeafs[ index ];
			int					leafFace = arrayBspLeafsFaces.size();

			for ( int j = 0; j < bspLeaf->numOfLeafFaces; j++ )
			{
				int			faceIndex = arrayLeafsFaces[ bspLeaf->leafFace + j ];

				if ( faceIndex >= faceStart && faceIndex <= faceEnd )
					arrayBspLeafsFaces.push_back( faceIndex );
			}

			bspLeaf->leafFace = leafFace;
			bspLeaf->numOfLeafFaces = arrayBspLeafsFaces.size() - leafFace;
		}

		// Считываем информацию о секущих плоскостях BSP дерева
		file.seekg( bspLumps[ BL_PLANES ].offset, std::ios::beg );
		file.read( ( char* ) &arrayBspPlanes[ 0 ], arrayBspPlanes.size() * sizeof( BSPPlane ) );

		// Меняем ось Z и Y местами
		for ( UInt32_t index = 0, count = arrayBspPlanes.size(); index < count; ++index )
		{
			BSPPlane* plane = &arrayBspPlanes[ index ];

			float			temp = plane->normal.y;
			plane->normal.y = plane->normal.z;
			plane->normal.z = -temp;
		}

		// Считываем информацию о видимой геометрии
		if ( bspLumps[ BL_VIS_DATA ].length )
		{
			file.seekg( bspLumps[ BL_VIS_DATA ].offset, std::ios::beg );
			file.read( ( char* ) &visData.numOfClusters, sizeof( int ) );
			file.read( ( char* ) &visData.bytesPerCluster, sizeof( int ) );

			UInt32_t		size = visData.numOfClusters * visData.bytesPerCluster;
			visData.bitsets = new Byte_t[ size ];

			file.read( ( char* ) &visData.bitsets[ 0 ], size * sizeof( Byte_t ) );
		}
		else
			visData.bitsets = nullptr;

		// Считываем карту освещения
		if ( arrayBspLightmaps.size() == 0 )
		{
			Byte_t				whiteLightmap[ 3 ] = { 255, 255,255 };
			arrayLightmaps.push_back( Lightmap_Create( whiteLightmap, 1, 1 ) );

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
			BSPTexture* bspTexture = &arrayBspTextures[ index ];
			IMaterial*			material = g_resourceSystem->LoadMaterial( bspTexture->strName, ( std::string( bspTexture->strName ) + ".lmt" ).c_str() );
			if ( !material )	material = g_resourceSystem->LoadMaterial( "Error", "materials/error.lmt" );
			arrayMaterials.push_back( material );
		}

		// Created physics bodyes
		IFactory*				physicsFactory = g_physicsSystem->GetFactory();
		for ( UInt32_t index = 0, countModels = arrayBspModels.size(); index < countModels; ++index )
		{
			BSPModel&					bspModel = arrayBspModels[ index ];
			ICollider*					collider = ( ICollider* ) physicsFactory->Create( COLLIDER_INTERFACE_VERSION );
			if ( !collider ) return false;

			for ( UInt32_t indexBrush = 0; indexBrush < bspModel.numOfBrushes; ++indexBrush )
			{
				bool						isValidBrush = false;
				std::vector< BSPPlane >		planeEquations;
				BSPBrush&					bspBrush = arrayBrushes[ bspModel.startBrushIndex + indexBrush ];

				if ( bspBrush.textureID == -1 )										continue;
				if ( !( arrayBspTextures[ bspBrush.textureID ].type & 1 ) )			continue;

				for ( UInt32_t indexSide = 0; indexSide < bspBrush.numOfBrushSides; ++indexSide )
				{
					BSPBrushSide&			bspBrushSide = arrayBrusheSides[ bspBrush.brushSide + indexSide ];
					BSPPlane				bspPlane = arrayBspPlanes[ bspBrushSide.plane ];

					bspPlane.distance = -bspPlane.distance;
					planeEquations.push_back( bspPlane );
					isValidBrush = true;
				}

				if ( !isValidBrush ) continue;

				std::vector< Vector3D_t >		verteces;
				BSP_GetVertecesFromPlaneEquations( planeEquations, verteces );
				if ( verteces.empty() )		continue;

				ShapeConvexHullDescriptor			shapeConvexHullDescriptor;
				shapeConvexHullDescriptor.verteces = verteces.data();
				shapeConvexHullDescriptor.countVerteces = verteces.size();
				collider->AddShape( shapeConvexHullDescriptor, Matrix4x4_t( 1.f ) );
			}

			if ( collider->GetCountShapes() == 0 )
			{
				collider->Release();
				continue;
			}

			IBody*		body = ( IBody* ) physicsFactory->Create( BODY_INTERFACE_VERSION );
			if ( !body )		return false;

			body->Create( collider, 0.0, Vector3D_t( 0.f, 0.f, 0.f ) );
			body->IncrementReference();

			g_physicsSystem->AddBody( body );
			arrayBodes.push_back( body );
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

		meshDescriptor.min = arrayBspModels[ 0 ].min;
		meshDescriptor.max = arrayBspModels[ 0 ].max;
		meshDescriptor.primitiveType = le::PT_TRIANGLES;
		meshDescriptor.countVertexElements = vertexElements.size();
		meshDescriptor.vertexElements = vertexElements.data();

		// Загружаем меш в GPU
		mesh = ( le::IMesh* ) g_studioRender->GetFactory()->Create( MESH_INTERFACE_VERSION );
		if ( !mesh )				throw std::runtime_error( "Interfece mesh with required version not found in factory studiorender" );

		mesh->IncrementReference();
		mesh->Create( meshDescriptor );
		if ( !mesh->IsCreated() )	throw std::runtime_error( "Mesh level not created" );

		// Добавляем на уровень модели
		for ( UInt32_t index = 0, count = arrayBspModels.size(); index < count; ++index )
		{
			Model*		model = new Model();
			BSPModel&	bspModel = arrayBspModels[ index ];

			model->IncrementReference();
			model->SetMesh( mesh );
			model->SetMin( bspModel.min );
			model->SetMax( bspModel.max );
			model->SetStartFace( bspModel.startFaceIndex );
			model->SetCountFace( bspModel.numOfFaces );
			arrayModels.push_back( model );
		}

		facesDraw.Resize( arrayFaces.size() );

		// Парсим параметры сущностей и создаем их
		if ( GameFactory )
		{
			std::vector< Entity >		arrayEntities;
			EntitiesParse( arrayEntities, bspEntities, bspLumps[ BL_ENTITIES ].length );

			for ( UInt32_t index = 0, count = arrayEntities.size(); index < count; ++index )
			{
				Entity&			levelEntity = arrayEntities[ index ];
				IEntity*		entity = ( IEntity* ) GameFactory->Create( levelEntity.className.c_str() );
				if ( !entity )
				{
					g_consoleSystem->PrintError( "Entity with classname \"%s\" not found in game factory", levelEntity.className.c_str() );
					continue;
				}

				entity->SetModel( levelEntity.model, levelEntity.body );
				entity->SetLevel( this );

				for ( auto it = levelEntity.values.begin(), itEnd = levelEntity.values.end(); it != itEnd; ++it )
					entity->KeyValue( it->first.c_str(), it->second.c_str() );

				entity->IncrementReference();
				this->arrayEntities.push_back( entity );
			}
		}
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( "Level not loaded: %s", Exception.what() );

		Clear();
		return false;
	}

	isLoaded = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Обновить уровень
// ------------------------------------------------------------------------------------ //
void le::Level::Update()
{
	for ( UInt32_t indexCamera = 0, countCameras = arrayCameras.size(); indexCamera < countCameras; ++indexCamera )
	{
		Camera*		camera = arrayCameras[ indexCamera ];

		// Обновляем логику сущностей
		for ( UInt32_t index = 0, count = arrayEntities.size(); index < count; ++index )
		{
			IEntity*        entity = arrayEntities[ index ];

			// TODO: Сделать динамический (изменяемый из вне) дистанцию обновления
			if ( glm::distance( camera->GetPosition(), entity->GetCenter() ) < 2500.f )
				entity->Update();
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Отрендерить уровень
// ------------------------------------------------------------------------------------ //
void le::Level::Render()
{
	for ( UInt32_t indexCamera = 0, countCameras = arrayCameras.size(); indexCamera < countCameras; ++indexCamera )
	{
		Camera* camera = arrayCameras[ indexCamera ];

		g_studioRender->BeginScene( camera );

		// Определяем в каком кластере находится камера
		int			currentCluster = arrayBspLeafs[ FindLeaf( camera ) ].cluster;
		facesDraw.ClearAll();

		// Посылаем на отрисовку видимые части статичной геометрии уровня
		for ( UInt32_t indexLeaf = 0, countLeafs = arrayBspLeafs.size(); indexLeaf < countLeafs; ++indexLeaf )
		{
			BSPLeaf& bspLeaf = arrayBspLeafs[ indexLeaf ];
			if ( !IsClusterVisible( currentCluster, bspLeaf.cluster ) || !camera->IsVisible( bspLeaf.min, bspLeaf.max ) )
				continue;

			for ( UInt32_t indexFace = 0; indexFace < bspLeaf.numOfLeafFaces; ++indexFace )
			{
				int			faceIndex = arrayBspLeafsFaces[ bspLeaf.leafFace + indexFace ];
				if ( !facesDraw.On( faceIndex ) )
				{
					facesDraw.Set( faceIndex );
					g_studioRender->SubmitModel( ( IModel* ) arrayModels[ 0 ], faceIndex, 1 );
				}
			}
		}

		// Send on render entities
		for ( UInt32_t index = 0, count = arrayEntities.size(); index < count; ++index )
		{
			IEntity*		entity = arrayEntities[ index ];
			int			cluster = arrayBspLeafs[ FindLeaf( entity->GetCenter() ) ].cluster;

			if ( !IsClusterVisible( cluster, currentCluster ) || !entity->IsVisible( camera ) )
				continue;

			entity->Render( g_studioRender );
		}

		g_studioRender->EndScene();
	}
}

// ------------------------------------------------------------------------------------ //
// Очистить уровень
// ------------------------------------------------------------------------------------ //
void le::Level::Clear()
{
	for ( UInt32_t index = 0, count = arrayEntities.size(); index < count; ++index )
	{
		IEntity*			entity = arrayEntities[ index ];

		if ( entity->GetCountReferences() <= 1 )
			entity->Release();
		else
			entity->DecrementReference();
	}

	for ( UInt32_t index = 0, count = arrayLightmaps.size(); index < count; ++index )
	{
		ITexture*           lightmap = arrayLightmaps[ index ];

		if ( lightmap->GetCountReferences() <= 1 )
			arrayLightmaps[ index ]->Release();
		else
			lightmap->DecrementReference();
	}

	for ( UInt32_t index = 0, count = arrayModels.size(); index < count; ++index )
	{
		Model*         model = arrayModels[ index ];

		if ( model->GetCountReferences() <= 1 )
			model->Release();
		else
			model->DecrementReference();
	}

	for ( UInt32_t index = 0, count = arrayBodes.size(); index < count; ++index )
	{
		IBody*         body = arrayBodes[ index ];

		if ( body->GetCountReferences() <= 1 )
			body->Release();
		else
			body->DecrementReference();
	}

	for ( UInt32_t index = 0, count = arrayCameras.size(); index < count; ++index )
	{
		auto*           camera = arrayCameras[ index ];

		if ( camera->GetCountReferences() <= 1 )
			camera->Release();
		else
			camera->DecrementReference();
	}

	if ( mesh )
	{
		if ( mesh->GetCountReferences() <= 1 )
			mesh->Release();
		else
			mesh->DecrementReference();
	}

	arrayBspLeafs.clear();
	arrayBspLeafsFaces.clear();
	arrayBspNodes.clear();
	arrayBspPlanes.clear();
	arrayModels.clear();
	arrayLightmaps.clear();
	arrayCameras.clear();
	arrayEntities.clear();
	arrayBodes.clear();

	mesh = nullptr;
	isLoaded = false;
}

// ------------------------------------------------------------------------------------ //
// Добавить камеру на уровень
// ------------------------------------------------------------------------------------ //
void le::Level::AddCamera( ICamera* Camera )
{
	LIFEENGINE_ASSERT( Camera );

	Camera->IncrementReference();
	arrayCameras.push_back( ( le::Camera* ) Camera );
}

// ------------------------------------------------------------------------------------ //
// Добавить сущность на уровень
// ------------------------------------------------------------------------------------ //
void le::Level::AddEntity( IEntity* Entity )
{
	LIFEENGINE_ASSERT( Entity );

	Entity->IncrementReference();
	arrayEntities.push_back( Entity );
}

// ------------------------------------------------------------------------------------ //
// Удалить камеру с уровня
// ------------------------------------------------------------------------------------ //
void le::Level::RemoveCamera( ICamera* Camera )
{
	LIFEENGINE_ASSERT( Camera );

	for ( UInt32_t index = 0, count = arrayCameras.size(); index < count; ++index )
		if ( arrayCameras[ index ] == Camera )
		{
			if ( Camera->GetCountReferences() <= 1 )
				Camera->Release();
			else
				Camera->DecrementReference();

			arrayCameras.erase( arrayCameras.begin() + index );
			return;
		}
}

// ------------------------------------------------------------------------------------ //
// Удалить камеру с уровня
// ------------------------------------------------------------------------------------ //
void le::Level::RemoveCamera( UInt32_t Index )
{
	if ( Index >= arrayCameras.size() ) return;
	Camera*         camera = arrayCameras[ Index ];

	if ( camera->GetCountReferences() <= 1 )
		camera->Release();
	else
		camera->DecrementReference();

	arrayCameras.erase( arrayCameras.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Удалить сущность с уровня
// ------------------------------------------------------------------------------------ //
void le::Level::RemoveEntity( IEntity* Entity )
{
	for ( UInt32_t index = 0, count = arrayEntities.size(); index < count; ++index )
		if ( arrayEntities[ index ] == Entity )
		{
			if ( Entity->GetCountReferences() <= 1 )
				Entity->Release();
			else
				Entity->DecrementReference();

			arrayEntities.erase( arrayEntities.begin() + index );
			break;
		}
}

// ------------------------------------------------------------------------------------ //
// Удалить сущность с уровня
// ------------------------------------------------------------------------------------ //
void le::Level::RemoveEntity( UInt32_t Index )
{
	if ( Index >= arrayEntities.size() ) return;
	IEntity*			entity = arrayEntities[ Index ];

	if ( entity->GetCountReferences() <= 1 )
		entity->Release();
	else
		entity->DecrementReference();

	arrayEntities.erase( arrayEntities.begin() + Index );
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
	return "IBSP v46";
}

// ------------------------------------------------------------------------------------ //
// Получить количество камер на уровне
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Level::GetCountCameras() const
{
	return arrayCameras.size();
}

// ------------------------------------------------------------------------------------ //
// Получить камеру
// ------------------------------------------------------------------------------------ //
le::ICamera* le::Level::GetCamera( UInt32_t Index ) const
{
	if ( Index >= arrayCameras.size() )
		return nullptr;

	return arrayCameras[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Получить количество сущностей на уровне
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Level::GetCountEntityes() const
{
	return arrayEntities.size();
}

// ------------------------------------------------------------------------------------ //
// Получить сущность
// ------------------------------------------------------------------------------------ //
le::IEntity* le::Level::GetEntity( UInt32_t Index ) const
{
	if ( Index >= arrayEntities.size() )
		return nullptr;

	return arrayEntities[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Level::Level() :
	mesh( nullptr ),
	isLoaded( false ),
	countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Level::~Level()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Найти лист в котором находится точка
// ------------------------------------------------------------------------------------ //
int le::Level::FindLeaf( const Vector3D_t& Position ) const
{
	if ( !isLoaded )
		return -1;

	int			index = 0;
	float		distance = 0.f;

	while ( index >= 0 )
	{
		const BSPNode& node = arrayBspNodes[ index ];
		const BSPPlane& plane = arrayBspPlanes[ node.plane ];

		distance = glm::dot( plane.normal, Position ) - plane.distance;

		if ( distance >= 0 )
			index = node.front;
		else
			index = node.back;
	}

	return -index - 1;
}

// ------------------------------------------------------------------------------------ //
// Проверка на видимость кластера из другого кластера
// ------------------------------------------------------------------------------------ //
bool le::Level::IsClusterVisible( int CurrentCluster, int TestCluster ) const
{
	if ( !isLoaded || !visData.bitsets || CurrentCluster < 0 )
		return true;

	Byte_t		visSet = visData.bitsets[ CurrentCluster * visData.bytesPerCluster + ( TestCluster >> 3 ) ];

	if ( !( visSet & ( 1 << ( TestCluster & 7 ) ) ) )
		return false;

	return true;
}

// ------------------------------------------------------------------------------------ //
// Пропарсить параметры сущностей
// ------------------------------------------------------------------------------------ //
void le::Level::EntitiesParse( std::vector< Entity >& ArrayEntities, BSPEntities& BSPEntities, UInt32_t Size )
{
	bool												isEntity = false, isBracket = false, isName = false, isValue = false;
	UInt32_t											idStart_EntityData = 0, idFinish_EntityData = 0;
	std::string											name, temp;
	std::unordered_map< std::string, std::string >		values;

	for ( UInt32_t idChar_Entities = 0; idChar_Entities < Size; ++idChar_Entities )
	{
		if ( BSPEntities.entitiesData[ idChar_Entities ] == '{' && !isEntity )
		{
			isEntity = true;
			idStart_EntityData = idChar_Entities + 1;
		}
		else if ( BSPEntities.entitiesData[ idChar_Entities ] == '}' && isEntity )
		{
			isEntity = false;
			idFinish_EntityData = idChar_Entities - 1;

			for ( size_t idChar_Entity = idStart_EntityData; idChar_Entity < idFinish_EntityData; idChar_Entity++ )
			{
				if ( BSPEntities.entitiesData[ idChar_Entity ] == '\"' && !isName && !isValue && !isBracket )
				{
					isName = isBracket = true;
					temp.clear();
					continue;
				}
				else if ( BSPEntities.entitiesData[ idChar_Entity ] == '\"' && isName && !isValue && isBracket )
				{
					name = temp;
					isBracket = false;
					temp.clear();
					continue;
				}
				else if ( BSPEntities.entitiesData[ idChar_Entity ] == '\"' && isName && !isValue && !isBracket )
				{
					isBracket = isValue = true;
					temp.clear();
					continue;
				}
				else if ( BSPEntities.entitiesData[ idChar_Entity ] == '\"' && isName && isValue && isBracket )
				{
					isBracket = isValue = isName = false;

					values[ name ] = temp;
					temp.clear();
					continue;
				}

				temp += BSPEntities.entitiesData[ idChar_Entity ];
			}

			if ( values.find( "classname" ) != values.end() )
			{
				Entity			entity;
				entity.className = values[ "classname" ];
				values.erase( "classname" );

				// If in entity exist propery 'model' - take model
				auto		itValueModel = values.find( "model" );
				if ( itValueModel != values.end() )
				{
					std::string			path = itValueModel->second;
					if ( !path.empty() )
					{
						// If model this BSP brush
						if ( path[ 0 ] == '*' )
						{
							// Convert string to int
							path.erase( 0, 1 );
							int			idModel = std::atoi( path.c_str() );

							if ( idModel >= 0 && idModel < arrayModels.size() )
							{
								entity.model = arrayModels[ idModel ];
								entity.body = arrayBodes[ idModel ];
							}
						}
						// Else this is model on HDD
						else
						{
							IMesh*			mesh = g_resourceSystem->LoadMesh( path.c_str(), path.c_str() );
							if ( !mesh ) continue;

							entity.model = new Model();
							entity.model->SetMesh( mesh );
						}
					}

					values.erase( itValueModel );
				}

				entity.values = values;
				ArrayEntities.push_back( entity );
			}

			values.clear();
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Level::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Level::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::Level::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Level::GetCountReferences() const
{
	return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Level::Entity::Entity() :
	model( nullptr ),
	body( nullptr )
{}
