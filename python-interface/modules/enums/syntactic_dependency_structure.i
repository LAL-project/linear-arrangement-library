%module syntactic_dependency_structure

%{
#include <lal/linarr/syntactic_dependency_structure.hpp>
%}
%include "../lal/linarr/syntactic_dependency_structure.hpp"

%pythoncode %{
__mod_name = "syntactic_dependency_structure"
__to_replace = "syntactic_dependency_structure_"
%}

%include rename_enum_contents.i
