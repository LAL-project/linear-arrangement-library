%module lalnumericdebug

%import laldefinitionsdebug.i

%include lalnumeric_impl.i

%pythoncode %{
__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
