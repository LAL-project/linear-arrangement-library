%module version

%include documentation.i

%{
#include <lal/lal_version.hpp>
%}

%include std_string.i

%include "../lal/lal_version.hpp"

%pythoncode %{
cvar = _version.cvar

major = cvar.__lal_major_verno
patch = cvar.__lal_patch_verno
%}
