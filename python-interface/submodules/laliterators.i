%module laliterators

%import laldefinitions.i
%import lalgraphs.i

%include laliterators_impl.i

%pythoncode %{
graphs = lalgraphs
del lalgraphs

__definitions = laldefinitions
del laldefinitions
%}
