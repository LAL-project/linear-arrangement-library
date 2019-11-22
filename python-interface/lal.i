%module lal

%import submodules/laldefinitions.i
%import submodules/lalgraphs.i
%import submodules/lalnumeric.i
%import submodules/lalgeneration.i
%import submodules/laliterators.i
%import submodules/lalio.i
%import submodules/lallinarr.i
%import submodules/lalproperties.i

%pythoncode %{
__mod_name = "lal"
__mod = __import__(__mod_name)
for name in dir(__mod):
	start_lal = name.find("lal")
	if start_lal == 0:
		new_name = name[(start_lal + 3):]
		setattr(__mod, new_name, getattr(__mod, name))
		delattr(__mod, name)
	
del start_lal, new_name
del __mod_name, __mod

__definitions = definitions
del definitions
%}
