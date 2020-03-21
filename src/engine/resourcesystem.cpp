//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <exception>
#include <fstream>
#include <FreeImage.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "common/image.h"
#include "common/meshsurface.h"
#include "common/meshdescriptor.h"
#include "engine/lifeengine.h"
#include "engine/engine.h"
#include "engine/material.h"
#include "engine/imaterialproxy.h"
#include "engine/materialproxyvar.h"
#include "studiorender/istudiorender.h"
#include "studiorender/itexture.h"
#include "studiorender/imesh.h"
#include "studiorender/studiovertexelement.h"
#include "studiorender/studiorendersampler.h"
#include "studiorender/istudiorendertechnique.h"
#include "studiorender/istudiorenderpass.h"
#include "studiorender/ishaderparameter.h"

#include "global.h"
#include "consolesystem.h"
#include "resourcesystem.h"
#include "level.h"
#include "fontfreetype.h"

#define LMD_ID			"LMD"
#define LMD_VERSION		2
#define LMT_VERSION		2

struct Vertex
{
    le::Vector3D_t			position;
    le::Vector3D_t			normal;
    le::Vector2D_t			texCoords;
    le::Vector3D_t			tangent;
    le::Vector3D_t			bitangent;
};

struct MaterialProxy
{
    std::string                                                 name;
    std::unordered_map< std::string, rapidjson::Value* >        values;
};

struct MaterialPass
{
    MaterialPass() :
        isDepthTest( true ),
        isDepthWrite( true ),
        isBlend( false ),
        isCullFace( true ),
        cullFaceType( le::CT_BACK )
    {}

    bool														isDepthTest;
    bool														isDepthWrite;
    bool														isBlend;
    bool														isCullFace;
    le::CULLFACE_TYPE											cullFaceType;
    std::string													shader;
    std::unordered_map< std::string, rapidjson::Value* >		parameters;
    std::vector< MaterialProxy >                                proxes;
};

struct MaterialTechnique
{
    std::string						type;
    std::vector< MaterialPass >		passes;
};

// ------------------------------------------------------------------------------------ //
// Преобразовать JSON массив в 2D вектор 
// ------------------------------------------------------------------------------------ //
inline le::Vector2D_t JsonArrayToVec2( rapidjson::Value::Array& Array )
{
    try
    {
        if ( Array.Size() < 2 ) throw;

        for ( size_t index = 0, count = Array.Size(); index < count; ++index )
            if ( !Array[ index ].IsNumber() )
                throw;
    }
    catch ( ... )
    {
        return le::Vector2D_t( 0.f, 0.f );
    }

    return le::Vector2D_t( Array[ 0 ].GetFloat(), Array[ 1 ].GetFloat() );
}

// ------------------------------------------------------------------------------------ //
// Преобразовать JSON массив в 3D вектор 
// ------------------------------------------------------------------------------------ //
inline le::Vector3D_t JsonArrayToVec3( rapidjson::Value::Array& Array )
{
    try
    {
        if ( Array.Size() < 3 ) throw;

        for ( size_t index = 0, count = Array.Size(); index < count; ++index )
            if ( !Array[ index ].IsNumber() )
                throw;
    }
    catch ( ... )
    {
        return le::Vector3D_t( 0.f, 0.f, 0.f );
    }

    return le::Vector3D_t( Array[ 0 ].GetFloat(), Array[ 1 ].GetFloat(), Array[ 2 ].GetFloat() );
}

// ------------------------------------------------------------------------------------ //
// Преобразовать JSON массив в 4D вектор 
// ------------------------------------------------------------------------------------ //
inline le::Vector4D_t JsonArrayToVec4( rapidjson::Value::Array& Array )
{
    try
    {
        if ( Array.Size() < 4 ) throw;

        for ( size_t index = 0, count = Array.Size(); index < count; ++index )
            if ( !Array[ index ].IsNumber() )
                throw;
    }
    catch ( ... )
    {
        return le::Vector4D_t( 0.f, 0.f, 0.f, 0.f );
    }

    return le::Vector4D_t( Array[ 0 ].GetFloat(), Array[ 1 ].GetFloat(), Array[ 2 ].GetFloat(), Array[ 3 ].GetFloat() );
}

// ------------------------------------------------------------------------------------ //
// Преобразовать JSON массив в матрицу 4х4
// ------------------------------------------------------------------------------------ //
inline le::Matrix4x4_t JsonArrayToMatrix( rapidjson::Value::Array& Array )
{
    try
    {
        if ( Array.Size() < 16 ) throw;

        for ( size_t index = 0, count = Array.Size(); index < count; ++index )
            if ( !Array[ index ].IsNumber() )
                throw;
    }
    catch ( ... )
    {
        return le::Matrix4x4_t( 1.f );
    }

    return le::Matrix4x4_t( Array[ 0 ].GetFloat(), Array[ 1 ].GetFloat(), Array[ 2 ].GetFloat(), Array[ 3 ].GetFloat(),
            Array[ 4 ].GetFloat(), Array[ 5 ].GetFloat(), Array[ 6 ].GetFloat(), Array[ 7 ].GetFloat(),
            Array[ 8 ].GetFloat(), Array[ 9 ].GetFloat(), Array[ 10 ].GetFloat(), Array[ 11 ].GetFloat(),
            Array[ 12 ].GetFloat(), Array[ 13 ].GetFloat(), Array[ 14 ].GetFloat(), Array[ 15 ].GetFloat() );
}

// ------------------------------------------------------------------------------------ //
// Преобразовать строку в перечисление техник рендера
// ------------------------------------------------------------------------------------ //
inline le::RENDER_TECHNIQUE RenderTechnique_StringToEnum( const char* Type )
{
    if ( !Type || Type == "" ) return le::RENDER_TECHNIQUE();

    if ( strcmp( Type, "deffered_shading" ) == 0 )		return le::RT_DEFFERED_SHADING;
    else												return le::RENDER_TECHNIQUE();
}

// ------------------------------------------------------------------------------------ //
// Преобразовать строку в перечисление типа отсекаемых полигонов
// ------------------------------------------------------------------------------------ //
inline le::CULLFACE_TYPE CullFaceType_StringToEnum( const char* Type )
{
    if ( !Type || Type == "" ) return le::CT_BACK;

    if ( strcmp( Type, "front" ) == 0 )		return le::CT_FRONT;
    else									return le::CT_BACK;
}

