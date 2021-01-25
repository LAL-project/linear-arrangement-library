%module laldebug

%{
#include <lal/definitions.hpp>
#include <lal/graphs.hpp>
#include <lal/generate.hpp>
#include <lal/graphs.hpp>
#include <lal/io.hpp>
#include <lal/iterators.hpp>
#include <lal/linarr.hpp>
#include <lal/numeric.hpp>
#include <lal/properties.hpp>
#include <lal/utilities.hpp>
%}

%import submodules/lalversiondebug.i
%import submodules/laldefinitionsdebug.i
%import submodules/lalgraphsdebug.i
%import submodules/lalnumericdebug.i
%import submodules/lalgeneratedebug.i
%import submodules/laliteratorsdebug.i
%import submodules/laliodebug.i
%import submodules/lallinarrdebug.i
%import submodules/lalpropertiesdebug.i
%import submodules/lalutilitiesdebug.i

%pythoncode %{
# remove 'lal' and 'debug' from the module's names.

__mod_name = "laldebug"
__mod = __import__(__mod_name)
for name in dir(__mod):
	start_lal = name.find("lal")
	start_debug = name.find("debug")
	if start_lal == 0 and start_debug > 0:
		new_name = name[(start_lal + 3):start_debug]
		setattr(__mod, new_name, getattr(__mod, name))
		delattr(__mod, name)

# remove unwanted modules
del algorithms_C
del algorithms_Dmin
del dataset_error
del syntactic_dependency_structure_type
del tree_type

# remove variables
del start_lal, start_debug, new_name, name
del __mod_name, __mod

# hide definitions
__definitions = definitions
del definitions
%}
