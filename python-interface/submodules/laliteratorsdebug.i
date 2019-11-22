%module laliteratorsdebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i

%include laliterators_impl.i

%pythoncode %{
graphs = lalgraphsdebug
del lalgraphsdebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
