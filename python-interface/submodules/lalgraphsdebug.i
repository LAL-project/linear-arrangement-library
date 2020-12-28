%module lalgraphsdebug

%import laldefinitionsdebug.i
%import enums/laltree_typedebug.i
%include lalgraphs_impl.i

%pythoncode %{
tree_type = laltree_typedebug
del laltree_typedebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
