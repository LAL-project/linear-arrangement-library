%module lallinarr

%import laldefinitions.i
%import enums/laltree_structure.i
%import enums/lalalgorithms_C.i
%import enums/lalalgorithms_Dmin.i
%import lalgraphs.i
%import lalnumeric.i

%include lallinarr_impl.i

%pythoncode %{
tree_structure = laltree_structure
del laltree_structure

algorithms_C = lalalgorithms_C
del lalalgorithms_C

algorithms_Dmin = lalalgorithms_Dmin
del lalalgorithms_Dmin

__definitions = laldefinitions
del laldefinitions

# remove unnecessary modules
del lalgraphs
del lalnumeric
%}
