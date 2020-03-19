%{

// lal includes
#include <lal/generate.hpp>

%}

%include stdint.i

%include "../lal/generate/all_lab_free_trees.hpp"
%include "../lal/generate/all_ulab_free_trees.hpp"
%include "../lal/generate/all_ulab_rooted_trees.hpp"

// this is a base class.
%include "../lal/generate/rand_ulab_rooted_trees.hpp"

%include "../lal/generate/rand_lab_free_trees.hpp"
%include "../lal/generate/rand_lab_rooted_trees.hpp"
%include "../lal/generate/rand_ulab_free_trees.hpp"