// ------------------------------------------------------------------------------------ //
// Загрузить изображение
// ------------------------------------------------------------------------------------ //
void LE_LoadImage( const char* Path, le::Image& Image, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
    IsError = false;
    FREE_IMAGE_FORMAT		imageFormat = FIF_UNKNOWN;
    imageFormat = FreeImage_GetFileType( Path, 0 );

    if ( imageFormat == FIF_UNKNOWN )
        imageFormat = FreeImage_GetFIFFromFilename( Path );

    FIBITMAP* bitmap = FreeImage_Load( imageFormat, Path, 0 );
    if ( !bitmap )
    {
        IsError = true;
        return;
    }

    if ( IsFlipVertical )				FreeImage_FlipVertical( bitmap );
    if ( IsSwitchRedAndBlueChannels )
    {
        auto		red = FreeImage_GetChannel( bitmap, FREE_IMAGE_COLOR_CHANNEL::FICC_RED );
        auto		blue = FreeImage_GetChannel( bitmap, FREE_IMAGE_COLOR_CHANNEL::FICC_BLUE );

        FreeImage_SetChannel( bitmap, blue, FREE_IMAGE_COLOR_CHANNEL::FICC_RED );
        FreeImage_SetChannel( bitmap, red, FREE_IMAGE_COLOR_CHANNEL::FICC_BLUE );

        FreeImage_Unload( red );
        FreeImage_Unload( blue );
    }

    le::UInt8_t* tempData = FreeImage_GetBits( bitmap );
    Image.width = FreeImage_GetWidth( bitmap );
    Image.height = FreeImage_GetHeight( bitmap );
    Image.depth = FreeImage_GetBPP( bitmap );
    Image.pitch = FreeImage_GetPitch( bitmap );

    Image.data = ( le::UInt8_t* ) malloc( Image.pitch * Image.height );
    memcpy( Image.data, tempData, Image.pitch * Image.height );

    Image.rMask = 0x00ff0000;
    Image.gMask = 0x0000ff00;
    Image.bMask = 0x000000ff;
    Image.aMask = ( Image.depth == 24 ) ? 0 : 0xff000000;

    FreeImage_Unload( bitmap );
    return;
}

// ------------------------------------------------------------------------------------ //
// Загрузить текстуру
// ------------------------------------------------------------------------------------ //
le::ITexture* LE_LoadTexture( const char* Path, le::IFactory* StudioRenderFactory )
{
    bool				isError = false;
    le::Image			image;

    LE_LoadImage( Path, image, isError, false, true );
    if ( isError )			return nullptr;

    le::ITexture* texture = ( le::ITexture* ) StudioRenderFactory->Create( TEXTURE_INTERFACE_VERSION );
    if ( !texture )			return nullptr;

    texture->Initialize( le::TT_2D, image.aMask > 0 ? le::IF_RGBA_8UNORM : le::IF_RGB_8UNORM, image.width, image.height );
    texture->Bind();
    texture->Append( image.data );
    texture->GenerateMipmaps();

    le::StudioRenderSampler			sampler;
    sampler.minFilter = le::SF_LINEAR_MIPMAP_LINEAR;
    sampler.magFilter = le::SF_LINEAR;
    texture->SetSampler( sampler );

    texture->Unbind();

    delete[] image.data;
    return texture;
}

