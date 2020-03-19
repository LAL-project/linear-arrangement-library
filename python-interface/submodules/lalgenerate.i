%module lalgenerate

%import laldefinitions.i
%import lalgraphs.i

%include lalgenerate_impl.i

%pythoncode %{
graphs = lalgraphs
del lalgraphs

__definitions = laldefinitions
del laldefinitions
%}
