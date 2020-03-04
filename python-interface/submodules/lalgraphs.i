%module lalgraphs

%import laldefinitions.i

%include lalgraphs_impl.i

%pythoncode %{
__definitions = laldefinitions
del laldefinitions

# import oneself as io for later renaming
# of functions in "lalgraphs_rename_enums.i"
import lalgraphs as graphs
%}

%include lalgraphs_rename_enums.i
