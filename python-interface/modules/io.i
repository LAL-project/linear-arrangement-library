%module io

%import definitions.i
%import graphs.i
%import enums/dataset_error.i

%include documentation.i

%{

#include <optional>

// lal includes
#include <lal/graphs.hpp>
#include <lal/io.hpp>

template<class T>
struct LAL__optional {
	std::optional<T> m_opt;
	inline bool has_contents() const noexcept { return (m_opt ? true : false); }
	inline T contents() const noexcept { return *m_opt; }
};

template<class T> LAL__optional<T> read_edge_list
(const std::string& filename, bool norm = true, bool check = true) {
	LAL__optional<T> r;
	r.m_opt = lal::io::read_edge_list<T>(filename, norm, check);
	return r;
}

template<class T> LAL__optional<T> read_head_vector
(const std::string& filename, bool norm = true, bool check = true) {
	LAL__optional<T> r;
	r.m_opt = lal::io::read_head_vector<T>(filename, norm, check);
	return r;
}

%}

/* ---------- WRAP LAL__optional ------------- */

template<class T>
struct LAL__optional {
	std::optional<T> m_opt;
	bool has_contents() const noexcept { return (m_opt ? true : false); }
	T contents() const noexcept { return *m_opt; }
};

// ---------- INSTANTIATE LAL__optional

%template(LAL__optional_directed_graph) LAL__optional<lal::graphs::directed_graph>;
%template(LAL__optional_undirected_graph) LAL__optional<lal::graphs::undirected_graph>;
%template(LAL__optional_free_tree) LAL__optional<lal::graphs::free_tree>;
%template(LAL__optional_rooted_tree) LAL__optional<lal::graphs::rooted_tree>;

/* ---------- WRAP read_edge_list ------------- */

template<class T> LAL__optional<T> read_edge_list
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
		A string that encoding the type of graph. Must be one of:
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
		print("Error: type '%s' is not valid" % gtype)
		return None
	
	opt = globals()[ "read_edge_list_" + gtype ](filename, norm, check)
	if opt.has_contents(): return opt.contents()
	return None
%}

/* ---------- WRAP read_head_vector ------------- */

template<class T> LAL__optional<T> read_head_vector
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
		A string that encoding the type of graph. Must be one of:
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
		print("Error: type '%s' is not valid" % gtype)
		return None
	
	opt = globals()[ "read_head_vector_" + gtype ](filename, norm, check)
	if opt.has_contents(): return opt.contents()
	return None
%}



%include std_string.i

%include "../lal/io/treebank_reader.hpp"
%include "../lal/io/treebank_dataset_reader.hpp"
%include "../lal/io/treebank_dataset_processor.hpp"






%pythoncode %{

__definitions = definitions
del definitions

# remove unnecessary modules
del graphs
del tree_type

# In this piece of code we modify class "treebank_dataset_processor".
# Remember: "treebank_dataset_processor" is the automatic processing
# of a dataset.
#
# In this piece of code we create two subclasses for this class,
# one for each enumeration it has: "tree_feature" and "processor_error".
# As created by SWIG, class "treebank_dataset_processor" has attributes of the
# form:
# 		treebank_dataset_processor.processor_error_none
# 		treebank_dataset_processor.processor_error_missing_parent
# 		treebank_dataset_processor.processor_error_missing_main
# 		....
# and also
# 		treebank_dataset_processor.tree_feature_n
# 		treebank_dataset_processor.tree_feature_k2
# 		treebank_dataset_processor.tree_feature_k3
# 		....
# Our aim in this piece of code is to make two subclasses so that the
# values of the enumerations "tree_feature" and "processor_error"
# are members of classes, namely, we want class "treebank_dataset_processor"
# to be usable in this way:
# 		treebank_dataset_processor.processor_error.none
# 		treebank_dataset_processor.processor_error.missing_parent
# 		treebank_dataset_processor.processor_error.missing_main
# 		....
# and also
# 		treebank_dataset_processor.tree_feature.n
# 		treebank_dataset_processor.tree_feature.k2
# 		treebank_dataset_processor.tree_feature.k3
# 		....


# -------------------
# Make the subclasses
#     Note: since 'processor_error' and 'tree_feature' have effectively
#     been turned into actual python classes, they will have a mro() method.

class processor_error:
	__dummy_attr = 0

class tree_feature:
	__dummy_attr = 0

# --------------------------------------------
# add the enumaration values to the subclasses
tbpr = treebank_dataset_processor

# processor_error
prerr_enum_vals = list(filter(lambda k: k.find("processor_error_") == 0, tbpr.__dict__.keys()))
for enval_name in prerr_enum_vals:
	# make new enumeration value name
	new_enval_name = enval_name[len("processor_error_"):]
	# set attribute to class "processor_error"
	setattr(processor_error, new_enval_name, getattr(tbpr, enval_name))
	# delete enumeration value from "treebank_dataset_processor"
	delattr(tbpr, enval_name)

# tree_features
tf_enum_vals = list(filter(lambda k: k.find("tree_feature_") == 0, tbpr.__dict__.keys()))
for enval_name in tf_enum_vals:
	# make new enumeration value name
	new_enval_name = enval_name[len("tree_feature_"):]
	# set attribute to class "tree_feature"
	setattr(tree_feature, new_enval_name, getattr(tbpr, enval_name))
	# delete enumeration value from "treebank_dataset_processor"
	delattr(tbpr, enval_name)

# -------------------------------------------
# clean up dummy attributes of the subclasses
delattr(processor_error, "_processor_error__dummy_attr")
delattr(tree_feature, "_tree_feature__dummy_attr")

# -----------------------------------------
# make "processor_error" and "tree_feature"
# attributes of class "treebank_dataset_processor"

# set the subclasses
setattr(tbpr, "processor_error", processor_error)
setattr(tbpr, "tree_feature", tree_feature)

# ------------------------
# clean up local variables

del tbpr
del prerr_enum_vals, tf_enum_vals
del enval_name
del new_enval_name

# even the classes!
del processor_error
del tree_feature
%}
