%module graphs

%include std_vector.i

%include documentation.i
%import definitions.i
%import enums/tree_type.i

%{

// C++ includes
#include <sstream>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs.hpp>

%}

// ----------------
// lal header files

%include "../lal/graphs/graph.hpp"
%include "../lal/graphs/undirected_graph.hpp"
%include "../lal/graphs/directed_graph.hpp"

%include "../lal/graphs/tree.hpp"
%include "../lal/graphs/free_tree.hpp"
%include "../lal/graphs/rooted_tree.hpp"

// -------------------------
// Extendind the C++ classes

%extend lal::graphs::undirected_graph {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
	undirected_graph clone() const {
		return *$self;
	}
}
%extend lal::graphs::directed_graph {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
	directed_graph clone() const {
		return *$self;
	}
}
%extend lal::graphs::rooted_tree {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
	rooted_tree clone() const {
		return *$self;
	}
}
%extend lal::graphs::free_tree {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
	free_tree clone() const {
		return *$self;
	}
}

// making lists of graphs...
namespace std {
	%template(__list_undirected_graph) vector<lal::graphs::undirected_graph>;
	%template(__list_directed_graph) vector<lal::graphs::directed_graph>;
	%template(__list_free_tree) vector<lal::graphs::free_tree>;
	%template(__list_rooted_tree) vector<lal::graphs::rooted_tree>;
}

%include "../lal/graphs/conversions.hpp"

%pythoncode %{
__definitions = definitions
del definitions
%}
