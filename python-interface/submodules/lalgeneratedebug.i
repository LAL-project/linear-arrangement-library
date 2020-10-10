%module lalgeneratedebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i

%include lalgenerate_impl.i

%pythoncode %{
__definitions = laldefinitionsdebug
del laldefinitionsdebug

# remove unnecessary modules
del lalgraphsdebug
%}
