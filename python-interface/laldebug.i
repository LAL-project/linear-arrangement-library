%module laldebug

%import submodules/laldefinitionsdebug.i
%import submodules/lalgraphsdebug.i
%import submodules/lalnumericdebug.i
%import submodules/lalgenerationdebug.i
%import submodules/laliteratorsdebug.i
%import submodules/laliodebug.i
%import submodules/lallinarrdebug.i
%import submodules/lalpropertiesdebug.i

%pythoncode %{
__mod_name = "laldebug"
__mod = __import__(__mod_name)
for name in dir(__mod):
	start_lal = name.find("lal")
	start_debug = name.find("debug")
	if start_lal == 0 and start_debug > 0:
		new_name = name[(start_lal + 3):start_debug]
		setattr(__mod, new_name, getattr(__mod, name))
		delattr(__mod, name)
	
del start_lal, start_debug, new_name
del __mod_name, __mod

__definitions = definitions
del definitions
%}
