%module lallinarrdebug

%import laldefinitionsdebug.i // this is necessary for the wrapper
%import enums/lalsyntactic_dependency_structure_typedebug.i
%import enums/lalalgorithms_Cdebug.i
%import enums/lalalgorithms_Dmindebug.i
// these are necessary for the wrapper
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

# remove unnecessary modules (not required for python)
del lalgraphsdebug
del lalnumericdebug
del laltree_typedebug # this comes from lalgraphs


stolen = getattr(__definitions, "dependency_flux")
delattr(__definitions, "dependency_flux")

setattr(__import__("lallinarrdebug"), "dependency_flux", stolen)
del stolen
%}
