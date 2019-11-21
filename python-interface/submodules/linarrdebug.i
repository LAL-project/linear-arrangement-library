%module linarrdebug

%import tree_structure_typedebug.i
%import algorithms_crossingsdebug.i
%import graphsdebug.i
%import numericdebug.i

%include linarr_impl.i

%pythoncode %{
tree_structure_type = tree_structure_typedebug
del tree_structure_typedebug

algorithms_crossings = algorithms_crossingsdebug
del algorithms_crossingsdebug

graphs = graphsdebug
del graphsdebug

numeric = numericdebug
del numericdebug
%}
