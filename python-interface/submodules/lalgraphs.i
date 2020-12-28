%module lalgraphs

%import laldefinitions.i
%import enums/laltree_type.i
%include lalgraphs_impl.i

%pythoncode %{
tree_type = laltree_type
del laltree_type

__definitions = laldefinitions
del laldefinitions
%}
