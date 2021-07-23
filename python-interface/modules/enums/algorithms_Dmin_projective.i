%module algorithms_Dmin_projective

%{
#include <lal/linarr/algorithms_Dmin_projective.hpp>
%}
%include "../lal/linarr/algorithms_Dmin_projective.hpp"

%pythoncode %{
__mod_name = "algorithms_Dmin_projective"
__to_replace = "algorithms_Dmin_projective_"
%}

%include rename_enum_contents.i
