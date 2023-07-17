#pragma once
// In a header file
#ifdef NGIN_ENGINE_EXPORTS
#define NGIN_ENGINE_API __declspec(dllexport)
#else
#define NGIN_ENGINE_API __declspec(dllimport)
#endif

NGIN_ENGINE_API void MyFunction();
