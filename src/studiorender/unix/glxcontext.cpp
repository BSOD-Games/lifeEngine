/////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://gitlab.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <mutex>
#include <vector>
#include <GL/glxew.h>

#include "engine/iconsolesystem.h"

#include "global.h"
#include "glxcontext.h"
#include "settingscontext.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    struct ContextDescriptor
    {
        Window              window;
        Display*            display;
        GLXContext          glxContext;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

static le::ContextDescriptor*				currentContext = nullptr;

// ------------------------------------------------------------------------------------ //
// Создать контекст
// ------------------------------------------------------------------------------------ //
bool le::GLX_CreateContext( WindowHandle_t WindowHandle, const SettingsContext& SettingsContext, ContextDescriptor_t& ContextDescriptor, ContextDescriptor_t* ShareContext )
{
    Window			window = *static_cast<Window*>( WindowHandle );
    Display*		display = XOpenDisplay( nullptr );
    GLXContext		glxContext = 0;

    le::ContextDescriptor*	shareContext = ShareContext ? static_cast< le::ContextDescriptor* >( *ShareContext ) : nullptr;
    bool					isSetProfile = false;

    try
    {
        // Заполняем атрибуты для выбора формата пиксела

        vector<int>			visualAttributes = { GLX_RGBA, GLX_DOUBLEBUFFER };
        visualAttributes.push_back( GLX_RED_SIZE );
        visualAttributes.push_back( SettingsContext.redBits );
        visualAttributes.push_back( GLX_GREEN_SIZE );
        visualAttributes.push_back( SettingsContext.greenBits );
        visualAttributes.push_back( GLX_BLUE_SIZE );
        visualAttributes.push_back( SettingsContext.blueBits );
        visualAttributes.push_back( GLX_DEPTH_SIZE );
        visualAttributes.push_back( SettingsContext.depthBits );
        visualAttributes.push_back( GLX_STENCIL_SIZE );
        visualAttributes.push_back( SettingsContext.stencilBits );
        visualAttributes.push_back( 0 );

        // Создаем временый контекст OpenGL'a

        XVisualInfo*		visualInfo = glXChooseVisual( display, DefaultScreen( display ), &visualAttributes[ 0 ] );
        if ( !visualInfo )		throw "Failed choose visual settings";

        if ( shareContext )
        {
            static mutex		mutex;
            mutex.lock();

            if ( shareContext == currentContext )
            {
                if ( !glXMakeCurrent( shareContext->display, 0, nullptr ) )
                    throw "Failed to deactivate shared context before sharing";

                currentContext = nullptr;
            }

            mutex.unlock();
        }

        glxContext = glXCreateContext( display, visualInfo, shareContext ? shareContext->glxContext : nullptr, GL_TRUE );
        if ( !glxContext )		throw "Creating temporary render context fail";

        if ( !glXMakeCurrent( display, window, glxContext ) )
            throw "Selecting temporary render context fail";

        XFree( visualInfo );

        // Загружаем расширения OpenGL'a

        glewExperimental = GL_TRUE;
        if ( glewInit() != GLEW_OK )	throw "OpenGL context is broken";

        // Выводим в лог информацию об OpenGL'e

        int			numberExtensions = 0;
        glGetIntegerv( GL_NUM_EXTENSIONS, &numberExtensions );

        g_consoleSystem->PrintInfo( "*** OpenGL info ***" );
        g_consoleSystem->PrintInfo( "  OpenGL version: %s", glGetString( GL_VERSION ) );
        g_consoleSystem->PrintInfo( "  OpenGL vendor: %s", glGetString( GL_VENDOR ) );
        g_consoleSystem->PrintInfo( "  OpenGL renderer: %s", glGetString( GL_RENDERER ) );
        g_consoleSystem->PrintInfo( "  OpenGL GLSL version: %s", glGetString( GL_SHADING_LANGUAGE_VERSION ) );
        g_consoleSystem->PrintInfo( "  OpenGL extensions:" );

        for ( uint32_t index = 0; index < numberExtensions; ++index )
        {
            const uint8_t*		extension = glGetStringi( GL_EXTENSIONS, index );
            g_consoleSystem->PrintInfo( ( char* ) extension );
        }

        g_consoleSystem->PrintInfo( "*** OpenGL info end ***" );

        // Проверяем на наличие расширения glXCreateContextAttribsARB, а
        // так же на присутсвие GLX 1.3 или выше,
        // если имеется то создаем расширеный контекст OpenGL'a

        int			glxMajor = 0;
        int			glxMinor = 0;
        if ( !glXQueryVersion( display, &glxMajor, &glxMinor ) )
            throw "Failed to query GLX version, limited to legacy context creation";

        g_consoleSystem->PrintInfo( "*** GLX info ***" );
        g_consoleSystem->PrintInfo( "  GLX version: %i.%i", glxMajor, glxMinor );
        g_consoleSystem->PrintInfo( "  GLX extensions: %s", glXQueryExtensionsString( display, DefaultScreen( display ) ) );
        g_consoleSystem->PrintInfo( "*** GLX info end ***" );

        if ( ( glxMajor > 1 || glxMinor >= 3 ) && glxewIsSupported( "GLX_ARB_create_context" ) == 1 )
        {
            // Заполняем атрибуты контекста OpenGL'a

            vector<int>			contextAttributes;

            // Если версия OpenGL'a была запрошена больше 1.1, то добавляем в атрибуты

            if ( SettingsContext.majorVersion > 1 || ( SettingsContext.majorVersion == 1 && SettingsContext.minorVersion > 1 ) )
            {
                contextAttributes.push_back( GLX_CONTEXT_MAJOR_VERSION_ARB );
                contextAttributes.push_back( SettingsContext.majorVersion );
                contextAttributes.push_back( GLX_CONTEXT_MINOR_VERSION_ARB );
                contextAttributes.push_back( SettingsContext.minorVersion );
            }

            // Если есть поддержка настройки контекста профиля, то
            // добовляем соответствующие атрибуты

            if ( glxewIsSupported( "GLX_ARB_create_context_profile" ) == 1 )
            {
                int				profile = ( SettingsContext.attributeFlags & le::SettingsContext::CA_CORE ) ? GLX_CONTEXT_CORE_PROFILE_BIT_ARB : GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
                int				debug = ( SettingsContext.attributeFlags & le::SettingsContext::CA_DEBUG ) ? GLX_CONTEXT_DEBUG_BIT_ARB : 0;

                contextAttributes.push_back( GLX_CONTEXT_PROFILE_MASK_ARB );
                contextAttributes.push_back( profile );
                contextAttributes.push_back( GLX_CONTEXT_FLAGS_ARB );
                contextAttributes.push_back( debug );

                isSetProfile = true;
            }
            else if ( ( SettingsContext.attributeFlags & le::SettingsContext::CA_CORE ) || ( SettingsContext.attributeFlags & le::SettingsContext::CA_DEBUG ) )
                    g_consoleSystem->PrintWarning( "Selecting a profile during context creation is not supported, disabling comptibility and debug" );

            // Добавляем признак конца массива

            contextAttributes.push_back( 0 );
            contextAttributes.push_back( 0 );

            // Заполняем атрибуты для выбора формата пиксела
            visualAttributes.clear();
            visualAttributes =
            {
                GLX_X_RENDERABLE, 1,
                GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                GLX_RENDER_TYPE, GLX_RGBA_BIT,
                GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
                GLX_DOUBLEBUFFER, 1
            };
            visualAttributes.push_back( GLX_RED_SIZE );
            visualAttributes.push_back( SettingsContext.redBits );
            visualAttributes.push_back( GLX_GREEN_SIZE );
            visualAttributes.push_back( SettingsContext.greenBits );
            visualAttributes.push_back( GLX_BLUE_SIZE );
            visualAttributes.push_back( SettingsContext.blueBits );
            visualAttributes.push_back( GLX_DEPTH_SIZE );
            visualAttributes.push_back( SettingsContext.depthBits );
            visualAttributes.push_back( GLX_STENCIL_SIZE );
            visualAttributes.push_back( SettingsContext.stencilBits );
            visualAttributes.push_back( 0 );

            // Получаем все конфигурации буфера кадра и ищим самый лучший

            int				countFramebuffers = 0;
            GLXFBConfig*	glxFBConfigs = glXChooseFBConfig( display, DefaultScreen( display ), &visualAttributes[ 0 ], &countFramebuffers );
            if ( !glxFBConfigs )		throw "Failed to retrieve a framebuffer config";

            int				bestIndexFBConfig = -1;
            int				bestNumberSamples = -1;

            for ( uint32_t index = 0; index < countFramebuffers; ++index )
            {
                XVisualInfo*			visualInfo = glXGetVisualFromFBConfig( display, glxFBConfigs[ index ] );

                if ( visualInfo )
                {
                    int			sampleBuffer;
                    int			samples;
                    glXGetFBConfigAttrib( display, glxFBConfigs[ index ], GLX_SAMPLE_BUFFERS, &sampleBuffer );
                    glXGetFBConfigAttrib( display, glxFBConfigs[ index ], GLX_SAMPLES, &samples  );

                    if ( bestIndexFBConfig < 0 || sampleBuffer && samples > bestNumberSamples )
                    {
                        bestIndexFBConfig = index;
                        bestNumberSamples = samples;
                    }
                }

                XFree( visualInfo );
            }

            // Берем лучшую конфигурацию буфера кадра и создаем контекст OpenGL'a

            GLXFBConfig			glxFBConfig = glxFBConfigs[ bestIndexFBConfig ];
            XFree( glxFBConfigs );

            GLXContext		oldGLXContext = glxContext;
            glxContext = glXCreateContextAttribsARB( display, glxFBConfig, shareContext ? shareContext->glxContext : nullptr, GL_TRUE, &contextAttributes[ 0 ] );

            if ( glxContext )
            {
                glXDestroyContext( display, oldGLXContext );

                if ( !glXMakeCurrent( display, window, glxContext ) )
                    throw "Selecting render context fail";
            }
            else
            {
                glxContext = oldGLXContext;
                g_consoleSystem->PrintError( "Failed created context OpenGL with extension GLX_ARB_create_context. Used legacy context" );
            }
        }
    }
    catch ( const string& Message )
    {
        g_consoleSystem->PrintError( Message.c_str() );
        return false;
    }

    le::ContextDescriptor*			contextDescriptor = new le::ContextDescriptor();
    contextDescriptor->display 			= display;
    contextDescriptor->window 			= window;
    contextDescriptor->glxContext 		= glxContext;
    ContextDescriptor 					= contextDescriptor;
    GLX_MakeCurrentContext( contextDescriptor );

    g_consoleSystem->PrintInfo( "Context OpenGL %s%s%s created", glGetString( GL_VERSION ),
        ( isSetProfile && ( SettingsContext.attributeFlags& le::SettingsContext::CA_CORE ) ? " core" : "" ),
        ( isSetProfile && ( SettingsContext.attributeFlags& le::SettingsContext::CA_DEBUG ) ? " debug" : "" ) );

    return true;
}


// ------------------------------------------------------------------------------------ //
// Сделать текущим контекст
// ------------------------------------------------------------------------------------ //
bool le::GLX_MakeCurrentContext( const ContextDescriptor_t& ContextDescriptor )
{
    if ( static_cast< le::ContextDescriptor* >( ContextDescriptor ) == currentContext ) return true;

    if ( !glXMakeCurrent( static_cast< le::ContextDescriptor* >( ContextDescriptor )->display, static_cast< le::ContextDescriptor* >( ContextDescriptor )->window, static_cast< le::ContextDescriptor* >( ContextDescriptor )->glxContext ) )
        return false;

    currentContext = static_cast< le::ContextDescriptor* >( ContextDescriptor );
    return true;
}

// ------------------------------------------------------------------------------------ //
// Удалить контекст
// ------------------------------------------------------------------------------------ //
void le::GLX_DeleteContext( ContextDescriptor_t& ContextDescriptor )
{
    if ( static_cast< le::ContextDescriptor* >( ContextDescriptor ) == currentContext )
    {
        glXMakeCurrent( static_cast< le::ContextDescriptor* >( ContextDescriptor )->display, 0, nullptr );
        currentContext = nullptr;
    }

    glXDestroyContext( static_cast< le::ContextDescriptor* >( ContextDescriptor )->display, static_cast< le::ContextDescriptor* >( ContextDescriptor )->glxContext );
    XCloseDisplay( static_cast< le::ContextDescriptor* >( ContextDescriptor )->display );
    delete static_cast< le::ContextDescriptor* >( ContextDescriptor );
    ContextDescriptor = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Сменить буферы
// ------------------------------------------------------------------------------------ //
void le::GLX_SwapBuffers( const ContextDescriptor_t& ContextDescriptor )
{
    if ( static_cast< le::ContextDescriptor* >( ContextDescriptor ) != currentContext ) return;
    glXSwapBuffers( static_cast< le::ContextDescriptor* >( ContextDescriptor )->display, static_cast< le::ContextDescriptor* >( ContextDescriptor )->window );
}

// ------------------------------------------------------------------------------------ //
// Включить вертикальную синхронизацию
// ------------------------------------------------------------------------------------ //
void le::GLX_SetVerticalSync( bool IsEnable )
{
    if ( !currentContext ) return;
    int				result = 0;

    if ( glxewIsSupported( "GLX_EXT_swap_control" ) == 1 )
    {
        int			interval = 0;

        if ( IsEnable )
            interval = glxewIsSupported( "GLX_EXT_swap_control_tear" ) == 1 ? -1 : 1;

        glXSwapIntervalEXT( currentContext->display, currentContext->window, interval );
    }
    else if ( glxewIsSupported( "GLX_MESA_swap_control" ) == 1 )
        result = glXSwapIntervalMESA( IsEnable ? 1 : 0 );
    else if ( glxewIsSupported( "GLX_SGI_swap_control" ) == 1 )
        result = glXSwapIntervalSGI( IsEnable ? 1 : 0 );
    else
    {
        static bool			isWarned = false;

        if ( isWarned )
        {
            g_consoleSystem->PrintWarning( "Setting vertical sync not supported" );
            isWarned = true;
        }
    }

    if ( result != 0 )
        g_consoleSystem->PrintError( "Setting vertical sync failed" );
}

//---------------------------------------------------------------------//
