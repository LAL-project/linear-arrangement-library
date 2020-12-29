%module lalsyntactic_dependency_structure_typedebug

%include lalsyntactic_dependency_structure_type_impl.i

%pythoncode %{
__mod_name = "lalsyntactic_dependency_structure_typedebug"
__to_replace = "syntactic_dependency_structure_type_"
%}

%include rename_enum_contents.i
