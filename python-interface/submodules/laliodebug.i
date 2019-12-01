%module laliodebug

%import laldefinitionsdebug.i
%import lalgraphsdebug.i
%import laldataset_errordebug.i

%include lalio_impl.i

%pythoncode %{
graphs = lalgraphsdebug
del lalgraphsdebug

dataset_error = laldataset_errordebug
del laldataset_errordebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug

# import oneself as io for later renaming
# of functions in "lalio_rename_enums.i"
import laliodebug as io
%}

%include lalio_rename_enums.i
