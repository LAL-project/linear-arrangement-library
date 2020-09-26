%module lalgraphsdebug

%import laldefinitionsdebug.i
%include lalgraphs_impl.i

%pythoncode %{
__definitions = laldefinitionsdebug
del laldefinitionsdebug

# import oneself as io for later renaming
# of functions in "lalgraphs_rename_enums.i"
import lalgraphsdebug as graphs
%}

%include lalgraphs_rename_enums.i
