%module syntactic_dependency_structure_type

%{
#include <lal/linarr/syntactic_dependency_structure_type.hpp>
%}
%include "../lal/linarr/syntactic_dependency_structure_type.hpp"

%pythoncode %{
__mod_name = "syntactic_dependency_structure_type"
__to_replace = "syntactic_dependency_structure_type_"
%}

%include rename_enum_contents.i
