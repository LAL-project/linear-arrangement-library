%module lalio

%import laldefinitions.i
%import lalgraphs.i
%import enums/laldataset_error.i

%include lalio_impl.i

%pythoncode %{
dataset_error = laldataset_error
del laldataset_error

__definitions = laldefinitions
del laldefinitions

# remove unnecessary modules
del lalgraphs
del laltree_type

# import oneself as io for later renaming
# of functions in "lalio_rename_enums.i"
import lalio as io
%}

%include lalio_rename_enums.i
