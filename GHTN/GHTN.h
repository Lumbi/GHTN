#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <deque>
#include <format>
#include <functional>
#include <initializer_list>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <unordered_map>
#include <variant>
#include <vector>

#ifdef GHTN_EXPORTS
#define GHTN_API __declspec(dllexport)
#else
#define GHTN_API __declspec(dllimport)
#endif
