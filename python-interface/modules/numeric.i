%module numeric

%import types.i

%include documentation.i

%{
// C++ includes
#include <sstream>

// lal includes
#include <lal/numeric.hpp>
%}

namespace lal {
namespace numeric {

%ignore operator<<;
%ignore integer::operator-();
%ignore rational::operator-();
%ignore integer::operator=;
%ignore rational::operator=;

%rename(__mod__) operator% (uint64_t) const noexcept;
%rename(__mod__) operator% (const integer&) const noexcept;
%rename(__pow__) power (uint64_t) const noexcept;
%rename(__pow__) power (const integer&) const noexcept;

} // -- namespace numeric
} // -- namespace lal

%include "../lal/numeric/integer.hpp"

namespace lal {
namespace numeric {

%template(integer) integer::integer<int64_t,std::enable_if_t<true, bool> = true>;
%template(set_number) integer::set_number<int64_t,std::enable_if_t<true, bool> = true>;
%template(__eq__) integer::operator== <int64_t,std::enable_if_t<true, bool> = true>;
%template(__neq__) integer::operator!= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__lt__) integer::operator< <int64_t,std::enable_if_t<true, bool> = true>;
%template(__le__) integer::operator<= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__gt__) integer::operator> <int64_t,std::enable_if_t<true, bool> = true>;
%template(__ge__) integer::operator>= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__add__) integer::operator+ <int64_t,std::enable_if_t<true, bool> = true>;
%template(__sub__) integer::operator- <int64_t,std::enable_if_t<true, bool> = true>;
%template(__mul__) integer::operator* <int64_t,std::enable_if_t<true, bool> = true>;
%template(__truediv__) integer::operator/ <int64_t,std::enable_if_t<true, bool> = true>;
%template(__iadd__) integer::operator+= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__isub__) integer::operator-= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__imul__) integer::operator*= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__itruediv__) integer::operator/= <int64_t,std::enable_if_t<true, bool> = true>;

} // -- namespace numeric
} // -- namespace lal

%include "../lal/numeric/rational.hpp"

namespace lal {
namespace numeric {

%template(rational) rational::rational<int64_t,std::enable_if_t<true, bool> = true>;
%template(set_number) rational::set_number<int64_t,std::enable_if_t<true, bool> = true>;
%template(__eq__) rational::operator== <int64_t,std::enable_if_t<true, bool> = true>;
%template(__neq__) rational::operator!= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__lt__) rational::operator< <int64_t,std::enable_if_t<true, bool> = true>;
%template(__le__) rational::operator<= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__gt__) rational::operator> <int64_t,std::enable_if_t<true, bool> = true>;
%template(__ge__) rational::operator>= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__add__) rational::operator+ <int64_t,std::enable_if_t<true, bool> = true>;
%template(__sub__) rational::operator- <int64_t,std::enable_if_t<true, bool> = true>;
%template(__mul__) rational::operator* <int64_t,std::enable_if_t<true, bool> = true>;
%template(__truediv__) rational::operator/ <int64_t,std::enable_if_t<true, bool> = true>;
%template(__iadd__) rational::operator+= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__isub__) rational::operator-= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__imul__) rational::operator*= <int64_t,std::enable_if_t<true, bool> = true>;
%template(__itruediv__) rational::operator/= <int64_t,std::enable_if_t<true, bool> = true>;

} // -- namespace numeric
} // -- namespace lal

namespace lal {
namespace numeric {

%extend integer {

	std::string __repr__() const {
		return $self->to_string();
	}

}

%extend rational {

	std::string __repr__() const {
		return $self->to_string();
	}

}

} // -- namespace numeric
} // -- namespace lal

%pythoncode %{
__types = types
del types

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

del __radd__
del __rsub__
del __rmul__
del __rtruediv__
del __rpow__

%}
