%module lalgraphsdebug

%import laldefinitionsdebug.i

%include lalgraphs_impl.i

%pythoncode %{
__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
