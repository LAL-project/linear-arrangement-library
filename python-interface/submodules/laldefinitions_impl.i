
%{
#include <lal/definitions.hpp>
#include <lal/linarr/dependency_flux.hpp>

// this header is need for the definition of the variable
// __tree_structure_type_size
#include <lal/linarr/syntactic_dependency_structure_type.hpp>
%}

// --------------
// C++ interfaces

%include stdint.i
%include std_array.i
%include std_vector.i
%include std_string.i
%include std_pair.i

// -------------------------
// renaming of C++ templates

%include "../lal/definitions.hpp"

namespace std {
	%template(list_uint32) vector<uint32_t>;
	%template(list_bool) vector<bool>;
	%template(list_string) vector<string>;

	%template(edge) pair<lal::node, lal::node>;
	%template(edge_pair) pair<lal::edge, lal::edge>;
	
	%template(list_edge) vector<lal::edge>;
	%template(list_edge_pair) vector<lal::edge_pair>;
	
	%template(list_list_node) vector<vector<lal::node>>;
	
	%template(value_arrangement) pair<uint32_t, lal::linear_arrangement>;
	
	%template(array_of_bools_tree_structure_type) array<bool, lal::linarr::__tree_structure_type_size>;
}

// this has to be 'included' after 'list_edge' is created!
%include "../lal/linarr/dependency_flux.hpp"

namespace std {
	%template(list_dependency_flux) vector<lal::linarr::dependency_flux>;
}
