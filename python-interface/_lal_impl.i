%import algorithms_crossings.i
%import dataset_error.i
%import tree_structure_type.i

%{

// C++ includes
#include <sstream>

// lal includes

// library's types
#include <lal/definitions.hpp>

// generation of graphs
#include <lal/generation.hpp>

// graphs
#include <lal/graphs.hpp>

// io operations
#include <lal/io.hpp>

// iterators
#include <lal/iterators.hpp>

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

// -------------------------
// renaming of C++ templates

%include "../lal/definitions.hpp"
namespace std {
	%template(edge) pair<lal::node, lal::node>;
	%template(edge_pair) pair<lal::edge, lal::edge>;
	
	%template(bool_list) vector<bool>;
	%template(uint64_list) vector<uint64_t>;
	
	%template(edge_list) vector<lal::edge>;
	%template(edge_pair_list) vector<lal::edge_pair>;
	
	%template(list_node_list) vector<vector<lal::node> >;
}

%ignore lal::graphs::operator<<;
%ignore lal::numeric::operator<<;
%ignore lal::numeric::integer::operator-();
%ignore lal::numeric::rational::operator-();
%ignore lal::numeric::integer::operator=;
%ignore lal::numeric::rational::operator=;

// ---------------------
// basic data structures

// numeric
%include "../lal/numeric/integer.hpp"
%include "../lal/numeric/rational.hpp"
%include "../lal/numeric/output.hpp"

// graphs
%include "../lal/graphs/graph.hpp"
%include "../lal/graphs/ugraph.hpp"
%include "../lal/graphs/dgraph.hpp"

%include "../lal/graphs/tree.hpp"
%include "../lal/graphs/rtree.hpp"

%include "../lal/graphs/utree.hpp"
%include "../lal/graphs/dtree.hpp"

%include "../lal/graphs/urtree.hpp"
%include "../lal/graphs/drtree.hpp"

%include "../lal/graphs/output.hpp"

// -------------
// iterators
%include "../lal/iterators/edge_iterator.hpp"
%include "../lal/iterators/Q_iterator.hpp"

// -------------
// io operations

// %include "../lal/io/basic_output.hpp"
// %include "../lal/io/dataset_error.hpp"
%include "../lal/io/edge_list.hpp"
%include "../lal/io/treebank_reader.hpp"
%include "../lal/io/treebank_dataset.hpp"
%include "../lal/io/treebank_processor.hpp"

// --------------------
// properties of graphs

%include "../lal/properties/C_rla.hpp"
%include "../lal/properties/D_rla.hpp"
%include "../lal/properties/degrees.hpp"
%include "../lal/properties/mhd.hpp"
%include "../lal/properties/Q.hpp"

// -------------------
// linear arrangements

%include "../lal/linarr/1level.hpp"
%include "../lal/linarr/2level.hpp"
// %include "../lal/linarr/algorithms_crossings.hpp"
%include "../lal/linarr/C.hpp"
%include "../lal/linarr/classification.hpp"
%include "../lal/linarr/D.hpp"
%include "../lal/linarr/headedness.hpp"
// %include "../lal/linarr/tree_structure_type.hpp"

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
%extend lal::graphs::rtree {
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

// --------------------------------
// Extendind the function templates

%template(MDD_1level_rational_ugraph)	lal::linarr::MDD_1level_rational<lal::graphs::ugraph>;
%template(MDD_1level_rational_dgraph)	lal::linarr::MDD_1level_rational<lal::graphs::dgraph>;
%template(MDD_1level_rational_utree)	lal::linarr::MDD_1level_rational<lal::graphs::utree>;
%template(MDD_1level_rational_urtree)	lal::linarr::MDD_1level_rational<lal::graphs::urtree>;
%template(MDD_1level_rational_dtree)	lal::linarr::MDD_1level_rational<lal::graphs::dtree>;
%template(MDD_1level_rational_drtree)	lal::linarr::MDD_1level_rational<lal::graphs::drtree>;

%template(MDD_1level_ugraph)	lal::linarr::MDD_1level<lal::graphs::ugraph>;
%template(MDD_1level_dgraph)	lal::linarr::MDD_1level<lal::graphs::dgraph>;
%template(MDD_1level_utree)		lal::linarr::MDD_1level<lal::graphs::utree>;
%template(MDD_1level_urtree)	lal::linarr::MDD_1level<lal::graphs::urtree>;
%template(MDD_1level_dtree)		lal::linarr::MDD_1level<lal::graphs::dtree>;
%template(MDD_1level_drtree)	lal::linarr::MDD_1level<lal::graphs::drtree>;

%template(MDD_2level_rational_ugraph)	lal::linarr::MDD_2level_rational<lal::graphs::ugraph>;
%template(MDD_2level_rational_dgraph)	lal::linarr::MDD_2level_rational<lal::graphs::dgraph>;
%template(MDD_2level_rational_utree)	lal::linarr::MDD_2level_rational<lal::graphs::utree>;
%template(MDD_2level_rational_urtree)	lal::linarr::MDD_2level_rational<lal::graphs::urtree>;
%template(MDD_2level_rational_dtree)	lal::linarr::MDD_2level_rational<lal::graphs::dtree>;
%template(MDD_2level_rational_drtree)	lal::linarr::MDD_2level_rational<lal::graphs::drtree>;

%template(MDD_2level_ugraph)	lal::linarr::MDD_2level<lal::graphs::ugraph>;
%template(MDD_2level_dgraph)	lal::linarr::MDD_2level<lal::graphs::dgraph>;
%template(MDD_2level_utree)		lal::linarr::MDD_2level<lal::graphs::utree>;
%template(MDD_2level_urtree)	lal::linarr::MDD_2level<lal::graphs::urtree>;
%template(MDD_2level_dtree)		lal::linarr::MDD_2level<lal::graphs::dtree>;
%template(MDD_2level_drtree)	lal::linarr::MDD_2level<lal::graphs::drtree>;
