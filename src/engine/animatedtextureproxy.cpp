//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "global.h"
#include "engine/imaterialproxyvar.h"
#include "engine/animatedtextureproxy.h"
#include "engine/consolesystem.h"
#include "studiorender/ishaderparameter.h"

// ------------------------------------------------------------------------------------ //
// Set variable
// ------------------------------------------------------------------------------------ //
void le::AnimatedTextureProxy::SetVar( IMaterialProxyVar* MaterialProxyVar )
{
    if ( strcmp( MaterialProxyVar->GetName(), "frames" ) == 0 )
    {
        if ( MaterialProxyVar->GetType() != MPVT_ARRAY_VECTOR_4D )
        {
            g_consoleSystem->PrintError( "In variable [frames] mast be type array Vector4D" );
            return;
        }

        frames = MaterialProxyVar;
    }

    else if ( strcmp( MaterialProxyVar->GetName(), "textureRectVar" ) == 0 )
    {
        if ( MaterialProxyVar->GetType() != MPVT_SHADER_PARAMETER )
        {
            g_consoleSystem->PrintError( "In variable [textureRectVar] mast be type shader parameter" );
            return;
        }

        textureRectVar = MaterialProxyVar;
    }

    else if ( strcmp( MaterialProxyVar->GetName(), "frameRate" ) == 0 )
    {
        if ( MaterialProxyVar->GetType() != MPVT_FLOAT )
        {
            g_consoleSystem->PrintError( "In variable [frameRate] mast be type float" );
            return;
        }

        frameRate = MaterialProxyVar;
    }
    else
        g_consoleSystem->PrintError( "Variable [%s] not used in proxy-material le::AnimatedTextureProxy", MaterialProxyVar->GetName() );

    if ( frames && textureRectVar && frameRate )
        isInitialized = true;
}

// ------------------------------------------------------------------------------------ //
// Clear variable
// ------------------------------------------------------------------------------------ //
void le::AnimatedTextureProxy::ClearVar( const char* NameVar )
{
    if ( strcmp( NameVar, "frames" ) )                  frames = nullptr;
    else if ( strcmp( NameVar, "textureRectVar" ) )     textureRectVar = nullptr;
    else if ( strcmp( NameVar, "frameRate" ) )          frameRate = nullptr;
    else return;

    isInitialized = false;
}

// ------------------------------------------------------------------------------------ //
// Clear all variables
// ------------------------------------------------------------------------------------ //
void le::AnimatedTextureProxy::ClearAllVars()
{
    frames = textureRectVar = frameRate = nullptr;
    isInitialized = false;
}

// ------------------------------------------------------------------------------------ //
// Update animation
// ------------------------------------------------------------------------------------ //
void le::AnimatedTextureProxy::Update( UInt32_t DeltaTime )
{
    if ( !isInitialized ) return;

    le::UInt32_t        count;
    le::Vector4D_t*     arrayFrames = frames->GetValueArrayVector4D( count );

     currentFrameFire += 1.f / 80.f * DeltaTime;
     if ( currentFrameFire > count ) currentFrameFire = 0;


     textureRectVar->GetValueShaderParameter()->SetValueVector4D( arrayFrames[ ( int ) currentFrameFire ] );
}

// ------------------------------------------------------------------------------------ //
// Get name proxy-material
// ------------------------------------------------------------------------------------ //
const char* le::AnimatedTextureProxy::GetName() const
{
    return "AnimatedTexture";
}

// ------------------------------------------------------------------------------------ //
// Get variable
// ------------------------------------------------------------------------------------ //
le::IMaterialProxyVar* le::AnimatedTextureProxy::GetVar( const char* NameVar ) const
{
    if ( strcmp( NameVar, "frames" ) )                  return frames;
    else if ( strcmp( NameVar, "textureRectVar" ) )     return textureRectVar;
    else if ( strcmp( NameVar, "frameRate" ) )          return frameRate;

    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::AnimatedTextureProxy::AnimatedTextureProxy() :
    frames( nullptr ),
    textureRectVar( nullptr ),
    frameRate( nullptr ),
    isInitialized( false ),
    currentFrameFire( 0.f )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::AnimatedTextureProxy::~AnimatedTextureProxy()
{}






