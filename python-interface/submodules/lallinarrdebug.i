%module lallinarrdebug

%import laldefinitionsdebug.i
%import enums/laltree_structuredebug.i
%import enums/lalalgorithms_crossingsdebug.i
%import enums/lalalgorithms_Dmindebug.i
%import lalgraphsdebug.i
%import lalnumericdebug.i

%include lallinarr_impl.i

%pythoncode %{
tree_structure = laltree_structuredebug
del laltree_structuredebug

algorithms_crossings = lalalgorithms_crossingsdebug
del lalalgorithms_crossingsdebug

algorithms_Dmin = lalalgorithms_Dmindebug
del lalalgorithms_Dmindebug

graphs = lalgraphsdebug
del lalgraphsdebug

numeric = lalnumericdebug
del lalnumericdebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug
%}
