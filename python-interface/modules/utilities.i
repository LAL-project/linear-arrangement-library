%module utilities

%import types.i
%import graphs.i

%include documentation.i

%{
#include <lal/graphs.hpp>
#include <lal/utilities.hpp>
%}

%include "../lal/utilities/tree_isomorphism.hpp"

%pythoncode %{
__types = types
del types

# remove unnecessary modules
del graphs
del tree_type
%}
