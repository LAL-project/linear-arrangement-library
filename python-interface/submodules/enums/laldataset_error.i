%module laldataset_error

%include laldataset_error_impl.i

%pythoncode %{
__mod_name = "laldataset_error"
__to_replace = "dataset_error_"
%}

%include rename_enum_contents.i
