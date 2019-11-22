%module lallinarrdebug

%import laldefinitionsdebug.i
%import laltree_structure_typedebug.i
%import lalalgorithms_crossingsdebug.i
%import lalgraphsdebug.i
%import lalnumericdebug.i

%include lallinarr_impl.i

%pythoncode %{
tree_structure_type = laltree_structure_typedebug
del laltree_structure_typedebug

algorithms_crossings = lalalgorithms_crossingsdebug
del lalalgorithms_crossingsdebug

graphs = lalgraphsdebug
del lalgraphsdebug

numeric = lalnumericdebug
del lalnumericdebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
