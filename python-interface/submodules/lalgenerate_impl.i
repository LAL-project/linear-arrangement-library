%{

// lal includes
#include <lal/generate.hpp>

%}

%include stdint.i

// This is a base class of all tree generators. It must be put before
// all other classes.
%include "../lal/generate/tree_gen.hpp"
%template(free_tree_gen) lal::generate::tree_gen<lal::graphs::free_tree>;
%template(rooted_tree_gen) lal::generate::tree_gen<lal::graphs::rooted_tree>;

%include "../lal/generate/all_lab_free_trees.hpp"
%include "../lal/generate/all_lab_rooted_trees.hpp"
%include "../lal/generate/all_ulab_free_trees.hpp"
%include "../lal/generate/all_ulab_rooted_trees.hpp"

// This is a base class. It needs to be put before 'rand_lab_free_trees'.
%include "../lal/generate/rand_ulab_rooted_trees.hpp"

%include "../lal/generate/rand_lab_free_trees.hpp"
%include "../lal/generate/rand_lab_rooted_trees.hpp"
%include "../lal/generate/rand_ulab_free_trees.hpp"

%include "../lal/generate/all_projective_arrangements.hpp"
%include "../lal/generate/rand_projective_arrangements.hpp"

%extend lal::generate::rand_ulab_rooted_trees {
	lal::graphs::rooted_tree get_tree() noexcept {
		return $self->get_tree();
	}
}
%extend lal::generate::rand_lab_rooted_trees {
	lal::graphs::rooted_tree get_tree() noexcept {
		return $self->get_tree();
	}
}
%extend lal::generate::rand_ulab_free_trees {
	lal::graphs::free_tree get_tree() noexcept {
		return $self->get_tree();
	}
}
%extend lal::generate::rand_lab_free_trees {
	lal::graphs::free_tree get_tree() noexcept {
		return $self->get_tree();
	}
}

%extend lal::generate::all_ulab_rooted_trees {
	lal::graphs::rooted_tree get_tree() noexcept {
		return $self->get_tree();
	}
}
%extend lal::generate::all_lab_rooted_trees {
	lal::graphs::rooted_tree get_tree() noexcept {
		return $self->get_tree();
	}
}
%extend lal::generate::all_ulab_free_trees {
	lal::graphs::free_tree get_tree() noexcept {
		return $self->get_tree();
	}
}
%extend lal::generate::all_lab_free_trees {
	lal::graphs::free_tree get_tree() noexcept {
		return $self->get_tree();
	}
}
