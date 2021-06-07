%module io

%include std_string.i
%include std_vector.i

%import definitions.i
%import graphs.i
%import enums/treebank_error_type.i
%import enums/treebank_feature.i

%include documentation.i

%{

// C++ includes
#include <optional>
#include <sstream>

// lal includes
#include <lal/graphs.hpp>
#include <lal/io.hpp>

template<class T>
struct _optional {
	std::optional<T> m_opt;
	inline bool has_contents() const noexcept { return (m_opt ? true : false); }
	inline T contents() const noexcept { return *m_opt; }
};

template<class T> _optional<T> read_edge_list
(const std::string& filename, bool norm = true, bool check = true) {
	_optional<T> r;
	r.m_opt = lal::io::read_edge_list<T>(filename, norm, check);
	return r;
}

template<class T> _optional<T> read_head_vector
(const std::string& filename, bool norm = true, bool check = true) {
	_optional<T> r;
	r.m_opt = lal::io::read_head_vector<T>(filename, norm, check);
	return r;
}

%}

/* ---------- WRAP _optional ------------- */

template<class T>
struct _optional {
	std::optional<T> m_opt;
	bool has_contents() const noexcept { return (m_opt ? true : false); }
	T contents() const noexcept { return *m_opt; }
};

// ---------- INSTANTIATE _optional

%template(_optional_directed_graph) _optional<lal::graphs::directed_graph>;
%template(_optional_undirected_graph) _optional<lal::graphs::undirected_graph>;
%template(_optional_free_tree) _optional<lal::graphs::free_tree>;
%template(_optional_rooted_tree) _optional<lal::graphs::rooted_tree>;

/* ---------- WRAP read_edge_list ------------- */

template<class T> _optional<T> read_edge_list
(const std::string& filename, bool norm = true, bool check = true);

// ---------- INSTANTIATE read_edge_list

%template(read_edge_list_directed_graph) read_edge_list<lal::graphs::directed_graph>;
%template(read_edge_list_undirected_graph) read_edge_list<lal::graphs::undirected_graph>;
%template(read_edge_list_free_tree) read_edge_list<lal::graphs::free_tree>;
%template(read_edge_list_rooted_tree) read_edge_list<lal::graphs::rooted_tree>;

/* ---------- CALL read_edge_list ------------- */

%pythoncode %{

def read_edge_list(gtype, filename, norm = True, check = True):
	r"""
	Reads a graph in edge list format. This format consists of a list of
	all the graph's edges. Each edge is described as a pair of indices of
	the nodes at each end of the edge. Nodes are labelled with indices
	starting at 0. The resulting number of nodes of the graph will be the
	maximum index in the file plus 1.
	
	Parameters
	----------
	* `gtype` :
		A string that encodes the type of graph. Must be one of:
		
		- free_tree
		- rooted_tree
		- undirected_graph
		- directed_graph
	
	* `filename` :
		The name of the file from which the list of edges is to be read.
	
	* `norm` :
		Is the graph to be normalised?
	
	* `check` :
		If the graph is not to be normalised check whether or not the graph read is normalised.
	
	Returns
	-------
	A graph object of the same type as the string `gtype`
	"""
	
	if gtype not in ["undirected_graph", "directed_graph", "free_tree", "rooted_tree"]:
		print("Error: graph type '%s' is not valid" % gtype)
		return None
	
	opt = globals()[ "read_edge_list_" + gtype ](filename, norm, check)
	if opt.has_contents(): return opt.contents()
	return None
%}

/* ---------- WRAP read_head_vector ------------- */

template<class T> _optional<T> read_head_vector
(const std::string& filename, bool norm = true, bool check = true);

// ---------- INSTANTIATE read_head_vector

%template(read_head_vector_free_tree) read_head_vector<lal::graphs::free_tree>;
%template(read_head_vector_rooted_tree) read_head_vector<lal::graphs::rooted_tree>;

