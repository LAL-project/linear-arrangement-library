#pragma once

#include <string_view>

namespace lal {
namespace utilities {

/**
 * @brief A decorator for formatted output.
 *
 * This is a helper class to more easily write formatted multiline output.
 * @tparam T The thing that we want to output.
 */
template <typename T>
struct decorator {
	/// Prefix string.
	std::string_view prefix;
	/// Value to be written into output.
	const T& value;
	/// Suffix string.
	std::string_view suffix;
};

namespace decorator_operators {

template <typename T>
[[nodiscard]] static inline decorator<T>
operator+ (std::string_view w, const T& t) noexcept
{
	return decorator<T>{.prefix = w, .value = t, .suffix = ""};
}

template <typename T>
[[nodiscard]] static inline decorator<T>
operator+ (const T& t, std::string_view w) noexcept
{
	return decorator<T>{.prefix = "", .value = t, .suffix = w};
}

template <typename T>
[[nodiscard]] static inline decorator<T>
operator+ (decorator<T> t, std::string_view w) noexcept
{
	t.suffix = w;
	return t;
}

template <typename T>
[[nodiscard]] static inline decorator<T>
operator+ (std::string_view w, decorator<T> t) noexcept
{
	t.prefix = w;
	return t;
}

} // namespace decorator_operators

} // namespace utilities
} // namespace lal
