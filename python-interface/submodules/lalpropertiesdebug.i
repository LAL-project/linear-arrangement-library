%module lalpropertiesdebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i
%import lalnumericdebug.i

%include lalproperties_impl.i

%pythoncode %{
graphs = lalgraphsdebug
del lalgraphsdebug

numeric = lalnumericdebug
del lalnumericdebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
