from sys import argv
import re

f = open(argv[1], 'r')
whole_file = ""
for line in f: whole_file += line
f.close()

last_match_end = 0
new_file = ""

p = re.compile('\$([^\$]*)\$', re.IGNORECASE)
iterator = p.finditer(whole_file)
for match in iterator:
	begin,end = match.span()
	before_expr = whole_file[(last_match_end+1):begin]
	inside_expr = whole_file[begin+1:end-1]
	
	new_file += before_expr + ":math:`" + inside_expr + "` "
	
	last_match_end = end

new_file += whole_file[last_match_end+1:]

f = open(argv[1], 'w')
f.write(new_file)
f.close()
