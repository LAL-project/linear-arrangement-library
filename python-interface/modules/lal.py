import os
path_to_this_init = os.path.dirname(os.path.abspath(__file__))

import sys
sys.path.append(path_to_this_init)

del path_to_this_init
del os
del sys

from lal import generate
from lal import graphs
from lal import io
from lal import iterators
from lal import linarr
from lal import numeric
from lal import properties
from lal import utilities
from lal import version
from lal import types
