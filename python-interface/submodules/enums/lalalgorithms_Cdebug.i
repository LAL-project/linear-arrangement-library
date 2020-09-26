%module lalalgorithms_Cdebug

%include lalalgorithms_C_impl.i

%pythoncode %{
__mod_name = "lalalgorithms_Cdebug"
__to_replace = "algorithms_C_"
%}

%include rename_enum_contents.i

