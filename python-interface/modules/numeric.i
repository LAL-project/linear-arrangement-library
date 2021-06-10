%module numeric

%import definitions.i

%include documentation.i

%{

// C++ includes
#include <sstream>

// lal includes
#include <lal/numeric.hpp>

%}

%ignore lal::numeric::operator<<;
%ignore lal::numeric::integer::operator-();
%ignore lal::numeric::rational::operator-();
%ignore lal::numeric::integer::operator=;
%ignore lal::numeric::rational::operator=;

%rename(__mod__) operator% (uint64_t) const;
%rename(__mod__) operator% (const integer&) const;
%rename(__pow__) pow (uint64_t) const;
%rename(__pow__) pow (const integer&) const;

%include "../lal/numeric/integer.hpp"
%include "../lal/numeric/rational.hpp"
%include "../lal/numeric/output.hpp"

%extend lal::numeric::integer {
	std::string __repr__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::numeric::rational {
	std::string __repr__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}

%pythoncode %{
__definitions = definitions
del definitions

def __radd__(self, k):
	return self + k
def __rsub__(self, k):
	return -self + k
def __rmul__(self, k):
	return self * k
def __rtruediv__(self, k):
	return k/self.to_int()
def __rpow__(self, k):
	return k**self.to_int()

setattr(integer, __radd__.__name__, __radd__)
setattr(integer, __rsub__.__name__, __rsub__)
setattr(integer, __rmul__.__name__, __rmul__)
setattr(integer, __rtruediv__.__name__, __rtruediv__)
setattr(integer, __rpow__.__name__, __rpow__)

del __radd__
del __rsub__
del __rmul__
del __rtruediv__
del __rpow__

def __radd__(self, k):
	return self + k
def __rsub__(self, k):
	return -self + k
def __rmul__(self, k):
	return self * k
def __rtruediv__(self, k):
	copyself = rational(self)
	copyself.invert()
	return k*copyself
def __rpow__(self, k):
	return (k**self.get_numerator())**(1.0/(self.get_denominator().to_int()))

setattr(rational, __radd__.__name__, __radd__)
setattr(rational, __rsub__.__name__, __rsub__)
setattr(rational, __rmul__.__name__, __rmul__)
setattr(rational, __rtruediv__.__name__, __rtruediv__)
setattr(rational, __rpow__.__name__, __rpow__)

%}
