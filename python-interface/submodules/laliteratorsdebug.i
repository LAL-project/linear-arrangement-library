%module laliteratorsdebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i

%include laliterators_impl.i

%pythoncode %{
__definitions = laldefinitionsdebug
del laldefinitionsdebug

# remove unnecessary modules
del lalgraphsdebug
del laltree_typedebug
%}
