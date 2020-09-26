%module laltree_structure

%include laltree_structure_impl.i

%pythoncode %{
__mod_name = "laltree_structure"
__to_replace = "tree_structure_"
%}

%include rename_enum_contents.i
