%module laldebug

%import submodules/graphsdebug.i
%import submodules/numericdebug.i
%import submodules/generationdebug.i
%import submodules/iteratorsdebug.i
%import submodules/iodebug.i
%import submodules/linarrdebug.i
%import submodules/propertiesdebug.i

%pythoncode %{
graphs = graphsdebug
del graphsdebug

numeric = numericdebug
del numericdebug

generation = generationdebug
del generationdebug

iterators = iteratorsdebug
del iteratorsdebug

io = iodebug
del iodebug

linarr = linarrdebug
del linarrdebug

properties = propertiesdebug
del propertiesdebug
%}
