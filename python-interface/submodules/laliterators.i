%module laliterators

%import laldefinitions.i
%import lalgraphs.i

%include laliterators_impl.i

%pythoncode %{
__definitions = laldefinitions
del laldefinitions

# remove unnecessary modules
del lalgraphs
%}
