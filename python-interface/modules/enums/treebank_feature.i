%module treebank_feature

%{
#include <lal/io/treebank_feature.hpp>
%}
%include "../lal/io/treebank_feature.hpp"

%pythoncode %{
__mod_name = "treebank_feature"
__to_replace = "treebank_feature_"
%}

%include rename_enum_contents.i
