#include <string>
#include <mutex>
#include <dlfcn.h>
#include "plugin_loader/shared_library.hpp"

namespace plugin_loader {

SharedLibrary::SharedLibrary()
{
    _handle = 0;
}

void SharedLibrary::load(const std::string& path, int flags)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (_handle){
        throw plugin_loader::LibraryLoadException("Library already loaded: " + path);
    }
    int realFlags = RTLD_LAZY;
    if (flags & SHLIB_LOCAL)
        realFlags |= RTLD_LOCAL;
    else
        realFlags |= RTLD_GLOBAL;
    _handle = dlopen(path.c_str(), realFlags);
    if (!_handle)
    {
        const char* err = dlerror();
        throw plugin_loader::LibraryLoadException(
                    "Could not load library: " + (err ? std::string(err) : path) );
    }
    _path = path;
}


void SharedLibrary::unload()
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (_handle)
    {
        dlclose(_handle);
        _handle = 0;
    }
}


bool SharedLibrary::isLoaded() const
{
    return _handle != 0;
}


void* SharedLibrary::findSymbol(const std::string& name)
{
    std::unique_lock<std::mutex> lock(_mutex);

    void* result = 0;
    if (_handle)
    {
        result = dlsym(_handle, name.c_str());
    }
    return result;
}


const std::string& SharedLibrary::getPath() const
{
    return _path;
}

std::string SharedLibrary::prefix()
{
#if CL_OS == CL_OS_CYGWIN
        return "cyg";
#else
        return "lib";
#endif
}

std::string SharedLibrary::suffix()
{
#if CL_OS == CL_OS_MAC_OS_X
        #if defined(_DEBUG) && !defined(CL_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
                return "d.dylib";
        #else
                return ".dylib";
        #endif
#elif CL_OS == CL_OS_HPUX
        #if defined(_DEBUG) && !defined(CL_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
                return "d.sl";
        #else
                return ".sl";
        #endif
#elif CL_OS == CL_OS_CYGWIN
        #if defined(_DEBUG) && !defined(CL_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
                return "d.dll";
        #else
                return ".dll";
        #endif
#else
        #if defined(_DEBUG) && !defined(CL_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
                return "d.so";
        #else
                return ".so";
        #endif
#endif

}

} // namespace

