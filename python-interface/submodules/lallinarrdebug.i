%module lallinarrdebug

%import laldefinitionsdebug.i
%import enums/laltree_structuredebug.i
%import enums/lalalgorithms_Cdebug.i
%import enums/lalalgorithms_Dmindebug.i
%import lalgraphsdebug.i
%import lalnumericdebug.i

%include lallinarr_impl.i

%pythoncode %{
tree_structure = laltree_structuredebug
del laltree_structuredebug

algorithms_C = lalalgorithms_Cdebug
del lalalgorithms_Cdebug

algorithms_Dmin = lalalgorithms_Dmindebug
del lalalgorithms_Dmindebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug

# remove unnecessary modules
del lalgraphsdebug
del lalnumericdebug
%}
