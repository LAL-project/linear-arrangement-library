%module lallinarr

%import laldefinitions.i
%import laltree_structure_type.i
%import lalalgorithms_crossings.i
%import lalalgorithms_Dmin.i
%import lalgraphs.i
%import lalnumeric.i

%include lallinarr_impl.i

%pythoncode %{
tree_structure_type = laltree_structure_type
del laltree_structure_type

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
