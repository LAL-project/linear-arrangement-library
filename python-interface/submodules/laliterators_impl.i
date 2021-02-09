
%{

// C++ includes
#include <lal/graphs.hpp>
#include <lal/iterators.hpp>

%}

%include "../lal/iterators/E_iterator.hpp"
%include "../lal/iterators/Q_iterator.hpp"

%template(E_iterator_directed_graphs) lal::iterators::E_iterator<lal::graphs::directed_graph>;
%template(E_iterator_undirected_graphs) lal::iterators::E_iterator<lal::graphs::undirected_graph>;

%template(Q_iterator_directed_graphs) lal::iterators::Q_iterator<lal::graphs::directed_graph>;
%template(Q_iterator_undirected_graphs) lal::iterators::Q_iterator<lal::graphs::undirected_graph>;
