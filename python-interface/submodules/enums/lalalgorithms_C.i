%module lalalgorithms_C

%include lalalgorithms_C_impl.i

%pythoncode %{
__mod_name = "lalalgorithms_C"
__to_replace = "algorithms_C_"
%}

%include rename_enum_contents.i
