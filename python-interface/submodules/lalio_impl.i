
%include documentation.i

%{

// C++ includes
#include <lal/graphs.hpp>
#include <lal/io.hpp>

%}

// --------------
// C++ interfaces

%include std_string.i

// --------------
// lal interfaces

%include "../lal/io/edge_list.hpp"

%include "../lal/io/treebank_reader.hpp"
%include "../lal/io/treebank_dataset_reader.hpp"
%include "../lal/io/treebank_dataset_processor.hpp"
