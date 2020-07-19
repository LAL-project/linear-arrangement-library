/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Linear Arrangement Library.
 *
 *  Linear Arrangement Library is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Linear Arrangement Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/
 
#include <lal/utils/gmp/utils.hpp>

namespace lal {
namespace numeric {
namespace gmp_utils {

void mpz_pow_mpz(mpz_t& r, const mpz_t& b, const mpz_t& e) {
	if (mpz_fits_ulong_p(e)) {
		mpz_pow_ui(r, b, mpz_get_ui(e));
		return;
	}

	if (mpz_even_p(e)) {
		// r = (b^(e/2))^2

		mpz_t e_half;
		mpz_init(e_half);

		// e_half = e/2
		mpz_div_ui(e_half, e, 2);

		// r = b^(e/2)
		mpz_pow_mpz(r, b, e_half);

		// r = (b^(e/2))^2 = b^e
		mpz_mul(r, r, r);
		mpz_clear(e_half);
		return;
	}

	// r = (b^(e - 1))*b

	mpz_t e_minus_one;
	mpz_init(e_minus_one);
	// e_minus_one = e - 1
	mpz_sub_ui(e_minus_one, e, 1);

	// r = b^(e - 1)
	mpz_pow_mpz(r, b, e_minus_one);

	// r = (b^(e - 1))*b = b^e
	mpz_mul(r, r, b);
	mpz_clear(e_minus_one);
}

void mpz_divide_mpq(mpq_t& num, const mpz_t& c) {
	mpz_t b;
	mpz_init(b);

	mpq_get_den(b, num);	// num = a/b

	mpz_mul(b, b, c);

	mpq_set_den(num, b);
	mpq_canonicalize(num);

	mpz_clear(b);
}

void mpq_divide_mpq(mpq_t& num, const mpq_t& den) {
	mpz_t a, b, c, d;
	mpz_inits(a, b, c, d, nullptr);

	mpq_get_num(a, num);	// num = a/b
	mpq_get_den(b, num);
	mpq_get_num(c, den);	// den = c/d
	mpq_get_den(d, den);

	mpz_mul(a, a, d);
	mpz_mul(b, b, c);

	mpq_set_num(num, a);
	mpq_set_den(num, b);
	mpq_canonicalize(num);

	mpz_clears(a, b, c, d, nullptr);
}

void operate_power(mpq_t& res, uint64_t p) {
	if (p == 0) {
		mpq_set_si(res, 1, 1);
		return;
	}
	if (p == 1) {
		return;
	}

	mpz_t num, den;
	mpz_inits(num, den, nullptr);

	// get numerator and denominator of 'res'
	mpq_get_num(num, res);
	mpq_get_den(den, res);

	// operate power
	mpz_pow_ui(num, num, p);
	mpz_pow_ui(den, den, p);

	// set value into 'res'
	mpq_set_num(res, num);
	mpq_set_den(res, den);

	// canonicalise
	mpq_canonicalize(res);

	mpz_clears(num, den, nullptr);
}

void operate_power(mpq_t& res, const mpz_t& p) {
	if (mpz_cmp_ui(p, 0) == 0) {
		mpq_set_si(res, 1, 1);
		return;
	}
	if (mpz_cmp_ui(p, 1) == 0) {
		return;
	}

	mpz_t num, den;
	mpz_inits(num, den, nullptr);

	// get numerator and denominator of 'res'
	mpq_get_num(num, res);
	mpq_get_den(den, res);

	// operate power
	gmp_utils::mpz_pow_mpz(num, num, p);
	gmp_utils::mpz_pow_mpz(den, den, p);

	// set value into 'res'
	mpq_set_num(res, num);
	mpq_set_den(res, den);

	// canonicalise
	mpq_canonicalize(res);

	mpz_clears(num, den, nullptr);
}

void mpz_zero(mpz_t& z) {
	mpz_init_set_ui(z, 0);
}

void mpz_one(mpz_t& o) {
	mpz_init_set_ui(o, 1);
}

size_t mpz_bytes(const mpz_t& v) {
	size_t alloc = static_cast<size_t>(v[0]._mp_alloc);
	return sizeof(mp_limb_t)*alloc;
}

} // -- namespace gmp_utils
} // -- namespace numeric
} // -- namespace lal
