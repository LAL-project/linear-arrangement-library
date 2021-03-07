%module tree_type

%include std_string.i

%{
#include <lal/graphs/tree_type.hpp>
%}
%include "../lal/graphs/tree_type.hpp"

%pythoncode %{
__mod_name = "tree_type"
__to_replace = "tree_type_"
%}

%include rename_enum_contents.i
