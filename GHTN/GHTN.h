#pragma once

#ifdef GHTN_EXPORTS
#define GHTN_API __declspec(dllexport)
#else
#define GHTN_API __declspec(dllimport)
#endif
