#pragma once

#include <string>

#if defined(__has_include)
#if __has_include(<lw/core/PathsConfig.h>)
#include <lw/core/PathsConfig.h>
#endif
#endif

#ifndef LW_PROJECT_ROOT
#define LW_PROJECT_ROOT "."
#endif

#ifndef LW_ASSETS_DIR
#define LW_ASSETS_DIR "assets"
#endif

namespace lw
{
    inline bool isAbsolutePath(const std::string& path)
    {
        return !path.empty() &&
            (path[0] == '/' || path[0] == '\\' ||
             (path.size() > 1 && path[1] == ':'));
    }

    inline std::string joinPath(const std::string& base, const std::string& relative)
    {
        if (relative.empty())
            return base;

        if (isAbsolutePath(relative))
            return relative;

        if (base.empty() || base == ".")
            return relative;

        const char last = base[base.size() - 1];
        if (last == '/' || last == '\\')
            return base + relative;

        return base + "/" + relative;
    }

    inline std::string projectPath(const std::string& relative)
    {
        return joinPath(LW_PROJECT_ROOT, relative);
    }

    inline std::string assetPath(const std::string& relative)
    {
        return joinPath(LW_ASSETS_DIR, relative);
    }
}
