%module iodebug

%import graphsdebug.i
%import dataset_errordebug.i

%include io_impl.i

%pythoncode %{
graphs = graphsdebug
del graphsdebug

dataset_error = dataset_errordebug
del dataset_errordebug
%}
