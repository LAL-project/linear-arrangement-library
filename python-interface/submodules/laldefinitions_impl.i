
%include documentation.i

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
	%template(__list_uint32) vector<uint32_t>;
	%template(__list_bool) vector<bool>;
	%template(__list_string) vector<string>;
	
	%template(__edge) pair<lal::node, lal::node>;
	%template(__edge_pair) pair<lal::edge, lal::edge>;
	
	%template(__list_edge) vector<lal::edge>;
	%template(__list_edge_pair) vector<lal::edge_pair>;
	
	%template(__list_list_node) vector<vector<lal::node>>;
	
	%template(__value_arrangement) pair<uint32_t, lal::linear_arrangement>;
}
