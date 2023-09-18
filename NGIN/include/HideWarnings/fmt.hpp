
#ifdef _MSC_VER

#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#pragma warning(pop)
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wpedantic"
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#pragma GCC diagnostic pop
#else
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#endif