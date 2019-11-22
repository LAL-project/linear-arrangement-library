%module lalnumeric

%import laldefinitions.i

%include lalnumeric_impl.i

%pythoncode %{
__definitions = laldefinitions
del laldefinitions
%}
