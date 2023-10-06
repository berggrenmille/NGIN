//
// Created by Maximiliam Berggren on 05/10/2023.
//

//Precompiled include
#include <Precompiled/PCH.h>
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/VFS/Path.hpp>
#include <utility>
// STL Include(s)
// Other Include(s)
#ifdef NGIN_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace NGIN::VFS
{
    Path::Path()
            : pathStr(emptyPath)
    {
    }

    Path::Path(const String& path)
            : pathStr(ConvertToNative(path))
    {
        Normalize();
    }

#ifdef NGIN_PLATFORM_WINDOWS

    Path::Path(StringType path)
            : pathStr(std::move(path))
    {
        Normalize();
    }

#endif


    String Path::ToString() const
    {
        String temp = ConvertFromNative(pathStr);

#if defined(NGIN_PLATFORM_WINDOWS)

        std::replace(temp.begin(), temp.end(), '\\', '/');
#endif
        return temp;
    }

    Bool Path::IsAbsolute() const
    {
        // can be absolute in two ways:
        // "C:/some/path"
        // "/some/path"

        // If the path is empty, it is not absolute
        if (pathStr.empty())
            return false;

        // If the path is root, it is absolute
        if (IsRoot())
            return true;

        // check begining
#ifdef NGIN_PLATFORM_WINDOWS
        if (pathStr.size() > 2 &&
            std::isalpha(pathStr[0]) &&
            pathStr[1] == ':' &&
            pathStr[2] == separator)
        {
            return true;
        }
#else
        if (pathStr[0] == separator)
            return true;
#endif
        return false;
    }

/// Determines if the path is a relative path
    Bool Path::IsRelative() const
    {
        // can be relative in two ways:
        // "some/path"
        // "./some/path"

        // If the path is not empty and not absolute, it is not relative
        return !pathStr.empty() && !IsAbsolute();

    }

/// Determines if the path represents a root directory
    Bool Path::IsRoot() const
    {
#ifdef NGIN_PLATFORM_WINDOWS
        return pathStr.length() == 3 && std::isalpha(pathStr[0]) && pathStr[1] == ':' && pathStr[2] == separator;
#else
        return pathStr.length() == 1 && pathStr[0] == separator;
#endif
    }

    Bool Path::IsDirectory() const
    {
        return !pathStr.empty() && pathStr.back() == separator;
    }

    Bool Path::IsFile() const
    {
        return !IsDirectory();
    }

    Bool Path::IsEmpty() const
    {
        return pathStr == emptyPath;
    }

    void Path::Normalize()
    {
        if (pathStr.empty())
        {
            // Set to platform-specific default directory
            pathStr = emptyPath;
            return;
        }

        // Replace incorrect slashes with correct platform-specific separator
        std::replace(pathStr.begin(), pathStr.end(), altSeparator, separator);


        // Remove consecutive slashes
        auto newEnd = std::unique(pathStr.begin(), pathStr.end(),
                                  [](Path::CharType a, Path::CharType b)
                                  {
                                      return a == b && a == separator;
                                  });
        pathStr.erase(newEnd, pathStr.end());

        // Remove trailing slash if present and not root
        if (pathStr.size() > 1 && pathStr.back() == separator)
            pathStr.pop_back();
    }

    Path::StringType Path::ConvertToNative(const String& str)
    {
#ifdef NGIN_PLATFORM_WINDOWS
        int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), nullptr, 0);
        StringType wStrTo(sizeNeeded, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &wStrTo[0], sizeNeeded);
        return wStrTo;
#else
        return str;
#endif
    }

    String Path::ConvertFromNative(const Path::StringType& str)
    {
#ifdef NGIN_PLATFORM_WINDOWS
        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &str[0], (int) str.size(), nullptr, 0, nullptr, nullptr);
        String strTo(sizeNeeded, 0);
        WideCharToMultiByte(CP_UTF8, 0, &str[0], (int) str.size(), &strTo[0], sizeNeeded, nullptr, nullptr);
        return strTo;
#else
        return str;
#endif
    }


}