%module lalpropertiesdebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i
%import lalnumericdebug.i

%include lalproperties_impl.i

%pythoncode %{
__definitions = laldefinitionsdebug
del laldefinitionsdebug

# remove unnecessary modules
del lalgraphsdebug
del lalnumericdebug
%}
