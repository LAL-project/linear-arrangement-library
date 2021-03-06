
%include documentation.i

%{

// C++ includes
#include <lal/graphs.hpp>
#include <lal/iterators.hpp>

%}

%include "../lal/iterators/E_iterator.hpp"
%template(__E_iterator_directed_graphs) lal::iterators::E_iterator<lal::graphs::directed_graph>;
%template(__E_iterator_undirected_graphs) lal::iterators::E_iterator<lal::graphs::undirected_graph>;
%template(__E_iterator_rooted_tree) lal::iterators::E_iterator<lal::graphs::rooted_tree>;
%template(__E_iterator_free_tree) lal::iterators::E_iterator<lal::graphs::free_tree>;

%include "../lal/iterators/Q_iterator.hpp"
%template(__Q_iterator_directed_graphs) lal::iterators::Q_iterator<lal::graphs::directed_graph>;
%template(__Q_iterator_undirected_graphs) lal::iterators::Q_iterator<lal::graphs::undirected_graph>;
%template(__Q_iterator_rooted_tree) lal::iterators::Q_iterator<lal::graphs::rooted_tree>;
%template(__Q_iterator_free_tree) lal::iterators::Q_iterator<lal::graphs::free_tree>;

%pythoncode %{
def __type_of_graph(g):
	r"""
	This method returns the type of the input graph `g`.
	
	This function is for internal usage only. Do not use.
	"""
	return str(type(g))[len("<class 'lalgraphs."):-2]

def E_iterator(g):
	r"""
	Returns an iterator-like object that iterates over the list of edges
	of the input graph `g`.
	
	Parameters
	----------
	* `g` :
		Input graph.
	"""
	__type_graph = __type_of_graph(g)
	return globals()[ "__E_iterator_" + __type_graph ](g)

def Q_iterator(g):
	r"""
	Returns an iterator-like object that iterates over the list of pairs
	of independent edges the input graph `g`.
	
	Parameters
	----------
	* `g` :
		Input graph.
	"""
	__type_graph = __type_of_graph(g)
	return globals()[ "__Q_iterator_" + __type_graph ](g)
%}
