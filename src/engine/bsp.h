//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef BSP_H
#define BSP_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum BSP_LUMPS
	{
		BL_ENTITIES = 0,				
		BL_TEXTURES,					
		BL_PLANES,						
		BL_NODES,						
		BL_LEAFS,						
		BL_LEAF_FACES,					
		BL_LEAF_BRUSHES,				
		BL_MODELS,						
		BL_BRUSHES,					
		BL_BRUSH_SIDES,					
		BL_VERTICES,					
		BL_INDICES,					
		BL_SHADERS,					
		BL_FACES,						
		BL_LIGHT_MAPS,					
		BL_LIGHT_VOLUMES,				
		BL_VIS_DATA,					
		BL_MAX_LUMPS					
	};

	//---------------------------------------------------------------------//

	enum BSP_TYPE_PLANE
	{
		BTP_POLYGON_FACE = 1,	
		BTP_PATCH,				
		BTP_MESH_FACE,			
		BTP_BILLBOARD			
	};

	//---------------------------------------------------------------------//

	struct BSPHeader
	{
		char		strID[ 4 ];		
		int			version;		
	};

	//---------------------------------------------------------------------//

	struct BSPLump
	{
		int			offset;			
		int			length;			
	};

	//---------------------------------------------------------------------//

	struct BSPVertex
	{
		Vector3D_t		position;			
		Vector2D_t		textureCoord;
		Vector2D_t		lightmapCoord;
		Vector3D_t		normal;
		Byte_t			color[ 4 ];			

		bool			operator==( BSPVertex& Vertex );
	};

	//---------------------------------------------------------------------//

	struct BSPModel
	{
		Vector3D_t		min;					
		Vector3D_t		max;					
		int				startFaceIndex;			
		int				numOfFaces;				
		int				startBrushIndex;		
		int				numOfBrushes;			
	};

	//---------------------------------------------------------------------//

	struct BSPFace
	{
		int						textureID;			
		int						effect;				
		BSP_TYPE_PLANE			type;				
		int						startVertIndex;		
		int						numOfVerts;			
		int						startIndex;			
		int						numOfIndices;		
		int						lightmapID;			
		int						lMapCorner[ 2 ];	
		int						lMapSize[ 2 ];		
		Vector3D_t				lMapPos;			
		Vector3D_t				lMapVecs[ 2 ];		
		Vector3D_t				normal;				
		int						size[ 2 ];			
	};

	//---------------------------------------------------------------------//

	struct BSPTexture
	{
		char		strName[ 64 ];				
		int			flags;						
		int			type;						
	};

	//---------------------------------------------------------------------//

	struct BSPLightmap
	{
		Byte_t		imageBits[ 128 ][ 128 ][ 3 ];
	};

	//---------------------------------------------------------------------//

	struct BSPNode
	{
		int					plane;					
		int					front;					
		int					back;					
		Vector3DInt_t		min;					
		Vector3DInt_t		max;					
	};

	//---------------------------------------------------------------------//

	struct BSPLeaf
	{
		int					cluster;			
		int					area;				
		Vector3DInt_t		min;				
		Vector3DInt_t		max;				
		int					leafFace;			
		int					numOfLeafFaces;		
		int					leafBrush;			
		int					numOfLeafBrushes;	
	};

	//---------------------------------------------------------------------//

	struct BSPPlane
	{
		Vector3D_t		normal;			
		float			distance;		
	};

	//---------------------------------------------------------------------//

	struct BSPVisData
	{
		BSPVisData();
		~BSPVisData();

		int			numOfClusters;			
		int			bytesPerCluster;		
		Byte_t*		bitsets;						
	};

	//---------------------------------------------------------------------//

	struct BSPEntities
	{
		BSPEntities();
		~BSPEntities();

		char*			entitiesData;
	};

	//---------------------------------------------------------------------//

	struct BSPBrush
	{
		int			brushSide;			
		int			numOfBrushSides;	
		int			textureID;			
	};

	//---------------------------------------------------------------------//

	struct BSPBrushSide
	{
		int			plane;				
		int			textureID;			
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !BSP_H

