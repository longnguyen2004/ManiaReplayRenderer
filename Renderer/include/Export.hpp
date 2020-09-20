
#ifndef RENDERERLIB_EXPORT_H
#define RENDERERLIB_EXPORT_H

#ifdef RENDERERLIB_STATIC_DEFINE
#  define RENDERERLIB_EXPORT
#  define RENDERERLIB_NO_EXPORT
#else
#  ifndef RENDERERLIB_EXPORT
#    ifdef RendererLib_EXPORTS
        /* We are building this library */
#      define RENDERERLIB_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define RENDERERLIB_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef RENDERERLIB_NO_EXPORT
#    define RENDERERLIB_NO_EXPORT 
#  endif
#endif

#ifndef RENDERERLIB_DEPRECATED
#  define RENDERERLIB_DEPRECATED __declspec(deprecated)
#endif

#ifndef RENDERERLIB_DEPRECATED_EXPORT
#  define RENDERERLIB_DEPRECATED_EXPORT RENDERERLIB_EXPORT RENDERERLIB_DEPRECATED
#endif

#ifndef RENDERERLIB_DEPRECATED_NO_EXPORT
#  define RENDERERLIB_DEPRECATED_NO_EXPORT RENDERERLIB_NO_EXPORT RENDERERLIB_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef RENDERERLIB_NO_DEPRECATED
#    define RENDERERLIB_NO_DEPRECATED
#  endif
#endif

#endif /* RENDERERLIB_EXPORT_H */
