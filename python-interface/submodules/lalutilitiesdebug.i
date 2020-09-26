%module lalutilitiesdebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i

%include lalutilities_impl.i

%pythoncode %{
__definitions = laldefinitionsdebug
del laldefinitionsdebug

# remove unnecessary modules
del lalgraphsdebug
%}
