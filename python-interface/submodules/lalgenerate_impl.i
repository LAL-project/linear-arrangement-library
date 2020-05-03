%{

// lal includes
#include <lal/generate.hpp>

%}

%include stdint.i

%include "../lal/generate/all_lab_free_trees.hpp"
%include "../lal/generate/all_ulab_free_trees.hpp"
%include "../lal/generate/all_ulab_rooted_trees.hpp"

// This is a base class. It needs to be put before 'rand_lab_free_trees'.
%include "../lal/generate/rand_ulab_rooted_trees.hpp"

%include "../lal/generate/rand_lab_free_trees.hpp"
%include "../lal/generate/rand_lab_rooted_trees.hpp"
%include "../lal/generate/rand_ulab_free_trees.hpp"

%include "../lal/generate/rand_arrangements.hpp"
