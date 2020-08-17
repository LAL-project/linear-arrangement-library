%module lal

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

%import submodules/lalversion.i
%import submodules/laldefinitions.i
%import submodules/lalgraphs.i
%import submodules/lalnumeric.i
%import submodules/lalgenerate.i
%import submodules/laliterators.i
%import submodules/lalio.i
%import submodules/lallinarr.i
%import submodules/lalproperties.i
%import submodules/lalutilities.i

%pythoncode %{
# remove 'lal' from the module's names.

__mod_name = "lal"
__mod = __import__(__mod_name)
for name in dir(__mod):
	start_lal = name.find("lal")
	if start_lal == 0:
		new_name = name[(start_lal + 3):]
		setattr(__mod, new_name, getattr(__mod, name))
		delattr(__mod, name)
	
del start_lal, new_name, name
del __mod_name, __mod

__definitions = definitions
del definitions
%}
