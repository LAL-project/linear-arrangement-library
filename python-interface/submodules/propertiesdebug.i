%module propertiesdebug

%import graphsdebug.i
%import numericdebug.i

%include properties_impl.i

%pythoncode %{
graphs = graphsdebug
del graphsdebug

numeric = numericdebug
del numericdebug
%}
