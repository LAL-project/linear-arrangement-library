%module laldataset_errordebug

%include laldataset_error_impl.i

%pythoncode %{
__mod_name = "laldataset_errordebug"
__to_replace = "dataset_error_"
%}

%include rename_enum_contents.i