/* ---------- CALL read_edge_list ------------- */

%pythoncode %{

def read_head_vector(gtype, filename, norm = True, check = True):
	r"""
	A head vector of an @e n-vertex tree is a list of non-negative integer numbers. The number at position @e i denotes the parent node of the vertex at said position. Value '0' denotes the root. In this case, the vertex corresponding to the value '0' is not labelled as a root.
	
	Each tree is formatted as a list of whole, positive numbers (including zero), each representing a node of the tree. The number 0 denotes the root of the tree, and a number at a certain position indicates its parent node. For example, when number 4 is at position 9 it means that node 9 has parent node 4. Therefore, if number 0 is at position 1 it means that node 1 is the root of the tree. A complete example of such a tree's representation is the following

		0 3 4 1 6 3

	which should be interpreted as

		(a) predecessor:       0 3 4 1 6 3
		(b) node of the tree:  1 2 3 4 5 6

	Note that lines like these are not valid:

		(1) 0 2 2 2 2 2
		(2) 2 0 0

	Line (1) is not valid due to a self-reference in the second position, and (2) not being valid due to containing two '0' (i.e., two roots).
	
	Parameters
	----------
	* `gtype` :
		A string that encodes the type of graph. Must be one of:
		- free_tree
		- rooted_tree
		- undirected_graph
		- directed_graph
	
	* `filename` :
		The name of the file from which the list of edges is to be read.
	
	* `norm` :
		Is the graph to be normalised?
	
	* `check` :
		If the graph is not to be normalised check whether or not the graph read is normalised.
	
	Returns
	-------
	A graph object of the same type as the string `gtype`
	"""
	
	if gtype not in ["undirected_graph", "directed_graph", "free_tree", "rooted_tree"]:
		print("Error: graph type '%s' is not valid" % gtype)
		return None
	
	opt = globals()[ "read_head_vector_" + gtype ](filename, norm, check)
	if opt.has_contents(): return opt.contents()
	return None
%}

/* ------- WRAP the correctness functions that return error messages -------- */

%include "../lal/io/report_correctness.hpp"

%extend lal::io::report_treebank_file {
	std::string __repr__() const noexcept {
		std::ostringstream out;
		out << "(" << $self->get_line_number() << ", " << $self->get_error_message() << ")";
		return out.str();
	}
}

%extend lal::io::report_treebank_collection {
	std::string __repr__() const noexcept {
		std::ostringstream out;
		out << "(Treebank: "                   << $self->get_treebank_file_name()
			<< ", Line within main file: "     << $self->get_line_within_main_file()
			<< ", Line within treebank file: " << $self->get_treebank_file_line()
			<< ", "                            << $self->get_error_message()
			<< ")";
		return out.str();
	}
}

namespace std {
	%template(_list_report_treebank_file) vector<lal::io::report_treebank_file>;
	%template(_list_report_treebank_collection) vector<lal::io::report_treebank_collection>;
}

%include "../lal/io/check_correctness.hpp"

/* -------------------------------------------------------------------------- */

%include "../lal/io/treebank_error.hpp"

%extend lal::io::treebank_error {
	std::string __repr__() const noexcept {
		std::ostringstream out;
		if ($self->get_error_type() != lal::io::treebank_error_type::no_error) {
			out << "Error " << static_cast<size_t>($self->get_error_type())
				<< ": " << $self->get_error_message();
		}
		return out.str();
	}
}

%include "../lal/io/process_treebank_base.hpp"

%include "../lal/io/treebank_reader.hpp"
%include "../lal/io/treebank_collection_reader.hpp"

%include "../lal/io/treebank_processor.hpp"
%include "../lal/io/treebank_collection_processor.hpp"

/* -------------------------------------------------------------------------- */

%pythoncode %{
__definitions = definitions
del definitions

# remove unnecessary modules
del graphs
del tree_type
%}
