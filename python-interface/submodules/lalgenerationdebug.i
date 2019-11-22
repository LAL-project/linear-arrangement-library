%module lalgenerationdebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i

%include lalgeneration_impl.i

%pythoncode %{
graphs = lalgraphsdebug
del lalgraphsdebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
