r"""
Possible errors that can arise while processing a collection.

There are several reasons why a treebank collection or a single treebank file could not be processed. Because of this, certain methods return one of these values instead of a plain 'false' value.

- ``no_error`` : No error occurred.

	Returned by:
	
	- ``lal.io.treebank_reader``
	- ``lal.io.treebank_processor``
	- ``lal.io.treebank_collection_reader``
	- ``lal.io.treebank_collection_processor``

- ``no_features`` : No features at all were given to the processor.
	Returned by:
	
	- ``lal.io.treebank_processor``
	- ``lal.io.treebank_collection_processor``

- ``treebank_file_does_not_exist`` : A treebank was not found in disk.
	
	Returned by:
	- ``lal.io.treebank_processor``
	- ``lal.io.treebank_collection_processor``

- ``treebank_file_could_not_be_opened`` : A treebank file could not be opened.
	
	Returned by:
	
	- ``lal.io.treebank_reader``
	- ``lal.io.treebank_processor``
	- ``lal.io.treebank_collection_processor``

- ``output_file_could_not_be_opened`` : Output file could not be opened.
	
	Returned by:
	
	- ``lal.io.treebank_processor``
	- ``lal.io.treebank_collection_processor``
	
- ``malformed_treebank``: The treebank file contains errors that should be fixed.
	
	In this case, method @ref lal::io::check_correctness_treebank should be run in order to obtain a report on the errors.
	
	Returned by:
	- ``lal.io.treebank_processor``.

- ``main_file_does_not_exist`` : Main file does not exist.
	
	Returned by:
	
	- ``lal.io.treebank_collection_reader``
	- ``lal.io.treebank_collection_processor``

- ``main_file_could_not_be_opened`` : Main file could not be opened.
	
	Returned by:
	
	- ``lal.io.treebank_collection_reader``
	- ``lal.io.treebank_collection_processor``

- ``output_directory_could_not_be_created`` : Output directory could not be created.
	
	If a directory output does not exist, the library will attempt to create
	it. If this fails then this error is returned.
	
	Returned by:
	
	- ``lal.io.treebank_collection_processor``.

- ``output_join_file_could_not_be_opened`` : The file containing the result of processing a treebank collection could not be opened.
	
	Returned by:
	
	- ``lal.io.treebank_collection_processor``.

- ``treebank_result_file_could_not_be_opened`` : The resulting file of processing a treebank could not be opened
	
	Returned by:
	
	- ``lal.io.treebank_collection_processor``.

- ``some_treebank_file_failed`` : Processing one or more of the treebanks failed.
	
	Returned by:
	
	- ``lal.io.treebank_collection_processor``.

- ``malformed_treebank_collection``: The treebank collection contains errors that should be fixed.
	
	In this case, method @ref lal::io::check_correctness_treebank_collection should be run in order to obtain a report on the errors.
	
	Returned by:
	- ``lal.io.treebank_collection_processor``.

};
"""
