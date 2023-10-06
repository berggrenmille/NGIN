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
        NGIN_API Path();

        NGIN_API explicit Path(const String& path);

#ifdef NGIN_PLATFORM_WINDOWS

        NGIN_API explicit Path(StringType path);

#endif

        NGIN_API Path(const Path& other) = default;

        NGIN_API Path& operator=(const Path& other) = default;

        NGIN_API Path(Path&& other) = default;

        NGIN_API Path& operator=(Path&& other) = default;

        NGIN_API ~Path() = default;

        [[nodiscard]] NGIN_API String ToString() const;

        [[nodiscard]] NGIN_API Bool IsAbsolute() const;

        [[nodiscard]] NGIN_API Bool IsRelative() const;

        [[nodiscard]] NGIN_API Bool IsRoot() const;

        [[nodiscard]] NGIN_API Bool IsDirectory() const;

        [[nodiscard]] NGIN_API Bool IsFile() const;

        [[nodiscard]] NGIN_API Bool IsEmpty() const;

        //Append operator /


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


