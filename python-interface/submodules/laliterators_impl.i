
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
def __type_of_graph(g):
	return str(type(g))[len("<class 'lalgraphs."):-2]

def E_iterator(g):
	__type_graph = __type_of_graph(g)
	return globals()[ "E_iterator_" + __type_graph ](g)

def Q_iterator(g):
	__type_graph = __type_of_graph(g)
	return globals()[ "Q_iterator_" + __type_graph ](g)
%}
