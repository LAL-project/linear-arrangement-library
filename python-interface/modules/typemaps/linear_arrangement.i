%{
#define __LAL_ERROR_INPUT_NOT_A_LIST "\n\
\
    I can't construct a lal::linear_arrangement with something that is not a list.\n\
    However, if you think that a lal::linear_arrangement should be constructible from\n\
    other python objects (like the one you are tying to use) then open an issue\n\
    explaining what you would like to do at the library's github page:\n\
    \n\
        https://github.com/LAL-project/linear-arrangement-library/issues\n\
    \n\
    Thank you for your collaboration and understanding,\n\
    \n\
    Yours faithfully,\n\
    \n\
    LAL developers\n\
"

#define __LAL_ERROR_INPUT_LIST_INVALID_VALUES "\n\
\
    Input list must contain only non-negative ints to be convertible to a linear.\n\
    arrangement. If you believe this error message is incorrect, that is, the list\n\
    passed as parameter is actually a list of only non-negative integers then,\n\
    please, open an issue explaining this situation at the library's github page:\n\
    \n\
        https://github.com/LAL-project/linear-arrangement-library/issues\n\
    \n\
    Thank you for your collaboration and understanding,\n\
    \n\
    Yours faithfully,\n\
    \n\
    LAL developers\n\
"
%}

%typemap(in) lal::linear_arrangement const& {
	void *LAL_linarr_arg_pointer = 0;
	int LAL_linarr_result = SWIG_ConvertPtr($input, &LAL_linarr_arg_pointer, SWIGTYPE_p_lal__linear_arrangement, 0 | 0);
	if (SWIG_IsOK(LAL_linarr_result)) {
		$1 = reinterpret_cast<lal::linear_arrangement *>(LAL_linarr_arg_pointer);
	}
	else {
		if (1 != PyList_Check($input)) {
			PyErr_SetString(PyExc_TypeError, __LAL_ERROR_INPUT_NOT_A_LIST);
			SWIG_fail;
		}
		const int size = PyList_Size($input);
		
		// allocate the linear arrangement
		$1 = new lal::linear_arrangement(static_cast<uint64_t>(size));
		
		// check that the list's contents are correct
		// while building the linear arrangement
		for (lal::node u = 0; u < size; ++u) {
			PyObject *o = PyList_GetItem($input, u);
			bool correct = true;
			
			if (PyInt_Check(o)) {
				long v = PyInt_AsLong(o);
				if (v < 0) {
					std::cout << "Error: value '" << std::to_string(v) << "' is negative.\n";
					correct = false; 
				}
				else {
					$1->assign(u, lal::position_t{static_cast<uint64_t>(v)});
				}
			}
			
			if (not correct) {
				PyErr_SetString(PyExc_TypeError, __LAL_ERROR_INPUT_LIST_INVALID_VALUES);
				SWIG_fail;
			}
		}
	}
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) lal::linear_arrangement const& {
	const int __LAL_input_is_list = PyList_Check($input) ? 1 : 0;
	
	const int __LAL_input_is_object =
	[&]() {
		void *__LAL_argp = 0;
		int __LAL_result = SWIG_ConvertPtr($input, &__LAL_argp, SWIGTYPE_p_lal__linear_arrangement, 0 | 0);
		return SWIG_IsOK(__LAL_result) ? 1 : 0;
	}();
	
	$1 = __LAL_input_is_list + __LAL_input_is_object >= 1 ? 1 : 0;
}
