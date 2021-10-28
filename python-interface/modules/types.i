%module types

%include documentation.i

%{
// C++ includes
#include <iostream>
#include <sstream>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/linear_arrangement.hpp>
%}

// --------------
// C++ interfaces

%include stdint.i
%include std_vector.i
%include std_string.i
%include std_pair.i

// -------------------------
// renaming of C++ templates
// (C++ types)

namespace std {
	%template(_list_uint32) vector<uint32_t>;
	%template(_list_uint64) vector<uint64_t>;
	%template(_list_bool) vector<bool>;
	%template(_list_string) vector<string>;
}

// ----------------
// ignore operators

namespace lal {

%ignore node_t::operator++ () noexcept;
%ignore node_t::operator-- () noexcept;
%ignore node_t::operator=;

%ignore position_t::operator++ () noexcept;
%ignore position_t::operator-- () noexcept;
%ignore position_t::operator=;

%ignore linear_arrangement::linear_arrangement(linear_arrangement&&) noexcept;
%ignore linear_arrangement::operator= (const linear_arrangement&) noexcept;
%ignore linear_arrangement::operator= (linear_arrangement&&) noexcept;
%ignore linear_arrangement::operator= (const std::vector<position>&) noexcept;
%ignore linear_arrangement::operator[];
%ignore linear_arrangement::operator[];
%ignore linear_arrangement::begin_direct() noexcept;
%ignore linear_arrangement::end_direct() noexcept;
%ignore linear_arrangement::begin_inverse() noexcept;
%ignore linear_arrangement::end_inverse() noexcept;

}

// wrap code

%include "../lal/basic_types.hpp"
%include "../lal/linear_arrangement.hpp"

%template(assign) lal::linear_arrangement::assign<
	lal::node,lal::position,
	std::enable_if_t<
		(std::is_integral_v<lal::node> or std::is_same_v<lal::node, node_t>)
		and
		(std::is_integral_v<lal::position> or std::is_same_v<lal::position, position_t>)
	,
	bool> = true>;

namespace lal {

%extend linear_arrangement {
	std::string __repr__() const {
		std::ostringstream out;
		out << "(";
		const std::size_t size = $self->size();
		if (size > 0) {
			out << (*$self)[lal::node_t{0}];
			for (lal::node_t u = 1; u < size; ++u) {
				out << ", " << (*$self)[u];
			}
		}
		out << ") | (";
		if (size > 0) {
			out << (*$self)[lal::position_t{0}];
			for (lal::position_t p = 1; p < size; ++p) {
				out << ", " << (*$self)[p];
			}
		}
		out << ")";
		return out.str();
	}
}

}

// -------------------------
// renaming of C++ templates
// (lal types)

namespace std {
	%template(_edge) pair<lal::node, lal::node>;
	%template(_edge_pair) pair<lal::edge, lal::edge>;
	
	%template(_list_edge) vector<lal::edge>;
	%template(_list_edge_pair) vector<lal::edge_pair>;
}

%pythoncode %{
del linear_arrangement_from_direct
del linear_arrangement_from_inverse
%}
