
%{

// C++ includes
#include <sstream>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs.hpp>

%}

// ----------------
// lal header files

%ignorewarn("graph - Move constructor ignored") graph(graph &&);
%ignorewarn("tree - Move constructor ignored") tree(tree &&);
%ignorewarn("ugraph - Move constructor ignored") undirected_graph(undirected_graph &&);
%ignorewarn("dgraph - Move constructor ignored") directed_graph(directed_graph &&);
%ignorewarn("rtree - Move constructor ignored") rooted_tree(rooted_tree &&);
%ignorewarn("ftree - Move constructor ignored") free_tree(free_tree &&);

%ignorewarn("graph - Move assignment ignored") graph::operator=(graph &&);
%ignorewarn("tree - Move assignment ignored") tree::operator=(tree &&);
%ignorewarn("ugraph - Move assignment ignored") undirected_graph::operator=(undirected_graph &&);
%ignorewarn("dgraph - Move assignment ignored") directed_graph::operator=(directed_graph &&);
%ignorewarn("rtree - Move assignment ignored") rooted_tree::operator=(rooted_tree &&);
%ignorewarn("ftree - Move assignment ignored") free_tree::operator=(free_tree &&);

%ignore lal::graphs::operator<<;

%include "../lal/graphs/graph.hpp"
%include "../lal/graphs/undirected_graph.hpp"
%include "../lal/graphs/directed_graph.hpp"

%include "../lal/graphs/tree.hpp"
%include "../lal/graphs/free_tree.hpp"
%include "../lal/graphs/rooted_tree.hpp"

%include "../lal/graphs/output.hpp"

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
