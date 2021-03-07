%module utilities

%import definitions.i
%import graphs.i

%include documentation.i

%{

#include <lal/graphs.hpp>
#include <lal/utilities.hpp>

%}

%include "../lal/utilities/tree_isomorphism.hpp"

%pythoncode %{
__definitions = definitions
del definitions

# remove unnecessary modules
del graphs
del tree_type
%}
