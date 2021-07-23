%module algorithms_Dmin_planar

%{
#include <lal/linarr/algorithms_Dmin_planar.hpp>
%}
%include "../lal/linarr/algorithms_Dmin_planar.hpp"

%pythoncode %{
__mod_name = "algorithms_Dmin_planar"
__to_replace = "algorithms_Dmin_planar_"
%}

%include rename_enum_contents.i
