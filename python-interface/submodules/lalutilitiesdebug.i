%module lalutilitiesdebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i

%include lalutilities_impl.i

%pythoncode %{
graphs = lalgraphsdebug
del lalgraphsdebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
