//
// Created by Maximiliam Berggren on 05/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
// STL Include(s)

// Other Include(s)

namespace NGIN::VFS
{
    class Path
    {
    protected:
#ifdef NGIN_PLATFORM_WINDOWS
        using CharType = wchar_t;
        using StringType = std::basic_string<CharType>;
#else
        using CharType = char;
        using StringType = std::basic_string<CharType>;
#endif
    public:
        NGIN_API explicit Path(const String& path);


        NGIN_API Path(const Path& other) = default;

        NGIN_API Path& operator=(const Path& other) = default;

        NGIN_API Path(Path&& other) = default;

        NGIN_API Path& operator=(Path&& other) = default;

        NGIN_API ~Path() = default;

        NGIN_API String ToString() const;

        NGIN_API Bool IsAbsolute() const;

        NGIN_API Bool IsRelative() const;

        NGIN_API Bool IsRoot() const;

        NGIN_API Bool IsDirectory() const;

        NGIN_API Bool IsFile() const;

        NGIN_API Bool IsEmpty() const;

        //Append operator /
        NGIN_API Path& operator/=(const Path& other) const;

        NGIN_API Path& operator+=(const Path& other);

    protected:
        StringType pathStr;

        NGIN_API void Normalize();

        NGIN_API static StringType ConvertToNative(const String& str);

        NGIN_API static String ConvertFromNative(const StringType& str);

#ifdef NGIN_PLATFORM_WINDOWS
        inline static const CharType separator = L'\\';
        inline static const CharType altSeparator = L'/';
        inline static const StringType emptyPath = L".\\";
#else
        static constexpr Char separator = '/';
        static constexpr Char altSeparator = '\\';
        static constexpr StringType emptyPath = "./";
#endif
    };


}


