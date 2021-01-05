%module lalproperties

%import laldefinitions.i
%import lalgraphs.i
%import lalnumeric.i

%include lalproperties_impl.i

%pythoncode %{
__definitions = laldefinitions
del laldefinitions

# remove unnecessary modules
del lalgraphs
del lalnumeric
del laltree_type
%}
