%module generationdebug

%import graphsdebug.i
%include generation_impl.i

%pythoncode %{
graphs = graphsdebug
del graphs
%}
