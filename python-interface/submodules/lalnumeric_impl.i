
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

%include "../lal/numeric/integer.hpp"
%include "../lal/numeric/rational.hpp"
%include "../lal/numeric/output.hpp"

%rename(__mod__) operator% (uint64_t) const;
%rename(__mod__) operator% (const integer&) const;

%extend lal::numeric::integer {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::numeric::rational {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
