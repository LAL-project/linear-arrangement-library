r"""

Properties of graphs
--------------------

This module contains functions to calculate properties of graphs that do not depend, whether implicitly or explicitly, on a linear arrangement. Most, if not all, properties can be calculated as exact rational numbers (see ``lal.numeric.rational``), but also as floating point values of double precision. For the former, add the suffix '_rational' at the end of the function name. For example, the function ``variance_C`` returns the variance of the number of crossings 'C' as a floating point value. By adding the suffix, i.e., ``variance_C_rational``, we obtain said variance as an exact rational value.

To see what a rational value is in the context of this library, see the documentation of the module ``lal::numeric``.

"""
