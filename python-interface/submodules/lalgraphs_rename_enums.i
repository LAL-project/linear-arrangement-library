
%pythoncode %{

# In this piece of code we modify class "drtree".
#
# In this piece of code we create one subclass for this class,
# for the only enumeration it has: "drtree_type".
# As created by SWIG, class "drtree" has attributes of the
# form:
# 		drtree.drtree_type_arborescence
# 		drtree.drtree_type_anti_arborescence
# 		drtree.drtree_type_none
#
# Our aim in this piece of code is to make a subclass so that the
# values of the enumeration "drtree_type" are members of a class, namely,
# we want class "drtree" to be usable in this way:
# 		drtree.drtree_type.arborescence
# 		drtree.drtree_type.anti_arborescence
# 		drtree.drtree_type.none
#


# -------------------
# Make the subclasses

class drtree_type:
	__dummy_attr = 0

# --------------------------------------------
# add the enumaration values to the subclasses
DRtree = graphs.drtree

# dirrottree_type
DRtree_enum_vals = list(filter(lambda k: k.find("drtree_type_") == 0, DRtree.__dict__.keys()))
for enval_name in DRtree_enum_vals:
	# make new enumeration value name
	new_enval_name = enval_name[len("drtree_type_"):]
	# set attribute to class "drtree_type"
	setattr(drtree_type, new_enval_name, getattr(DRtree, enval_name))
	# delete enumeration value from "drtree"
	delattr(DRtree, enval_name)

# -------------------------------------------
# clean up dummy attributes of the subclasses
delattr(drtree_type, "_drtree_type__dummy_attr")

# ------------------------------------------------
# make the subclasses attributes of class "drtree"

# set the subclasses
setattr(DRtree, "drtree_type", drtree_type)

# ------------------------
# clean up local variables
del DRtree
del DRtree_enum_vals
del enval_name
del new_enval_name

# even the classes!
del drtree_type

# and the 'graphs' module
del graphs
%}
