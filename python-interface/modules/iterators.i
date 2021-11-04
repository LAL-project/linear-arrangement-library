%module iterators

%import types.i
%import graphs.i

%include documentation.i

%{
#include <lal/graphs.hpp>
#include <lal/iterators.hpp>
%}

%include "../lal/iterators/E_iterator.hpp"

namespace lal {
namespace iterators {

%template(E_iterator_directed_graphs) E_iterator<lal::graphs::directed_graph>;
%template(E_iterator_undirected_graphs) E_iterator<lal::graphs::undirected_graph>;
%template(E_iterator_rooted_tree) E_iterator<lal::graphs::rooted_tree>;
%template(E_iterator_free_tree) E_iterator<lal::graphs::free_tree>;

} // -- namespace iterators
} // -- namespace lal

%include "../lal/iterators/Q_iterator.hpp"

namespace lal {
namespace iterators {

%template(Q_iterator_directed_graphs) Q_iterator<graphs::directed_graph>;
%template(Q_iterator_undirected_graphs) Q_iterator<graphs::undirected_graph>;
%template(Q_iterator_rooted_tree) Q_iterator<graphs::rooted_tree>;
%template(Q_iterator_free_tree) Q_iterator<graphs::free_tree>;

} // -- namespace iterators
} // -- namespace lal

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
__types = types
del types

# remove unnecessary modules
del graphs
del tree_type
%}
