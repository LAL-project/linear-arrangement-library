%module treebank_error_type

%{
#include <lal/io/treebank_error_type.hpp>
%}
%include "../lal/io/treebank_error_type.hpp"

%pythoncode %{
__mod_name = "treebank_error_type"
__to_replace = "treebank_error_type_"
%}

%include rename_enum_contents.i
