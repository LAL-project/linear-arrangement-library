
%pythoncode %{

# In this piece of code we modify class "treebank_dataset_processor".
# Remember: "treebank_dataset_processor" is the automatic processing
# of a dataset.
#
# In this piece of code we create two subclasses for this class,
# one for each enumeration it has: "tree_feature" and "processor_error".
# As created by SWIG, class "treebank_dataset_processor" has attributes of the
# form:
# 		treebank_dataset_processor.processor_error_none
# 		treebank_dataset_processor.processor_error_missing_parent
# 		treebank_dataset_processor.processor_error_missing_main
# 		....
# and also
# 		treebank_dataset_processor.tree_feature_n
# 		treebank_dataset_processor.tree_feature_k2
# 		treebank_dataset_processor.tree_feature_k3
# 		....
# Our aim in this piece of code is to make two subclasses so that the
# values of the enumerations "tree_feature" and "processor_error"
# are members of classes, namely, we want class "treebank_dataset_processor"
# to be usable in this way:
# 		treebank_dataset_processor.processor_error.none
# 		treebank_dataset_processor.processor_error.missing_parent
# 		treebank_dataset_processor.processor_error.missing_main
# 		....
# and also
# 		treebank_dataset_processor.tree_feature.n
# 		treebank_dataset_processor.tree_feature.k2
# 		treebank_dataset_processor.tree_feature.k3
# 		....


# -------------------
# Make the subclasses
#     Note: since 'processor_error' and 'tree_feature' have effectively
#     been turned into actual python classes, they will have a mro() method.

class processor_error:
	__dummy_attr = 0

class tree_feature:
	__dummy_attr = 0

# --------------------------------------------
# add the enumaration values to the subclasses
tbpr = io.treebank_dataset_processor

# processor_error
prerr_enum_vals = list(filter(lambda k: k.find("processor_error_") == 0, tbpr.__dict__.keys()))
for enval_name in prerr_enum_vals:
	# make new enumeration value name
	new_enval_name = enval_name[len("processor_error_"):]
	# set attribute to class "processor_error"
	setattr(processor_error, new_enval_name, getattr(tbpr, enval_name))
	# delete enumeration value from "treebank_dataset_processor"
	delattr(tbpr, enval_name)

# tree_features
tf_enum_vals = list(filter(lambda k: k.find("tree_feature_") == 0, tbpr.__dict__.keys()))
for enval_name in tf_enum_vals:
	# make new enumeration value name
	new_enval_name = enval_name[len("tree_feature_"):]
	# set attribute to class "tree_feature"
	setattr(tree_feature, new_enval_name, getattr(tbpr, enval_name))
	# delete enumeration value from "treebank_dataset_processor"
	delattr(tbpr, enval_name)

# -------------------------------------------
# clean up dummy attributes of the subclasses
delattr(processor_error, "_processor_error__dummy_attr")
delattr(tree_feature, "_tree_feature__dummy_attr")
delattr(tree_feature, "mro")

# -----------------------------------------
# make "processor_error" and "tree_feature"
# attributes of class "treebank_dataset_processor"

# set the subclasses
setattr(tbpr, "processor_error", processor_error)
setattr(tbpr, "tree_feature", tree_feature)

# ------------------------
# clean up local variables

del tbpr
del prerr_enum_vals, tf_enum_vals
del enval_name
del new_enval_name

# even the classes!
del processor_error
del tree_feature

# and the 'io' module
del io
%}
