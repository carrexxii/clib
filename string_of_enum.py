#!/usr/bin/env python3
import os
import argparse
import re

enums = {}

def parse_file(fname):
	path = os.path.join(os.getcwd(), fname)
	if not os.path.isfile(path):
		print("Error, cannot find input file: \"%s\"" % path)
		exit(1)

	with open(path, "r") as file:
		enum_start = re.compile(r"^\s*(?:enum)\s*(\w+)\s*{\s*$") # enum <name> {
		enum_end   = re.compile(r"^\s*};\s*$")                   # };
		enum_val   = re.compile(r"^\s*(\w+)\s*(?:=\s*(.+))?,$")  # <enum> [= value],
		for line in file:
			name = re.match(enum_start, line)
			if name:
				if name in enums:
					print("Error, duplicate names for enums: ", name, "\n\t", line)
					exit(1)

				vals = {}
				line = file.readline() # Consume enum line
				while True: # do-while
					val = re.match(enum_val, line)
					if val:
						vals[val[1]] = val[2]

					line = file.readline()
					if re.match(enum_end, line):
						break

				enums[name[1]] = vals

def output_enums(fname, mode):
	out = []
	for name, enum in enums.items():
		max_len = max([len(e) for e in enum]) # for ternary alignment

		# Add underscores between words
		# Words are detected by case changing from upper to lower
		# AIStateType = AI_STATE_TYPE | EntityGroupMask = ENTITY_GROUP_MASK
		ename = ""
		for i in range(len(name)):
			if i and name[i].isupper() and not name[i+1].isupper():
				ename += '_'
			ename += name[i].upper()

		out.append("#define STRING_OF_%s(x) \\\n" % ename)
		for e, v in enum.items():
			padding = ' '*(max_len - len(e))
			out.append("\t((x) == {0}){1}? \"{0}\"{1}: \\\n".format(e, padding))
		out.append("\t\"<Unknown Value for \\\"%s\\\">\"\n\n" % name)

	if fname:
		with open(fname, mode) as file:
			file.writelines(out)
	else:
		print("".join(out))

if __name__ == "__main__":
	parser = argparse.ArgumentParser(
		prog="string_of_enum",
		description="Extract enums from C files and create string macros for them.",
	)
	parser.add_argument("input_files", nargs='+')
	parser.add_argument("-o", dest="output_file")
	parser.add_argument("-a", "--append", dest="append", action="store_true", default=False)
	args = parser.parse_args()

	for file in args.input_files:
		parse_file(file)
	output_enums(args.output_file, "a" if args.append else "w")

