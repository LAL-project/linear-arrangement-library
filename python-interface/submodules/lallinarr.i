%module lallinarr

%include std_vector.i
%include std_array.i

%import laldefinitions.i // this is necessary for the wrapper
%import enums/lalsyntactic_dependency_structure_type.i
%import enums/lalalgorithms_C.i
%import enums/lalalgorithms_Dmin.i
// these are necessary for the wrapper
%import lalgraphs.i
%import lalnumeric.i

%include lallinarr_impl.i

%pythoncode %{
syntactic_dependency_structure_type = lalsyntactic_dependency_structure_type
del lalsyntactic_dependency_structure_type

algorithms_C = lalalgorithms_C
del lalalgorithms_C

algorithms_Dmin = lalalgorithms_Dmin
del lalalgorithms_Dmin

__definitions = laldefinitions
del laldefinitions

# remove unnecessary modules (not required for python)
del lalgraphs
del lalnumeric
del laltree_type # this comes from lalgraphs
%}
