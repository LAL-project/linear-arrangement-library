r"""
Generating different classes of objects.
----------------------------------------

This module contains algorithms for the generation of trees and of arrangements.

Generating trees
----------------

The classes that generate trees have a self-explanatory format:

>>> 1_2_3_trees

The numbers are placeholders for the following:

- 3: rooted/free -- Generate rooted or free trees.
- 2: lab/ulab -- Generate labelled or unlabelled free/rooted trees.
- 1: rand/all -- Indicates whether the generation is to be random (rand) or exhaustive (all). An exhaustive generation will enumerate all lab/ulab rooted/free trees whereas random generation generate trees unformly at random.

Therefore, the class ``rand_lab_rooted_trees`` generates random labelled rooted trees uniformly at random, and the class ``all_ulab_free_trees`` should be used to enumerate all unlabelled free trees.

All classes for tree generation return trees that are preprocessed. This preprocessing varies depending on whether the tree is rooted or free. The full preprocessing details can be checked in class ``tree_gen``, from which all these classes inherit.

Using these classes is straightforward. To generate trees uniformly at random:

>>> TreeGen = lal.generate.rand_2_3_trees(n)
>>> for i in range(0,100):
>>>     T = TreeGen.get_tree()
>>>     # ...

To enumerate all trees:

>>> lal.generate.all_2_3_trees TreeGen(n)
>>> while TreeGen.has_next():
>>>     TreeGen.next()
>>>     T = TreeGen.get_tree()
>>>     # ...

(remember to replace the numbers in the actual code!).

    *Remark:* In this documentation you will find 8 classes for random generation of trees. Users should refrain from using those starting with '__' as the trees returned by these classes are not preprocessed. However, if one wishes to know the implementation details (as for algorithms implemented, and the papers cited), please, read the documentation of said classes.

Generating arrangements
-----------------------

This module contains classes for the generation of arrangements `of a given tree`. Depending on the type of arrangements, the given tree should be free or rooted accordingly.

Again, the names for these classes are also self-explanatory:

>>> 1_2_arrangement


The numbers are placeholders for the following:

- 2: projective -- Indicates whether the generated arrangements should be projective.
- 1: rand/all -- As before, this indicates whether the generation is to be random (rand) or exhaustive (all). An exhaustive generation will enumerate all arrangements

Therefore, the class @ref rand_projective_arrangements generates random projective arrangements of a tree, and the class @ref all_projective_arrangements should be used to enumerate all projective arrangements of a tree.

Using these classes is straightforward. To generate trees uniformly at random:

>>> # given a tree T (of the appropriate type -- indicated with the '2')
>>> ArrGen = lal.generate.rand_2_arrangement(T)
>>> for  i in range(0,100):
>>>     arr = ArrGen.make_rand_arrgmnt()
>>>     # ...

To enumerate all arrangements:

>>> # given a tree T (of the appropriate type -- indicated with the '2')
>>> ArrGen = lal.generate.all_2_arrangement(T)
>>> while ArrGen.has_next():
>>>     ArrGen.next()
>>>     arr = ArrGen.get_arrangement()

(remember to replace the numbers in the actual code!).

    *Remark:* In all cases, the arrangements generated are considered to be labelled, i.e., there are no symmetries taken into account when it comes to enumerating or generating uniformly at random said arrangements. For example, for an \f$n\f$-vertex star tree, the class @ref all_projective_arrangements will enumerate \f$n!\f$ arrangements.

"""
