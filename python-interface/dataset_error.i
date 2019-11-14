%module dataset_error

%{
#include <lal/io/dataset_error.hpp>
%}
%include "../lal/io/dataset_error.hpp"

%pythoncode %{
__enum_name = "dataset_error"      # your enumeration's name
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
