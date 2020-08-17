%module lalutilities

%import laldefinitions.i
%import lalgraphs.i

%include lalutilities_impl.i

%pythoncode %{
graphs = lalgraphs
del lalgraphs

__definitions = laldefinitions
del laldefinitions
%}
