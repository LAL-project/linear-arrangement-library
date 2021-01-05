%module lalutilities

%import laldefinitions.i
%import lalgraphs.i

%include lalutilities_impl.i

%pythoncode %{
__definitions = laldefinitions
del laldefinitions

# remove unnecessary modules
del lalgraphs
del laltree_type
%}
