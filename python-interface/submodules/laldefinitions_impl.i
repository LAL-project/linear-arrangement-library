
%{
#include <lal/definitions.hpp>
%}

// --------------
// C++ interfaces

%include stdint.i
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
}
