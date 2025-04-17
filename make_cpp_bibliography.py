license_header = ''
with open('lal/license_header', 'r') as f:
	for line in f:
		license_header += line

bibliography_bib = open('doxyconfig/bibliography.bib', 'r')
bibliography_hpp_detail = open('lal/detail/bibliography.hpp', 'w')
bibliography_entries_hpp = open('lal/bibliography_entries.hpp', 'w')

bibliography_hpp_detail.write(license_header + '\n')

bibliography_hpp_detail.write('#pragma once\n')
bibliography_hpp_detail.write('\n')

bibliography_hpp_detail.write('// C++ includes\n')
bibliography_hpp_detail.write('#include <string_view>\n')
bibliography_hpp_detail.write('#include <array>\n')
bibliography_hpp_detail.write('\n')
bibliography_hpp_detail.write('// lal includes\n')
bibliography_hpp_detail.write('#include <lal/bibliography.hpp>\n')
bibliography_hpp_detail.write('\n')

bibliography_hpp_detail.write('namespace lal {\n')
bibliography_hpp_detail.write('namespace detail {\n')
bibliography_hpp_detail.write('\n')
bibliography_hpp_detail.write('#if defined LAL_REGISTER_BIBLIOGRAPHY\n')
bibliography_hpp_detail.write('\n')
bibliography_hpp_detail.write('// clang-format off\n')
bibliography_hpp_detail.write('\n')

all_entry_names = []

entry = ''
in_entry = False
open_brackets = 0

for line in bibliography_bib:
	l = line.strip()
	if l == '':
		continue
	
	if l[0] == '@':
		in_entry = True
		start = l.find('{') + 1
		end = l.find(',')
		entry_name = l[start:end]
		
		all_entry_names.append(entry_name)
		entry = f'static constexpr std::string_view {entry_name} =\n'
	
	if in_entry:
		if l != '}':
			correct_line = line[:-1]
			correct_line = correct_line.replace('\t', '    ')
			correct_line = correct_line.replace('\\', '\\\\')
			correct_line = correct_line.replace('\\\\"', '\\"')
			
			entry += '\t\"' + correct_line + '\\n\"' + '\n'
		else:
			entry += '\t\"}\";\n'
	
	for c in line:
		if c == '{':
			open_brackets += 1
		elif c == '}':
			open_brackets -= 1
	
	if open_brackets == 0 and entry != '':
		in_entry = False
		bibliography_hpp_detail.write(f'/// String for reference \\cite {entry_name}.\n')
		bibliography_hpp_detail.write(entry + '\n')
		entry = ''

num_entries = len(all_entry_names)
type_string = f'std::array<std::string_view, num_bib_entries>'

bibliography_hpp_detail.write(f'/// All bibliographic entries as an array.\n')
bibliography_hpp_detail.write(f'static constexpr {type_string} list_of_references = []()\n')
bibliography_hpp_detail.write('{\n')
bibliography_hpp_detail.write(f'\treturn {type_string}''{\n')
for entry_name in all_entry_names:
	bibliography_hpp_detail.write(f'\t\t{entry_name},\n')
bibliography_hpp_detail.write('\t};\n')
bibliography_hpp_detail.write('}();\n')
bibliography_hpp_detail.write('\n')
bibliography_hpp_detail.write('// clang-format on\n')
bibliography_hpp_detail.write('\n')
bibliography_hpp_detail.write('#endif\n')
bibliography_hpp_detail.write('\n')

bibliography_hpp_detail.write('} // namespace detail\n')
bibliography_hpp_detail.write('} // namespace lal\n')

bibliography_entries_hpp.write(license_header)
bibliography_entries_hpp.write('\n')
bibliography_entries_hpp.write('#pragma once\n')
bibliography_entries_hpp.write('\n')
bibliography_entries_hpp.write('// C++ includes\n')
bibliography_entries_hpp.write('#include <cstddef>\n')
bibliography_entries_hpp.write('\n')
bibliography_entries_hpp.write('namespace lal {\n')
bibliography_entries_hpp.write('\n')
bibliography_entries_hpp.write('#if defined LAL_REGISTER_BIBLIOGRAPHY\n')
bibliography_entries_hpp.write('\n')
bibliography_entries_hpp.write(f'/// Total number of entries in the bibliography.\n')
bibliography_entries_hpp.write(f'static constexpr std::size_t num_bib_entries = {num_entries};\n')
bibliography_entries_hpp.write('\n')

bibliography_entries_hpp.write('/// A type-safe list of bibliographic entries.\n')
bibliography_entries_hpp.write('enum class bib_entries {\n')
for entry_name in all_entry_names:
	bibliography_entries_hpp.write(f'\t/// Entry \\cite {entry_name}.\n')
	bibliography_entries_hpp.write(f'\t{entry_name},\n')
bibliography_entries_hpp.write('};\n')
bibliography_entries_hpp.write('\n')
bibliography_entries_hpp.write('#endif\n')
bibliography_entries_hpp.write('\n')

bibliography_entries_hpp.write('} // namespace lal\n')
