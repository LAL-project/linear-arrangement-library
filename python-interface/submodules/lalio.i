%module lalio

%import laldefinitions.i
%import lalgraphs.i
%import enums/laldataset_error.i

%include lalio_impl.i

%pythoncode %{
graphs = lalgraphs
del lalgraphs

dataset_error = laldataset_error
del laldataset_error

__definitions = laldefinitions
del laldefinitions

# import oneself as io for later renaming
# of functions in "lalio_rename_enums.i"
import lalio as io
%}

%include lalio_rename_enums.i
