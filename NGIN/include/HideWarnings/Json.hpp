
#pragma once
#define RAPIDJSON_NOMEMBERITERATORCLASS 1
#define RAPIDJSON_BUILD_CXX17 ON
#define RAPIDJSON_BUILD_TESTS OFF
#define RAPIDJSON_BUILD_EXAMPLES OFF
#define RAPIDJSON_BUILD_DOC OFF

#ifdef _MSC_VER
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#pragma warning(pop)
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wpedantic"
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#pragma GCC diagnostic pop
#else
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
.hpp >
#endif