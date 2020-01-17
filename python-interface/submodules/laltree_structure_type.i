%module laltree_structure_type

%include laltree_structure_type_impl.i

%pythoncode %{
__mod_name = "laltree_structure_type"
__mod = __import__(__mod_name)
__to_replace = "tree_structure_type_"
__replace_length = len(__to_replace)
for name in dir(__mod):
    if name.find(__to_replace) == 0:
        setattr(__mod, name[__replace_length:], getattr(__mod, name))
        delattr(__mod, name)
del name
del __mod, __mod_name
del __to_replace, __replace_length
%}