
#ifdef _MSC_VER

#include <codeanalysis\warnings.h>
#pragma warning( push )
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include <nlohmann/json.hpp>
#pragma warning( pop )

#else

#include <nlohmann/json.hpp>

#endif