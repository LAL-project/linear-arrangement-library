%module lalproperties

%import laldefinitions.i
%import lalgraphs.i
%import lalnumeric.i

%include lalproperties_impl.i

%pythoncode %{
graphs = lalgraphs
del lalgraphs

numeric = lalnumeric
del lalnumeric

__definitions = laldefinitions
del laldefinitions
%}