// ------------------------------------------------------------------------------------ //
// Загрузить материал
// ------------------------------------------------------------------------------------ //
le::IMaterial* LE_LoadMaterial( const char* Path, le::IResourceSystem* ResourceSystem, le::IMaterialManager* MaterialManager, le::IFactory* StudioRenderFactory )
{
    std::ifstream		file( Path );
    if ( !file.is_open() )						return nullptr;

    std::string					stringBuffer;
    std::getline( file, stringBuffer, '\0' );

    rapidjson::Document			document;
    document.Parse( stringBuffer.c_str() );
    if ( document.HasParseError() )				return nullptr;

    if ( document.FindMember( "version" ) == document.MemberEnd() ||
         !document[ "version" ].IsNumber() || document[ "version" ].GetInt() != LMT_VERSION )
        return nullptr;

    std::string													surface;
    std::unordered_map< std::string, MaterialTechnique >		techniques;

    // Считываем все параметры материала в память
    for ( auto itRoot = document.MemberBegin(), itRootEnd = document.MemberEnd(); itRoot != itRootEnd; ++itRoot )
    {
        // Название поверхности
        if ( strcmp( itRoot->name.GetString(), "surface" ) == 0 && itRoot->value.IsString() )
            surface = itRoot->value.GetString();

        // Считываем техники визуализации
        else if ( strcmp( itRoot->name.GetString(), "technique" ) == 0 && itRoot->value.IsObject() )
        {
            MaterialTechnique			technique;
            for ( auto itTechnique = itRoot->value.GetObject().MemberBegin(), itTechniqueEnd = itRoot->value.GetObject().MemberEnd(); itTechnique != itTechniqueEnd; ++itTechnique )
            {
                // Тип техники
                if ( strcmp( itTechnique->name.GetString(), "type" ) == 0 && itTechnique->value.IsString() )
                    technique.type = itTechnique->value.GetString();

                // Проходы
                else if ( strcmp( itTechnique->name.GetString(), "pass" ) == 0 && itTechnique->value.IsObject() )
                {
                    MaterialPass			pass;
                    for ( auto itPass = itTechnique->value.GetObject().MemberBegin(), itPassEnd = itTechnique->value.GetObject().MemberEnd(); itPass != itPassEnd; ++itPass )
                    {
                        // Включен ли тест глубины
                        if ( strcmp( itPass->name.GetString(), "depthTest" ) == 0 && itPass->value.IsBool() )
                            pass.isDepthTest = itPass->value.GetBool();

                        // Включена ли запись в буфер глубины
                        else if ( strcmp( itPass->name.GetString(), "depthWrite" ) == 0 && itPass->value.IsBool() )
                            pass.isDepthWrite = itPass->value.GetBool();

                        // Включено ли смешивание
                        else if ( strcmp( itPass->name.GetString(), "blend" ) == 0 && itPass->value.IsBool() )
                            pass.isBlend = itPass->value.GetBool();

                        // Включено ли отсечение полигонов
                        else if ( strcmp( itPass->name.GetString(), "cullface" ) == 0 && itPass->value.IsBool() )
                            pass.isCullFace = itPass->value.GetBool();

                        // Тип отсикаемых полигонов
                        else if ( strcmp( itPass->name.GetString(), "cullface_type" ) == 0 )
                            if ( itPass->value.IsString() )
                                pass.cullFaceType = CullFaceType_StringToEnum( itPass->value.GetString() );
                            else if ( itPass->value.IsNumber() )
                                pass.cullFaceType = ( le::CULLFACE_TYPE ) itPass->value.GetInt();

                        // Шейдер
                        if ( strcmp( itPass->name.GetString(), "shader" ) == 0 && itPass->value.IsString() )
                            pass.shader = itPass->value.GetString();

                        // Параметры шейдера
                        else if ( strcmp( itPass->name.GetString(), "parameters" ) == 0 && itPass->value.IsObject() )
                            for ( auto itParameter = itPass->value.GetObject().MemberBegin(), itParameterEnd = itPass->value.GetObject().MemberEnd(); itParameter != itParameterEnd; ++itParameter )
                                pass.parameters[ itParameter->name.GetString() ] = &itParameter->value;

                        // Прокси-материалы для прохода
                        else if ( strcmp( itPass->name.GetString(), "proxies" ) == 0 && itPass->value.IsObject() )
                            for ( auto itProxy = itPass->value.GetObject().MemberBegin(), itProxyEnd = itPass->value.GetObject().MemberEnd(); itProxy != itProxyEnd; ++itProxy )
                            {
                                MaterialProxy           proxy;
                                if ( !itProxy->name.IsString() || !itProxy->value.IsObject() ) continue;

                                proxy.name = itProxy->name.GetString();
                                for ( auto itProxyValue = itProxy->value.GetObject().MemberBegin(), itProxyValueEnd = itProxy->value.GetObject().MemberEnd(); itProxyValue != itProxyValueEnd; ++itProxyValue )
                                    proxy.values[ itProxyValue->name.GetString() ] = &itProxyValue->value;

                                pass.proxes.push_back( proxy );
                            }

                    }

                    technique.passes.push_back( pass );
                }
            }

            if ( !technique.type.empty() && techniques.find( technique.type ) == techniques.end() )
                techniques[ technique.type ] = technique;
        }
    }

    if ( techniques.empty() )			return nullptr;

    le::Material* material = new le::Material();
    if ( !surface.empty() )		material->SetSurfaceName( surface.c_str() );

    for ( auto itTechnique = techniques.begin(), itTechniqueEnd = techniques.end(); itTechnique != itTechniqueEnd; ++itTechnique )
    {
        le::IStudioRenderTechnique* technique = ( le::IStudioRenderTechnique* ) StudioRenderFactory->Create( TECHNIQUE_INTERFACE_VERSION );
        if ( !technique )		return nullptr;

        technique->SetType( RenderTechnique_StringToEnum( itTechnique->first.c_str() ) );
        for ( le::UInt32_t indexPass = 0, countPasses = itTechnique->second.passes.size(); indexPass < countPasses; ++indexPass )
        {
            auto& materialPass = itTechnique->second.passes[ indexPass ];
            le::IStudioRenderPass* pass = ( le::IStudioRenderPass* ) StudioRenderFactory->Create( PASS_INTERFACE_VERSION );
            if ( !pass )		return nullptr;

            pass->SetShader( materialPass.shader.c_str() );
            pass->EnableDepthTest( materialPass.isDepthTest );
            pass->EnableDepthWrite( materialPass.isDepthWrite );
            pass->EnableBlend( materialPass.isBlend );
            pass->EnableCullFace( materialPass.isCullFace );
            pass->SetCullFaceType( materialPass.cullFaceType );

            for ( auto itParameters = materialPass.parameters.begin(), itParametersEnd = materialPass.parameters.end(); itParameters != itParametersEnd; ++itParameters )
            {
                auto& value = itParameters->second;
                le::IShaderParameter* parameter = ( le::IShaderParameter* ) StudioRenderFactory->Create( SHADERPARAMETER_INTERFACE_VERSION );
                if ( !parameter )		return nullptr;

                parameter->SetName( itParameters->first.c_str() );

                if ( itParameters->second->IsString() )
                {
                    le::ITexture* texture = ResourceSystem->LoadTexture( itParameters->second->GetString(), itParameters->second->GetString() );
                    if ( !texture )	continue;

                    parameter->SetValueTexture( texture );
                }

                else if ( itParameters->second->IsObject() )
                {
                    rapidjson::Value::Object            object = itParameters->second->GetObject();
                    bool                                hasX = object.HasMember( "x" );
                    bool                                hasY = object.HasMember( "y" );
                    bool                                hasZ = object.HasMember( "z" );
                    bool                                hasW = object.HasMember( "w" );

                    if ( hasX && hasY && !hasZ && !hasW )       parameter->SetValueVector2D( le::Vector2D_t( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat() ) );
                    else if ( hasX && hasY && hasZ && !hasW )   parameter->SetValueVector3D( le::Vector3D_t( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat() ) );
                    else if ( hasX && hasY && hasZ && hasW )    parameter->SetValueVector4D( le::Vector4D_t( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat(), object.FindMember( "w" )->value.GetFloat() ) );
                }

                else if ( itParameters->second->IsBool() )		parameter->SetValueShaderFlag( itParameters->second->GetBool() );
                else if ( itParameters->second->IsDouble() )	parameter->SetValueFloat( itParameters->second->GetDouble() );
                else if ( itParameters->second->IsFloat() )		parameter->SetValueFloat( itParameters->second->GetFloat() );
                else if ( itParameters->second->IsInt() )		parameter->SetValueInt( itParameters->second->GetInt() );

                pass->AddParameter( parameter );
            }

            for ( auto itProxies = materialPass.proxes.begin(), itProxiesEnd = materialPass.proxes.end(); itProxies != itProxiesEnd; ++itProxies )
            {
                bool                        isProxyValid = true;
                le::IMaterialProxy*         proxy = MaterialManager->CreateProxy( itProxies->name.c_str() );
                if ( !proxy ) continue;

                for ( auto itProxiesVar = itProxies->values.begin(), itProxiesVarEnd = itProxies->values.end(); itProxiesVar != itProxiesVarEnd; ++itProxiesVar )
                {
                    le::MaterialProxyVar*           var = new le::MaterialProxyVar();
                    var->SetName( itProxiesVar->first.c_str() );

                    if ( itProxiesVar->second->IsArray() )
                    {
                        rapidjson::Value::Array         array = itProxiesVar->second->GetArray();

                        if ( array.Begin()->IsInt() )
                        {
                            std::vector< int >          stdArray;
                            for ( le::UInt32_t index = 0, count = array.Size(); index < count; ++index )
                                stdArray.push_back( array[ index ].GetInt() );

                            var->SetValueArrayInt( stdArray.data(), stdArray.size() );
                        }
                        else if ( array.Begin()->IsFloat() )
                        {
                            std::vector< float >          stdArray;
                            for ( le::UInt32_t index = 0, count = array.Size(); index < count; ++index )
                                stdArray.push_back( array[ index ].GetInt() );

                            var->SetValueArrayFloat( stdArray.data(), stdArray.size() );
                        }
                        else if ( array.Begin()->IsObject() )
                        {
                            rapidjson::Value::Object            object = array.Begin()->GetObject();
                            bool                                hasX = object.HasMember( "x" );
                            bool                                hasY = object.HasMember( "y" );
                            bool                                hasZ = object.HasMember( "z" );
                            bool                                hasW = object.HasMember( "w" );

                            if ( hasX && hasY && !hasZ && !hasW )
                            {
                                std::vector< le::Vector2D_t >          stdArray;
                                for ( le::UInt32_t index = 0, count = array.Size(); index < count; ++index )
                                {
                                    object = array[ index ].GetObject();
                                    stdArray.push_back( le::Vector2D_t( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat() ) );
                                }

                                var->SetValueArrayVector2D( stdArray.data(), stdArray.size() );
                            }
                            else if ( hasX && hasY && hasZ && !hasW )
                            {
                                std::vector< le::Vector3D_t >          stdArray;
                                for ( le::UInt32_t index = 0, count = array.Size(); index < count; ++index )
                                {
                                    object = array[ index ].GetObject();
                                    stdArray.push_back( le::Vector3D_t( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat() ) );
                                }

                                var->SetValueArrayVector3D( stdArray.data(), stdArray.size() );
                            }
                            else if ( hasX && hasY && hasZ && hasW )
                            {
                                std::vector< le::Vector4D_t >          stdArray;
                                for ( le::UInt32_t index = 0, count = array.Size(); index < count; ++index )
                                {
                                    object = array[ index ].GetObject();
                                    stdArray.push_back( le::Vector4D_t( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat(), object.FindMember( "w" )->value.GetFloat() ) );
                                }

                                var->SetValueArrayVector4D( stdArray.data(), stdArray.size() );
                            }
                        }
                    }
                    else if ( itProxiesVar->second->IsString() )
                    {
                        le::IShaderParameter*           parameter = pass->FindParameter( itProxiesVar->second->GetString() );
                        if ( !parameter )
                        {
                            delete var;
                            continue;
                        }

                        var->SetValueShaderParameter( parameter );
                    }
                    else if ( itProxiesVar->second->IsObject() )
                    {
                        rapidjson::Value::Object            object = itProxiesVar->second->GetObject();
                        bool                                hasX = object.HasMember( "x" );
                        bool                                hasY = object.HasMember( "y" );
                        bool                                hasZ = object.HasMember( "z" );
                        bool                                hasW = object.HasMember( "w" );

                        if ( hasX && hasY && !hasZ && !hasW )       var->SetValueVector2D( le::Vector2D_t( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat() ) );
                        else if ( hasX && hasY && hasZ && !hasW )   var->SetValueVector3D( le::Vector3D_t( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat() ) );
                        else if ( hasX && hasY && hasZ && hasW )    var->SetValueVector4D( le::Vector4D_t( object.FindMember( "x" )->value.GetFloat(), object.FindMember( "y" )->value.GetFloat(), object.FindMember( "z" )->value.GetFloat(), object.FindMember( "w" )->value.GetFloat() ) );
                    }
                    else if ( itProxiesVar->second->IsBool() )          var->SetValueBool( itProxiesVar->second->GetBool() );
                    else if ( itProxiesVar->second->IsDouble() )        var->SetValueFloat( itProxiesVar->second->GetFloat() );
                    else if ( itProxiesVar->second->IsFloat() )         var->SetValueFloat( itProxiesVar->second->GetFloat() );
                    else if ( itProxiesVar->second->IsInt() )           var->SetValueInt( itProxiesVar->second->GetInt() );

                    proxy->SetVar( var );
                }

                if ( !isProxyValid )
                {
                    delete proxy;
                    continue;
                }

                pass->AddProxy( proxy );
            }

            technique->AddPass( pass );
        }

        material->AddTechnique( technique );
    }

    return ( le::IMaterial* ) material;
}

