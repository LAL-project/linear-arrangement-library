%module algorithms_Dmin

%{
#include <lal/linarr/algorithms_Dmin.hpp>
%}
%include "../lal/linarr/algorithms_Dmin.hpp"

%pythoncode %{
__mod_name = "algorithms_Dmin"
__to_replace = "algorithms_Dmin_"
%}

%include rename_enum_contents.i
