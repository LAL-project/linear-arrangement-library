%module lalsyntactic_dependency_structure_type

%include lalsyntactic_dependency_structure_type_impl.i

%pythoncode %{
__mod_name = "lalsyntactic_dependency_structure_type"
__to_replace = "syntactic_dependency_structure_type_"
%}

%include rename_enum_contents.i
