%module lalgraphs

%import laldefinitions.i
%import enums/laltree_type.i
%include lalgraphs_impl.i

%pythoncode %{
__definitions = laldefinitions
del laldefinitions
%}
