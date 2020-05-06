
%pythoncode %{

# In this piece of code we modify class "rtree".
#
# In this piece of code we create one subclass for this class,
# for the only enumeration it has: "rtree_type".
# As created by SWIG, class "rtree" has attributes of the
# form:
# 		rtree.rtree_type_arborescence
# 		rtree.rtree_type_anti_arborescence
# 		rtree.rtree_type_none
#
# Our aim in this piece of code is to make a subclass so that the
# values of the enumeration "rtree_type" are members of a class, namely,
# we want class "rtree" to be usable in this way:
# 		rtree.rtree_type.arborescence
# 		rtree.rtree_type.anti_arborescence
# 		rtree.rtree_type.none
#


# -------------------
# Make the subclasses

class rtree_type:
	__dummy_attr = 0

# --------------------------------------------
# add the enumaration values to the subclasses
Rtree = graphs.rtree

# dirrottree_type
Rtree_enum_vals = list(filter(lambda k: k.find("rtree_type_") == 0, Rtree.__dict__.keys()))
for enval_name in Rtree_enum_vals:
	# make new enumeration value name
	new_enval_name = enval_name[len("rtree_type_"):]
	# set attribute to class "rtree_type"
	setattr(rtree_type, new_enval_name, getattr(Rtree, enval_name))
	# delete enumeration value from "rtree"
	delattr(Rtree, enval_name)

# -------------------------------------------
# clean up dummy attributes of the subclasses
delattr(rtree_type, "_rtree_type__dummy_attr")

# ------------------------------------------------
# make the subclasses attributes of class "rtree"

# set the subclasses
setattr(Rtree, "rtree_type", rtree_type)

# ------------------------
# clean up local variables
del Rtree
del Rtree_enum_vals
del enval_name
del new_enval_name

# even the classes!
del rtree_type

# and the 'graphs' module
del graphs
%}
