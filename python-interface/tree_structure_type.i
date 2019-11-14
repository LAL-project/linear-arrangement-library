%module tree_structure_type

%{
#include <lal/linarr/tree_structure_type.hpp>
%}
%include "../lal/linarr/tree_structure_type.hpp"

%pythoncode %{
__enum_name = "tree_structure_type"# your enumeration's name
__enum = __import__(__enum_name)   # import module
__scope_name = __enum_name + "_"   # scope's name (= "colours_")
__scope_length = len(__scope_name) # length
for name in dir(__enum):
    if name.find(__scope_name) == 0:
        setattr(__enum, name[__scope_length:], getattr(__enum, name))
        delattr(__enum, name) # optional
del name
del __enum
%}
