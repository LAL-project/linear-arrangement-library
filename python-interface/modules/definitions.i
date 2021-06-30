%module definitions

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
	%template(_list_uint32) vector<uint32_t>;
	%template(_list_uint64) vector<uint64_t>;
	%template(_list_bool) vector<bool>;
	%template(_list_string) vector<string>;
	
	%template(_edge) pair<lal::node, lal::node>;
	%template(_edge_pair) pair<lal::edge, lal::edge>;
	
	%template(_list_edge) vector<lal::edge>;
	%template(_list_edge_pair) vector<lal::edge_pair>;
	
	%template(_list_list_node) vector<vector<lal::node>>;
}
