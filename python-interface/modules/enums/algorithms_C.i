%module algorithms_C

%{
#include <lal/linarr/algorithms_C.hpp>
%}
%include "../lal/linarr/algorithms_C.hpp"

%pythoncode %{
__mod_name = "algorithms_C"
__to_replace = "algorithms_C_"
%}

%include rename_enum_contents.i
