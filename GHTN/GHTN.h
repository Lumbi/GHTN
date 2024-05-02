#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <deque>
#include <functional>
#include <initializer_list>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <unordered_map>
#include <variant>
#include <vector>

#if defined(GHTN_EXPORTS)
#define GHTN_API __declspec(dllexport)
#elif defined(GHTN_IMPORTS)
#define GHTN_API __declspec(dllimport)
#else
#define GHTN_API
#endif
