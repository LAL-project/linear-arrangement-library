%module lalgraphs

%import laldefinitions.i

%include lalgraphs_impl.i

%pythoncode %{
__definitions = laldefinitions
del laldefinitions
%}
