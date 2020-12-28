%module laltree_typedebug

%include std_string.i
%include laltree_type_impl.i

%pythoncode %{
__mod_name = "laltree_typedebug"
__to_replace = "tree_type_"
%}

%include rename_enum_contents.i

