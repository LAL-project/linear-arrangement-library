%module lalalgorithms_Dmindebug

%include lalalgorithms_Dmin_impl.i

%pythoncode %{
__mod_name = "lalalgorithms_Dmindebug"
__to_replace = "algorithms_Dmin_"
%}

%include rename_enum_contents.i
