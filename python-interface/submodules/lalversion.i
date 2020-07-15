%module lalversion

%include lalversion_impl.i

%pythoncode %{
cvar = _lalversion.cvar

major = cvar.__lal_major_verno
minor = cvar.__lal_minor_verno
patch = cvar.__lal_patch_verno
%}
