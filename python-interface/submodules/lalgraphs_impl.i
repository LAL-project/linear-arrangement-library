
%{

// C++ includes
#include <sstream>

// definitions
#include <lal/definitions.hpp>

// lal includes
#include <lal/graphs.hpp>
#include <lal/graphs/output.hpp>

%}

// ----------------
// lal header files

%include "../lal/graphs/graph.hpp"
%include "../lal/graphs/ugraph.hpp"
%include "../lal/graphs/dgraph.hpp"

%include "../lal/graphs/tree.hpp"
%include "../lal/graphs/rtree.hpp"

%include "../lal/graphs/utree.hpp"
%include "../lal/graphs/dtree.hpp"

%include "../lal/graphs/urtree.hpp"
%include "../lal/graphs/drtree.hpp"

%ignore lal::graphs::operator<<;
%include "../lal/graphs/output.hpp"

// -------------------------
// Extendind the C++ classes

%extend lal::graphs::ugraph {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::graphs::dgraph {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::graphs::urtree {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::graphs::drtree {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
