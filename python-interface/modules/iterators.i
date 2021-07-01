%module iterators

%import definitions.i
%import graphs.i


%include documentation.i

%{

// C++ includes
#include <lal/graphs.hpp>
#include <lal/iterators.hpp>

%}

%include "../lal/iterators/E_iterator.hpp"
%template(E_iterator_directed_graphs) lal::iterators::E_iterator<lal::graphs::directed_graph>;
%template(E_iterator_undirected_graphs) lal::iterators::E_iterator<lal::graphs::undirected_graph>;
%template(E_iterator_rooted_tree) lal::iterators::E_iterator<lal::graphs::rooted_tree>;
%template(E_iterator_free_tree) lal::iterators::E_iterator<lal::graphs::free_tree>;

%include "../lal/iterators/Q_iterator.hpp"
%template(Q_iterator_directed_graphs) lal::iterators::Q_iterator<lal::graphs::directed_graph>;
%template(Q_iterator_undirected_graphs) lal::iterators::Q_iterator<lal::graphs::undirected_graph>;
%template(Q_iterator_rooted_tree) lal::iterators::Q_iterator<lal::graphs::rooted_tree>;
%template(Q_iterator_free_tree) lal::iterators::Q_iterator<lal::graphs::free_tree>;

%pythoncode %{
def E_iterator(g):
	r"""
	Returns an iterator-like object that iterates over the list of edges
	of the input graph `g`.
	
	Parameters
	----------
	* `g` :
		Input graph.
	"""
	__full_type = str(type(g))
	__pos_graph = __full_type.find(".graphs.")
	__type_graph = __full_type[__pos_graph + len(".graphs."):-2]
	return globals()[ "E_iterator_" + __type_graph ](g)

def Q_iterator(g):
	r"""
	Returns an iterator-like object that iterates over the list of pairs
	of independent edges the input graph `g`.
	
	Parameters
	----------
	* `g` :
		Input graph.
	"""
	__full_type = str(type(g))
	__pos_graph = __full_type.find(".graphs.")
	__type_graph = __full_type[__pos_graph + len(".graphs."):-2]
	return globals()[ "Q_iterator_" + __type_graph ](g)
%}

%pythoncode %{
__definitions = definitions
del definitions

# remove unnecessary modules
del graphs
del tree_type
%}