// ------------------------------------------------------------------------------------ //
// Загрузить меш
// ------------------------------------------------------------------------------------ //
le::IMesh* LE_LoadMesh( const char* Path, le::IResourceSystem* ResourceSystem, le::IFactory* StudioRenderFactory )
{
    std::ifstream				file( Path, std::ios::binary );
    if ( !file.is_open() )		return nullptr;

    // Читаем заголовок файла
    char						strId[ 3 ];
    le::UInt16_t				version = 0;

    file.read( strId, 3 );
    file.read( ( char* ) &version, sizeof( le::UInt16_t ) );
    if ( strncmp( strId, LMD_ID, 3 ) != 0 || version != LMD_VERSION )		return nullptr;

    // Читаем все материалы модели
    le::UInt32_t						sizeString = 0;
    le::UInt32_t						sizeArrayMaterials = 0;
    std::string* routeMaterial = nullptr;
    std::vector< std::string >			arrayRouteMaterials;
    std::vector< le::IMaterial* >		arrayMaterials;

    file.read( ( char* ) &sizeArrayMaterials, sizeof( le::UInt32_t ) );
    arrayRouteMaterials.resize( sizeArrayMaterials );

    for ( le::UInt32_t index = 0; index < sizeArrayMaterials; ++index )
    {
        routeMaterial = &arrayRouteMaterials[ index ];

        file.read( ( char* ) &sizeString, sizeof( le::UInt32_t ) );
        routeMaterial->resize( sizeString );
        file.read( &( *routeMaterial )[ 0 ], sizeString );
    }

    // Загружаем материалы
    for ( le::UInt32_t index = 0; index < sizeArrayMaterials; ++index )
    {
        le::IMaterial* material = ResourceSystem->LoadMaterial( arrayRouteMaterials[ index ].c_str(), arrayRouteMaterials[ index ].c_str() );
        if ( !material ) continue;

        arrayMaterials.push_back( material );
    }

    // Читаем все вершины модели
    le::UInt32_t						sizeArrayVerteces;
    std::vector< Vertex >				arrayVerteces;

    file.read( ( char* ) &sizeArrayVerteces, sizeof( le::UInt32_t ) );
    arrayVerteces.resize( sizeArrayVerteces );

    if ( sizeArrayVerteces > 0 )
        file.read( ( char* ) &arrayVerteces[ 0 ], sizeArrayVerteces * sizeof( Vertex ) );
    else
        return nullptr;

    // Читаем все индексы модели
    le::UInt32_t						sizeArrayIndices;
    std::vector< le::UInt32_t >			arrayIndices;

    file.read( ( char* ) &sizeArrayIndices, sizeof( le::UInt32_t ) );
    arrayIndices.resize( sizeArrayIndices );

    if ( sizeArrayIndices > 0 )
        file.read( ( char* ) &arrayIndices[ 0 ], sizeArrayIndices * sizeof( le::UInt32_t ) );
    else
        return nullptr;

    // Читаем все сетки модели
    le::UInt32_t						sizeArraySurfaces;
    std::vector< le::MeshSurface >		arraySurfaces;

    file.read( ( char* ) &sizeArraySurfaces, sizeof( le::UInt32_t ) );
    if ( sizeArraySurfaces == 0 )		return nullptr;

    le::MeshSurface						surface;
    surface.startVertexIndex = 0;
    surface.lightmapID = 0;

    for ( le::UInt32_t index = 0; index < sizeArraySurfaces; ++index )
    {
        file.read( ( char* ) &surface.materialID, sizeof( le::UInt32_t ) );
        file.read( ( char* ) &surface.startIndex, sizeof( le::UInt32_t ) );
        file.read( ( char* ) &surface.countIndeces, sizeof( le::UInt32_t ) );
        arraySurfaces.push_back( surface );
    }

    if ( arraySurfaces.empty() )				return nullptr;

    // Находим минимальную и максимальную точку в меше
    le::Vector3D_t			min = arrayVerteces[ 0 ].position;
    le::Vector3D_t			max = arrayVerteces[ 0 ].position;

    for ( uint32_t index = 0; index < sizeArrayVerteces; ++index )
    {
        min.x = glm::min( min.x, arrayVerteces[ index ].position.x );
        min.y = glm::min( min.y, arrayVerteces[ index ].position.y );
        min.z = glm::min( min.z, arrayVerteces[ index ].position.z );

        max.x = glm::max( max.x, arrayVerteces[ index ].position.x );
        max.y = glm::max( max.y, arrayVerteces[ index ].position.y );
        max.z = glm::max( max.z, arrayVerteces[ index ].position.z );
    }

    // Создаем сам меш
    le::IMesh* mesh = ( le::IMesh* ) StudioRenderFactory->Create( MESH_INTERFACE_VERSION );
    if ( !mesh )				return nullptr;

    // Создаем описание для формата вершин
    std::vector< le::StudioVertexElement >			vertexElements =
    {
        { 3, le::VET_FLOAT },
        { 3, le::VET_FLOAT },
        { 2, le::VET_FLOAT },
        { 3, le::VET_FLOAT },
        { 3, le::VET_FLOAT }
    };

    // Создаем описание меша для загрузки его в модуль рендера
    le::MeshDescriptor				meshDescriptor;
    meshDescriptor.countIndeces = arrayIndices.size();
    meshDescriptor.countMaterials = arrayMaterials.size();
    meshDescriptor.countLightmaps = 0;
    meshDescriptor.countSurfaces = arraySurfaces.size();
    meshDescriptor.sizeVerteces = arrayVerteces.size() * sizeof( Vertex );

    meshDescriptor.indeces = arrayIndices.data();
    meshDescriptor.materials = arrayMaterials.data();
    meshDescriptor.lightmaps = nullptr;
    meshDescriptor.surfaces = arraySurfaces.data();
    meshDescriptor.verteces = arrayVerteces.data();

    meshDescriptor.min = min;
    meshDescriptor.max = max;
    meshDescriptor.primitiveType = le::PT_TRIANGLES;
    meshDescriptor.countVertexElements = vertexElements.size();
    meshDescriptor.vertexElements = vertexElements.data();

    // Загружаем меш в GPU
    mesh->Create( meshDescriptor );
    if ( !mesh->IsCreated() )
    {
        StudioRenderFactory->Delete( mesh );
        return nullptr;
    }

    return mesh;
}

