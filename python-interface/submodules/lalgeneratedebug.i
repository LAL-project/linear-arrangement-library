%module lalgeneratedebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i

%include lalgenerate_impl.i

%pythoncode %{
graphs = lalgraphsdebug
del lalgraphsdebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
