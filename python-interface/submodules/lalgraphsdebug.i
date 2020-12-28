%module lalgraphsdebug

%import laldefinitionsdebug.i
%import enums/laltree_typedebug.i
%include lalgraphs_impl.i

%pythoncode %{
__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
