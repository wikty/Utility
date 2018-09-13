import os
import sys
import random

def main(filename):
	with open(filename, 'r') as f:
		lines = f.readlines()
		for i in range(0, len(lines)):
			j = random.randint(i, len(lines)-1)
			temp = lines[i]
			lines[i] = lines[j]
			lines[j] = temp
		newf = open("shuffle_" + filename, 'w')
		newf.writelines(lines)


if __name__ == '__main__':
	fn = sys.argv[1]
	if fn and os.path.isfile(fn):
		main(fn)