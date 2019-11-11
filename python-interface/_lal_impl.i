%import dataset_error.i

%{

// -- C++ includes
#include <sstream>

// -- lal includes

// library's main directory
#include <lal/definitions.hpp>
#include <lal/graphs.hpp>

// iterators
#include <lal/iterators.hpp>

// io operations
#include <lal/io.hpp>

// generation of graphs
#include <lal/generation.hpp>

// linear arrangements
#include <lal/linarr.hpp>

// numeric
#include <lal/numeric.hpp>

// properties
#include <lal/properties.hpp>

%}

// --------------
// C++ interfaces

%include stdint.i
%include std_string.i
%include std_vector.i
%include std_pair.i
%include typemaps.i

// -------------------------
// renaming of C++ templates

%include "../lal/definitions.hpp"
namespace std {
	%template(edge) pair<lal::node, lal::node>;
	%template(edge_pair) pair<lal::edge, lal::edge>;
	
	%template(bool_list) vector<bool>;
	%template(node_list) vector<lal::node>;
	
	%template(edge_list) vector<lal::edge>;
	%template(edge_pair_list) vector<lal::edge_pair>;
	%template(rational_list) vector<lal::numeric::rational>;
	%template(string_list) vector<string>;
	
	%template(list_node_list) vector<vector<lal::node> >;
}

// ---------------------
// basic data structures

// numeric
%include "../lal/numeric/integer.hpp"
%include "../lal/numeric/rational.hpp"

// graphs
%include "../lal/graphs/graph.hpp"
%include "../lal/graphs/tree.hpp"
%include "../lal/graphs/rtree.hpp"
%include "../lal/graphs/ugraph.hpp"
%include "../lal/graphs/utree.hpp"
%include "../lal/graphs/urtree.hpp"
%include "../lal/graphs/dgraph.hpp"
%include "../lal/graphs/dtree.hpp"
%include "../lal/graphs/drtree.hpp"

// -------------
// io operations
%include "../lal/iterators/edge_iterator.hpp"
%include "../lal/iterators/Q_iterator.hpp"

// -------------
// io operations

%include "../lal/io/edge_list.hpp"
%include "../lal/io/basic_output.hpp"
%include "../lal/io/treebank_processor.hpp"
%include "../lal/io/treebank_reader.hpp"
%include "../lal/io/treebank_dataset.hpp"

// --------------------
// properties of graphs

%include "../lal/properties/C_rla.hpp"
%include "../lal/properties/D_rla.hpp"
%include "../lal/properties/degrees.hpp"
%include "../lal/properties/mhd.hpp"
%include "../lal/properties/Q.hpp"

// -------------------
// linear arrangements

%include "../lal/linarr/C.hpp"
%include "../lal/linarr/D.hpp"
%include "../lal/linarr/headedness.hpp"

// --------------------
// generation of graphs

%include "../lal/generation/free_ulab_trees.hpp"
%include "../lal/generation/free_lab_trees.hpp"
%include "../lal/generation/rand_free_lab_trees.hpp"
%include "../lal/generation/rand_free_ulab_trees.hpp"
%include "../lal/generation/rand_rooted_lab_dir_trees.hpp"

// ---------------------------------
// renaming of methods and operators

%rename(__mod__) operator% (uint64_t) const;
%rename(__mod__) operator% (const integer&) const;

// ----------------------------
// Extending some C++ templates

%extend std::vector<bool> {
	std::string __str__() const {
		std::ostringstream out;
		out << "[";
		if ($self->size() > 0) {
			out << ( (*$self)[0] ? "1" : "0" );
		}
		for (size_t i = 1; i < $self->size(); ++i) {
			out << ", " << ( (*$self)[i] ? "1" : "0" );
		}
		out << "]";
		return out.str();
	}
};

%extend std::vector<lal::node> {
	std::string __str__() const {
		std::ostringstream out;
		out << "[";
		if ($self->size() > 0) {
			out << (*$self)[0];
		}
		for (size_t i = 1; i < $self->size(); ++i) {
			out << ", " << (*$self)[i];
		}
		out << "]";
		return out.str();
	}
};

%extend std::vector<lal::edge> {
	std::string __str__() const {
		std::ostringstream out;
		out << "[";
		if ($self->size() > 0) {
			out << "("
				<< (*$self)[0].first << ", "
				<< (*$self)[0].second << ")";
		}
		for (size_t i = 1; i < $self->size(); ++i) {
			out << ", ";
			out << "("
				<< (*$self)[i].first << ", "
				<< (*$self)[i].second
				<< ")";
		}
		out << "]";
		return out.str();
	}
};

%extend std::vector<lal::edge_pair> {
	std::string __str__() const {
		std::ostringstream out;
		out << "{";
		if ($self->size() > 0) {
			out << "["
				<< "(" << (*$self)[0].first.first << ", " << (*$self)[0].first.second << ")"
				<< ", "
				<< "(" << (*$self)[0].second.first << ", " << (*$self)[0].second.second << ")"
				<< "]";
		}
		for (size_t i = 1; i < $self->size(); ++i) {
			out << ", "
				<< "["
				<< "(" << (*$self)[i].first.first << ", " << (*$self)[i].first.second << ")"
				<< ", "
				<< "(" << (*$self)[i].second.first << ", " << (*$self)[i].second.second << ")"
				<< "]";
		}
		out << "}";
		return out.str();
	}
};

// -------------------------
// Extendind the C++ classes

// -- GRAPHS

%extend lal::graphs::graph {
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

// -- NUMERIC

%extend lal::numeric::integer {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::numeric::rational {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
