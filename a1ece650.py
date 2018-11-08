import sys
from input_parser import InputParser
# YOUR CODE GOES HERE


def main():
	# YOUR MAIN CODE GOES HERE
	st_dict = {}
	exist_node = {}
	# sample code to read from stdin.
	# make sure to remove all spurious print statements as required
	# by the assignment
	while True:
		line = sys.stdin.readline()
		if line.rstrip().strip() == '':
			break
		else:
			print('read a line:', line)
			# parse the input line
			i_p = InputParser(line, st_dict,exist_node)
			i_p.parseInput()

	print('Finished reading input')

	# return exit code 0 on successful termination
	sys.exit(0)


if __name__ == '__main__':
	main()
