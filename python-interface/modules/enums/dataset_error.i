%module dataset_error

%{
#include <lal/io/dataset_error.hpp>
%}
%include "../lal/io/dataset_error.hpp"

%pythoncode %{
__mod_name = "dataset_error"
__to_replace = "dataset_error_"
%}

%include rename_enum_contents.i
