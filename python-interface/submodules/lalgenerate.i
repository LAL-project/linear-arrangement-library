%module lalgenerate

%import laldefinitions.i
%import lalgraphs.i

%include lalgenerate_impl.i

%pythoncode %{
__definitions = laldefinitions
del laldefinitions

# remove unnecessary modules
del lalgraphs
del laltree_type
%}
