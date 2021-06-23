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

%template(integer) lal::numeric::integer::integer<int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(set_number) lal::numeric::integer::set_number<int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__eq__) lal::numeric::integer::operator== <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__neq__) lal::numeric::integer::operator!= <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__lt__) lal::numeric::integer::operator< <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__le__) lal::numeric::integer::operator<= <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__gt__) lal::numeric::integer::operator> <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__ge__) lal::numeric::integer::operator>= <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__add__) lal::numeric::integer::operator+ <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__sub__) lal::numeric::integer::operator- <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__mul__) lal::numeric::integer::operator* <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__truediv__) lal::numeric::integer::operator/ <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;

%include "../lal/numeric/rational.hpp"

%template(rational) lal::numeric::rational::rational<int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(set_number) lal::numeric::rational::set_number<int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__eq__) lal::numeric::rational::operator== <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__neq__) lal::numeric::rational::operator!= <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__lt__) lal::numeric::rational::operator< <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__le__) lal::numeric::rational::operator<= <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__gt__) lal::numeric::rational::operator> <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__ge__) lal::numeric::rational::operator>= <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__add__) lal::numeric::rational::operator+ <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__sub__) lal::numeric::rational::operator- <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__mul__) lal::numeric::rational::operator* <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;
%template(__truediv__) lal::numeric::rational::operator/ <int64_t,std::enable_if_t<std::is_integral_v<int64_t>, bool> = true>;

%extend lal::numeric::integer {
	std::string __repr__() const {
		return $self->to_string();
	}
}
%extend lal::numeric::rational {
	std::string __repr__() const {
		return $self->to_string();
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

del __radd__
del __rsub__
del __rmul__
del __rtruediv__
del __rpow__

%}