// ------------------------------------------------------------------------------------ //
// Load font
// ------------------------------------------------------------------------------------ //
le::IFont* LE_LoadFont( const char* Path )
{
    le::FontFreeType*			fontFreeType = new le::FontFreeType();
    if ( !fontFreeType->Load( Path ) )
    {
        delete fontFreeType;
        return nullptr;
    }

    return fontFreeType;
}

// ------------------------------------------------------------------------------------ //
// Загрузить уровень
// ------------------------------------------------------------------------------------ //
le::ILevel* LE_LoadLevel( const char* Path, le::IFactory* GameFactory )
{
    le::Level* level = new le::Level();
    if ( !level->Load( Path, GameFactory ) )
    {
        delete level;
        return nullptr;
    }

    return level;
};

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик картинок
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Image( const char* Format, LoadImageFn_t LoadImage )
{
    LIFEENGINE_ASSERT( Format );
    LIFEENGINE_ASSERT( LoadImage );

    g_consoleSystem->PrintInfo( "Loader image for format [%s] registered", Format );
    loaderImages[ Format ] = LoadImage;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик текстур
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Texture( const char* Format, LoadTextureFn_t LoadTexture )
{
    LIFEENGINE_ASSERT( Format );
    LIFEENGINE_ASSERT( LoadTexture );

    g_consoleSystem->PrintInfo( "Loader texture for format [%s] registered", Format );
    loaderTextures[ Format ] = LoadTexture;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик материалов
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Material( const char* Format, LoadMaterialFn_t LoadMaterial )
{
    LIFEENGINE_ASSERT( Format );
    LIFEENGINE_ASSERT( LoadMaterial );

    g_consoleSystem->PrintInfo( "Loader material for format [%s] registered", Format );
    loaderMaterials[ Format ] = LoadMaterial;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик мешей
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Mesh( const char* Format, LoadMeshFn_t LoadMesh )
{
    LIFEENGINE_ASSERT( Format );
    LIFEENGINE_ASSERT( LoadMesh );

    g_consoleSystem->PrintInfo( "Loader mesh for format [%s] registered", Format );
    loaderMeshes[ Format ] = LoadMesh;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать загрузчик уровней
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Level( const char* Format, LoadLevelFn_t LoadLevel )
{
    LIFEENGINE_ASSERT( Format );
    LIFEENGINE_ASSERT( LoadLevel );

    g_consoleSystem->PrintInfo( "Loader level for format [%s] registered", Format );
    loaderLevels[ Format ] = LoadLevel;
}

// ------------------------------------------------------------------------------------ //
// Register loader fonts
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::RegisterLoader_Font( const char* Format, LoadFontFn_t LoadFont )
{
    LIFEENGINE_ASSERT( Format );
    LIFEENGINE_ASSERT( LoadFont );

    g_consoleSystem->PrintInfo( "Loader font for format [%s] registered", Format );
    loaderFonts[ Format ] = LoadFont;}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик картинок
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Image( const char* Format )
{
    LIFEENGINE_ASSERT( Format );

    auto		it = loaderImages.find( Format );
    if ( it == loaderImages.end() ) return;

    loaderImages.erase( it );
    g_consoleSystem->PrintInfo( "Loader image for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик текстур
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Texture( const char* Format )
{
    LIFEENGINE_ASSERT( Format );

    auto		it = loaderTextures.find( Format );
    if ( it == loaderTextures.end() ) return;

    loaderTextures.erase( it );
    g_consoleSystem->PrintInfo( "Loader texture for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик материалов
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Material( const char* Format )
{
    LIFEENGINE_ASSERT( Format );

    auto		it = loaderMaterials.find( Format );
    if ( it == loaderMaterials.end() ) return;

    loaderMaterials.erase( it );
    g_consoleSystem->PrintInfo( "Loader material for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик мешей
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Mesh( const char* Format )
{
    LIFEENGINE_ASSERT( Format );

    auto		it = loaderMeshes.find( Format );
    if ( it == loaderMeshes.end() ) return;

    loaderMeshes.erase( it );
    g_consoleSystem->PrintInfo( "Loader mesh for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Разрегестрировать загрузчик уровней
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Level( const char* Format )
{
    LIFEENGINE_ASSERT( Format );

    auto		it = loaderLevels.find( Format );
    if ( it == loaderLevels.end() ) return;

    loaderLevels.erase( it );
    g_consoleSystem->PrintInfo( "Loader level for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Unregister loader font
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnregisterLoader_Font( const char* Format )
{
    LIFEENGINE_ASSERT( Format );

    auto		it = loaderFonts.find( Format );
    if ( it == loaderFonts.end() ) return;

    loaderFonts.erase( it );
    g_consoleSystem->PrintInfo( "Loader font for format [%s] unregistered", Format );
}

// ------------------------------------------------------------------------------------ //
// Загрузить картинку
// ------------------------------------------------------------------------------------ //
le::Image le::ResourceSystem::LoadImage( const char* Path, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
    LIFEENGINE_ASSERT( Path );
    IsError = false;

    try
    {
        if ( loaderImages.empty() )					throw std::runtime_error( "No image loaders" );
        std::string			path = gameDir + "/" + Path;

        std::string			format = GetFormatFile( path );
        if ( format.empty() )						throw std::runtime_error( "In image format not found" );

        auto				parser = loaderImages.find( format );
        if ( parser == loaderImages.end() )			throw std::runtime_error( "Loader for format image not found" );

        Image				image;
        parser->second( path.c_str(), image, IsError, IsFlipVertical, IsSwitchRedAndBlueChannels );
        if ( IsError )								throw std::runtime_error( "Fail loading image" );

        return image;
    }
    catch ( std::exception & Exception )
    {
        g_consoleSystem->PrintError( "Image [%s] not loaded: %s", Path, Exception.what() );
        return Image();
    }
}

// ------------------------------------------------------------------------------------ //
// Загрузить текстуру
// ------------------------------------------------------------------------------------ //
le::ITexture* le::ResourceSystem::LoadTexture( const char* Name, const char* Path )
{
    LIFEENGINE_ASSERT( Name );
    LIFEENGINE_ASSERT( Path );

    try
    {
        if ( !studioRenderFactory )							throw std::runtime_error( "Resource system not initialized" );

        if ( textures.find( Name ) != textures.end() )		return textures[ Name ];
        if ( loaderTextures.empty() )						throw std::runtime_error( "No texture loaders" );

        std::string			path = gameDir + "/" + Path;

        g_consoleSystem->PrintInfo( "Loading texture [%s] with name [%s]", Path, Name );

        std::string			format = GetFormatFile( path );
        if ( format.empty() )						throw std::runtime_error( "In texture format not found" );

        auto				parser = loaderTextures.find( format );
        if ( parser == loaderTextures.end() )		throw std::runtime_error( "Loader for format texture not found" );

        ITexture* texture = parser->second( path.c_str(), studioRenderFactory );
        if ( !texture )								throw std::runtime_error( "Fail loading texture" );

        textures.insert( std::make_pair( Name, texture ) );
        g_consoleSystem->PrintInfo( "Loaded texture [%s]", Name );

        return texture;
    }
    catch ( std::exception & Exception )
    {
        g_consoleSystem->PrintError( "Texture [%s] not loaded: %s", Path, Exception.what() );
        return nullptr;
    }
}

// ------------------------------------------------------------------------------------ //
// Загрузить материал
// ------------------------------------------------------------------------------------ //
le::IMaterial* le::ResourceSystem::LoadMaterial( const char* Name, const char* Path )
{
    LIFEENGINE_ASSERT( Name );
    LIFEENGINE_ASSERT( Path );

    try
    {
        if ( !studioRenderFactory )							throw std::runtime_error( "Resource system not initialized" );

        if ( materials.find( Name ) != materials.end() )	return materials[ Name ];
        if ( loaderMaterials.empty() )						throw std::runtime_error( "No material loaders" );

        std::string			path = gameDir + "/" + Path;

        g_consoleSystem->PrintInfo( "Loading material [%s] with name [%s]", Path, Name );

        std::string			format = GetFormatFile( path );
        if ( format.empty() )						throw std::runtime_error( "In material format not found" );

        auto				parser = loaderMaterials.find( format );
        if ( parser == loaderMaterials.end() )		throw std::runtime_error( "Loader for format material not found" );

        IMaterial* material = parser->second( path.c_str(), this, materialManager, studioRenderFactory );
        if ( !material )	throw std::runtime_error( "Fail loading material" );

        materials.insert( std::make_pair( Name, material ) );
        g_consoleSystem->PrintInfo( "Loaded material [%s]", Name );

        return material;
    }
    catch ( std::exception & Exception )
    {
        g_consoleSystem->PrintError( "Material [%s] not loaded: %s", Path, Exception.what() );
        return nullptr;
    }
}

// ------------------------------------------------------------------------------------ //
// Загрузить меш
// ------------------------------------------------------------------------------------ //
le::IMesh* le::ResourceSystem::LoadMesh( const char* Name, const char* Path )
{
    LIFEENGINE_ASSERT( Name );
    LIFEENGINE_ASSERT( Path );

    try
    {
        if ( !studioRenderFactory )							throw std::runtime_error( "Resource system not initialized" );

        if ( meshes.find( Name ) != meshes.end() )			return meshes[ Name ];
        if ( loaderMeshes.empty() )							throw std::runtime_error( "No mesh loaders" );

        std::string			path = gameDir + "/" + Path;

        g_consoleSystem->PrintInfo( "Loading mesh [%s] with name [%s]", Path, Name );

        std::string			format = GetFormatFile( path );
        if ( format.empty() )						throw std::runtime_error( "In mesh format not found" );

        auto				parser = loaderMeshes.find( format );
        if ( parser == loaderMeshes.end() )		throw std::runtime_error( "Loader for format mesh not found" );

        IMesh* mesh = parser->second( path.c_str(), this, studioRenderFactory );
        if ( !mesh )							throw std::runtime_error( "Fail loading mesh" );

        meshes.insert( std::make_pair( Name, mesh ) );
        g_consoleSystem->PrintInfo( "Loaded mesh [%s]", Name );

        return mesh;
    }
    catch ( std::exception & Exception )
    {
        g_consoleSystem->PrintError( "Mesh [%s] not loaded: %s", Path, Exception.what() );
        return nullptr;
    }
}

// ------------------------------------------------------------------------------------ //
// Загрузить уровень
// ------------------------------------------------------------------------------------ //
le::ILevel* le::ResourceSystem::LoadLevel( const char* Name, const char* Path, IFactory* GameFactory )
{
    LIFEENGINE_ASSERT( Name );
    LIFEENGINE_ASSERT( Path );

    try
    {
        if ( !studioRenderFactory )							throw std::runtime_error( "Resource system not initialized" );

        if ( levels.find( Name ) != levels.end() )			return levels[ Name ];
        if ( loaderLevels.empty() )							throw std::runtime_error( "No level loaders" );

        std::string			path = gameDir + "/" + Path;

        g_consoleSystem->PrintInfo( "Loading level [%s] with name [%s]", Path, Name );

        std::string			format = GetFormatFile( path );
        if ( format.empty() )						throw std::runtime_error( "In level format not found" );

        auto				parser = loaderLevels.find( format );
        if ( parser == loaderLevels.end() )		throw std::runtime_error( "Loader for format level not found" );

        ILevel* level = parser->second( path.c_str(), GameFactory );
        if ( !level )							throw std::runtime_error( "Fail loading level" );

        levels.insert( std::make_pair( Name, level ) );
        g_consoleSystem->PrintInfo( "Loaded level [%s]", Name );

        return level;
    }
    catch ( std::exception & Exception )
    {
        g_consoleSystem->PrintError( "Level [%s] not loaded: %s", Path, Exception.what() );
        return nullptr;
    }
}

// ------------------------------------------------------------------------------------ //
// Load font
// ------------------------------------------------------------------------------------ //
le:: IFont* le::ResourceSystem::LoadFont( const char* Name, const char* Path )
{
    LIFEENGINE_ASSERT( Name );
    LIFEENGINE_ASSERT( Path );

    try
    {
        if ( fonts.find( Name ) != fonts.end() )			return fonts[ Name ];
        if ( loaderFonts.empty() )							throw std::runtime_error( "No font loaders" );

        std::string			path = gameDir + "/" + Path;

        g_consoleSystem->PrintInfo( "Loading font [%s] with name [%s]", Path, Name );

        std::string			format = GetFormatFile( path );
        if ( format.empty() )						throw std::runtime_error( "In font format not found" );

        auto				parser = loaderFonts.find( format );
        if ( parser == loaderFonts.end() )		throw std::runtime_error( "Loader for format font not found" );

        IFont* 		font = parser->second( path.c_str() );
        if ( !font )							throw std::runtime_error( "Fail loading font" );

        fonts.insert( std::make_pair( Name, font ) );
        g_consoleSystem->PrintInfo( "Loaded font [%s]", Name );

        return font;
    }
    catch ( std::exception & Exception )
    {
        g_consoleSystem->PrintError( "Font [%s] not loaded: %s", Path, Exception.what() );
        return nullptr;
    }
}

// ------------------------------------------------------------------------------------ //
// Выгрузить картинку
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadImage( Image& Image )
{
    if ( !Image.data ) return;

    delete[] Image.data;
    Image.data = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить текстуру
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadTexture( const char* Name )
{
    LIFEENGINE_ASSERT( Name );

    if ( !studioRenderFactory )
    {
        g_consoleSystem->PrintError( "Resource system not initialized" );
        return;
    }

    auto				it = textures.find( Name );
    if ( it == textures.end() )	return;

    studioRenderFactory->Delete( it->second );
    textures.erase( it );

    g_consoleSystem->PrintInfo( "Unloaded texture [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить материал
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMaterial( const char* Name )
{
    LIFEENGINE_ASSERT( Name );

    auto				it = materials.find( Name );
    if ( it == materials.end() )	return;

    delete it->second;
    materials.erase( it );

    g_consoleSystem->PrintInfo( "Unloaded material [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить меш
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMesh( const char* Name )
{
    LIFEENGINE_ASSERT( Name );

    if ( !studioRenderFactory )
    {
        g_consoleSystem->PrintError( "Resource system not initialized" );
        return;
    }

    auto				it = meshes.find( Name );
    if ( it == meshes.end() )	return;

    studioRenderFactory->Delete( it->second );
    meshes.erase( it );

    g_consoleSystem->PrintInfo( "Unloaded mesh [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить уровень
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadLevel( const char* Name )
{
    LIFEENGINE_ASSERT( Name );

    auto				it = levels.find( Name );
    if ( it == levels.end() )	return;

    // TODO: Сделать правильное удаление, ибо объект может быть создан в другом модуле
    delete it->second;
    levels.erase( it );

    g_consoleSystem->PrintInfo( "Unloaded level [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Unload font
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadFont( const char* Name )
{
    LIFEENGINE_ASSERT( Name );

    auto				it = fonts.find( Name );
    if ( it == fonts.end() )	return;

    // TODO: Сделать правильное удаление, ибо объект может быть создан в другом модуле
    delete it->second;
    fonts.erase( it );

    g_consoleSystem->PrintInfo( "Unloaded font [%s]", Name );
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все материалы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMaterials()
{
    if ( materials.empty() ) return;

    for ( auto it = materials.begin(), itEnd = materials.end(); it != itEnd; ++it )
        delete it->second;

    g_consoleSystem->PrintInfo( "Unloaded all materials" );
    materials.clear();
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все мешы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadMeshes()
{
    if ( meshes.empty() ) return;

    if ( !studioRenderFactory )
    {
        g_consoleSystem->PrintError( "Resource system not initialized" );
        return;
    }

    for ( auto it = meshes.begin(), itEnd = meshes.end(); it != itEnd; ++it )
        studioRenderFactory->Delete( it->second );

    g_consoleSystem->PrintInfo( "Unloaded all meshes" );
    meshes.clear();
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все уровени
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadLevels()
{
    if ( levels.empty() ) return;

    for ( auto it = levels.begin(), itEnd = levels.end(); it != itEnd; ++it )
        delete it->second;

    g_consoleSystem->PrintInfo( "Unloaded all levels" );
    levels.clear();
}

// ------------------------------------------------------------------------------------ //
// Unload all fonts
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadFonts()
{
    if ( fonts.empty() ) return;

    for ( auto it = fonts.begin(), itEnd = fonts.end(); it != itEnd; ++it )
        delete it->second;

    g_consoleSystem->PrintInfo( "Unloaded all fonts" );
    fonts.clear();
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все текстуры
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadTextures()
{
    if ( textures.empty() ) return;

    if ( !studioRenderFactory )
    {
        g_consoleSystem->PrintError( "Resource system not initialized" );
        return;
    }

    for ( auto it = textures.begin(), itEnd = textures.end(); it != itEnd; ++it )
        studioRenderFactory->Delete( it->second );

    g_consoleSystem->PrintInfo( "Unloaded all textures" );
    textures.clear();
}

// ------------------------------------------------------------------------------------ //
// Выгрузить все ресурсы
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::UnloadAll()
{
    UnloadLevels();
    UnloadMeshes();
    UnloadMaterials();
    UnloadTextures();
    UnloadFonts();
}

// ------------------------------------------------------------------------------------ //
//  Получить текстуру
// ------------------------------------------------------------------------------------ //
le::ITexture* le::ResourceSystem::GetTexture( const char* Name ) const
{
    LIFEENGINE_ASSERT( Name );

    auto	it = textures.find( Name );
    if ( it != textures.end() )		return it->second;

    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Получить материал
// ------------------------------------------------------------------------------------ //
le::IMaterial* le::ResourceSystem::GetMaterial( const char* Name ) const
{
    LIFEENGINE_ASSERT( Name );

    auto	it = materials.find( Name );
    if ( it != materials.end() )		return it->second;

    return nullptr;
}
// ------------------------------------------------------------------------------------ //
// Получить меш
// ------------------------------------------------------------------------------------ //
le::IMesh* le::ResourceSystem::GetMesh( const char* Name ) const
{
    LIFEENGINE_ASSERT( Name );

    auto	it = meshes.find( Name );
    if ( it != meshes.end() )		return it->second;

    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Получить уровень
// ------------------------------------------------------------------------------------ //
le::ILevel* le::ResourceSystem::GetLevel( const char* Name ) const
{
    LIFEENGINE_ASSERT( Name );

    auto	it = levels.find( Name );
    if ( it != levels.end() )		return it->second;

    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Get font
// ------------------------------------------------------------------------------------ //
le::IFont* le::ResourceSystem::GetFont( const char* Name ) const
{
    LIFEENGINE_ASSERT( Name );

    auto	it = fonts.find( Name );
    if ( it != fonts.end() )		return it->second;

    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Инициализировать систему ресурсов
// ------------------------------------------------------------------------------------ //
bool le::ResourceSystem::Initialize( IEngine* Engine )
{
    try
    {
        IStudioRender* studioRender = Engine->GetStudioRender();
        if ( !studioRender )	throw std::runtime_error( "Resource system requared studiorender" );

        studioRenderFactory = studioRender->GetFactory();
        materialManager = Engine->GetMaterialManager();

        RegisterLoader_Image( "png", LE_LoadImage );
        RegisterLoader_Image( "jpg", LE_LoadImage );
        RegisterLoader_Image( "tga", LE_LoadImage );
        RegisterLoader_Texture( "png", LE_LoadTexture );
        RegisterLoader_Texture( "jpg", LE_LoadTexture );
        RegisterLoader_Texture( "tga", LE_LoadTexture );
        RegisterLoader_Material( "lmt", LE_LoadMaterial );
        RegisterLoader_Mesh( "lmd", LE_LoadMesh );
        RegisterLoader_Level( "bsp", LE_LoadLevel );
        RegisterLoader_Font( "ttf", LE_LoadFont );
    }
    catch ( std::exception & Exception )
    {
        g_consoleSystem->PrintError( Exception.what() );
        return false;
    }
    return true;
}

// ------------------------------------------------------------------------------------ //
// Задать каталог игры
// ------------------------------------------------------------------------------------ //
void le::ResourceSystem::SetGameDir( const char* GameDir )
{
    gameDir = GameDir;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ResourceSystem::ResourceSystem() :
    studioRenderFactory( nullptr ),
    materialManager( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ResourceSystem::~ResourceSystem()
{
    UnloadAll();
}
