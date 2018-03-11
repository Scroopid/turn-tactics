#pragma once

#define PI 3.1415

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

ENGINE_API inline bool within(float number, float amount, float value);