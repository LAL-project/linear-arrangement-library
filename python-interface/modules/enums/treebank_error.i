%module treebank_error

%{
#include <lal/io/treebank_error.hpp>
%}
%include "../lal/io/treebank_error.hpp"

%pythoncode %{
__mod_name = "treebank_error"
__to_replace = "treebank_error_"
%}

%include rename_enum_contents.i
