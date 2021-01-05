%module lallinarrdebug

%import laldefinitionsdebug.i
%import enums/lalsyntactic_dependency_structure_typedebug.i
%import enums/lalalgorithms_Cdebug.i
%import enums/lalalgorithms_Dmindebug.i
%import lalgraphsdebug.i
%import lalnumericdebug.i

%include lallinarr_impl.i

%pythoncode %{
syntactic_dependency_structure_type = lalsyntactic_dependency_structure_typedebug
del lalsyntactic_dependency_structure_typedebug

algorithms_C = lalalgorithms_Cdebug
del lalalgorithms_Cdebug

algorithms_Dmin = lalalgorithms_Dmindebug
del lalalgorithms_Dmindebug

__definitions = laldefinitionsdebug
del laldefinitionsdebug

# remove unnecessary modules
del lalgraphsdebug
del lalnumericdebug
%}
