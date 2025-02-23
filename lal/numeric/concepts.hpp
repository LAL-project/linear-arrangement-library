#pragma once

// C++ includes
#include <type_traits>

namespace lal {
namespace numeric {

/**
 * @brief Is a given type integral?
 * @tparam value_t The type to check.
 */
template <typename value_t>
concept Integral = std::is_integral_v<value_t>;

} // namespace numeric
} // namespace lal
