%module lallinarr

%import laldefinitions.i
%import enums/lalsyntactic_dependency_structure_type.i
%import enums/lalalgorithms_C.i
%import enums/lalalgorithms_Dmin.i
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

# remove unnecessary modules
del lalgraphs
del lalnumeric
%}
