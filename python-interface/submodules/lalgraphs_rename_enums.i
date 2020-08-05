
%pythoncode %{

# In this piece of code we modify class "rooted_tree".
#
# In this piece of code we create one subclass for this class,
# for the only enumeration it has: "rooted_tree_type".
# As created by SWIG, class "rooted_tree" has attributes of the
# form:
# 		rooted_tree.rooted_tree_type_arborescence
# 		rooted_tree.rooted_tree_type_anti_arborescence
# 		rooted_tree.rooted_tree_type_none
#
# Our aim in this piece of code is to make a subclass so that the
# values of the enumeration "rooted_tree_type" are members of a class, namely,
# we want class "rooted_tree" to be usable in this way:
# 		rooted_tree.rooted_tree_type.arborescence
# 		rooted_tree.rooted_tree_type.anti_arborescence
# 		rooted_tree.rooted_tree_type.none
#


# -------------------
# Make the subclasses

class rooted_tree_type:
	__dummy_attr = 0

# --------------------------------------------
# add the enumaration values to the subclasses
rooted_tree = graphs.rooted_tree

# dirrottree_type
rooted_tree_enum_vals = list(filter(lambda k: k.find("rooted_tree_type_") == 0, rooted_tree.__dict__.keys()))
for enval_name in rooted_tree_enum_vals:
	# make new enumeration value name
	new_enval_name = enval_name[len("rooted_tree_type_"):]
	# set attribute to class "rooted_tree_type"
	setattr(rooted_tree_type, new_enval_name, getattr(rooted_tree, enval_name))
	# delete enumeration value from "rooted_tree"
	delattr(rooted_tree, enval_name)

# -------------------------------------------
# clean up dummy attributes of the subclasses
delattr(rooted_tree_type, "_rooted_tree_type__dummy_attr")

# ------------------------------------------------
# make the subclasses attributes of class "rooted_tree"

# set the subclasses
setattr(rooted_tree, "rooted_tree_type", rooted_tree_type)

# ------------------------
# clean up local variables
del rooted_tree
del rooted_tree_enum_vals
del enval_name
del new_enval_name

# even the classes!
del rooted_tree_type

# and the 'graphs' module
del graphs
%}
