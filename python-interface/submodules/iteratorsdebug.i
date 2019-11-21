%module iteratorsdebug

%import graphsdebug.i

%include iterators_impl.i

%pythoncode %{
graphs = graphsdebug
del graphsdebug
%}
