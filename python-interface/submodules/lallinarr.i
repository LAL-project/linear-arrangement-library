%module lallinarr

%import laldefinitions.i
%import enums/laltree_structure.i
%import enums/lalalgorithms_crossings.i
%import enums/lalalgorithms_Dmin.i
%import lalgraphs.i
%import lalnumeric.i

%include lallinarr_impl.i

%pythoncode %{
tree_structure = laltree_structure
del laltree_structure

algorithms_crossings = lalalgorithms_crossings
del lalalgorithms_crossings

algorithms_Dmin = lalalgorithms_Dmin
del lalalgorithms_Dmin

graphs = lalgraphs
del lalgraphs

numeric = lalnumeric
del lalnumeric

__definitions = laldefinitions
del laldefinitions
%}
