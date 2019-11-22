%module lalgeneration

%import laldefinitions.i
%import lalgraphs.i

%include lalgeneration_impl.i

%pythoncode %{
graphs = lalgraphs
del lalgraphs

__definitions = laldefinitions
del laldefinitions
%}
