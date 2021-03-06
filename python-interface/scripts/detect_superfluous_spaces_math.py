from sys import argv
import re

def find_matching_strings(all_expressions, regexpr):
	found = False
	for expr in all_expressions:
		iterator = space_begin.finditer(expr)
		for match in iterator:
			begin,end = match.span()
			print("ERROR: matched math expression with superfluous spaces!")
			print("--->", expr)
			found = True
	return found

f = open(argv[1], 'r')
whole_file = ""
for line in f: whole_file += line
f.close()

correct_expressions = re.compile('\$([^\$]*)\$', re.IGNORECASE)
iterator_expressions = correct_expressions.finditer(whole_file)
all_expressions = []
for expression in iterator_expressions:
	begin,end = expression.span()
	all_expressions.append(whole_file[begin:end])

space_begin = re.compile('\$ +([^\$]*)\$', re.IGNORECASE)
space_beginend = re.compile('\$ +([^\$]*)\ +\$', re.IGNORECASE)
space_end = re.compile('\$([^\$]*)\ +\$', re.IGNORECASE)

found1 = find_matching_strings(all_expressions, space_begin)
found2 = find_matching_strings(all_expressions, space_beginend)
found3 = find_matching_strings(all_expressions, space_end)

if found1 or found2 or found3: exit(1)
exit(0)
