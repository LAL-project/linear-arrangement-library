
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

%ignore lal::graphs::operator<<;
%include "../lal/graphs/output.hpp"

// -------------------------
// Extendind the C++ classes

%extend lal::graphs::undirected_graph {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::graphs::directed_graph {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::graphs::rooted_tree {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::graphs::free_tree {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
