%module properties

%import definitions.i
%import graphs.i
%import numeric.i

%include documentation.i

%{

#include <lal/graphs.hpp>
#include <lal/properties.hpp>

%}

%include "../lal/properties/C_rla.hpp"
%include "../lal/properties/D_rla.hpp"
%include "../lal/properties/degrees.hpp"
%include "../lal/properties/mean_hierarchical_distance.hpp"
%include "../lal/properties/Q.hpp"
%include "../lal/properties/tree_centre.hpp"
%include "../lal/properties/tree_centroid.hpp"
%include "../lal/properties/tree_diameter.hpp"

%pythoncode %{
__definitions = definitions
del definitions

# remove unnecessary modules
del graphs
del numeric
del tree_type
%}
