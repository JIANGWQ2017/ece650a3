import re
from graph_generator import GraphGenerator

class InputParser:
	def __init__(self, line, st_dict,exist_node):
		self.st_dict = st_dict
		self.line = line
		self.exist_node = exist_node
		
	def parseInput(self):
		self.line = self.line.strip().rstrip()
		# add street
		if self.line.startswith('a '):
			pattern = r'a\s+".+"\s+(\(\s*-?[ 0-9]+\s*,\s*-?[ 0-9]+\s*\)\s*)+'
			match = re.match(pattern, self.line)
			if not match:
				print('Error: wrong command format')
				return None
			if match.group(0) == self.line:
				st_name_pattern = r'"[\s|a-z|A-Z]+"'
				try:
					st_name = re.findall(st_name_pattern, self.line)[0].lower()
				except:
					print('Error: street name must only contain Alphabetical and space characters')
					return None
				if st_name in self.st_dict.keys():
					print('Error: the street name has already exists, use c \"streetname\" (coordinates)+ to change street')
					return None
				points_pattern = r'\s*-?[ 0-9]+\s*,\s*-?[ 0-9]+\s*'
				points = re.findall(points_pattern, self.line)
				st_points = []
				for p in points:
					nums = p.split(',')
					x_axis = float(nums[0].replace(' ',''))
					y_axis = float(nums[1].replace(' ',''))
					point_location = tuple([x_axis, y_axis])
					st_points.append(point_location)
				self.st_dict[st_name] = st_points
				return self.st_dict
			else:
				print('Error: wrong command format')
				return None
		# generate
		elif self.line == 'g':
			if self.st_dict == {}:
				print('Error: No street, cannot generate graph')
				return None
			gg = GraphGenerator(self.st_dict,{})
			gg.generateGraph()
			gg.printGraph()
			
		# remove street
		elif self.line.startswith('r '):
			pattern = r'r\s+".+"\s*'
			match = re.match(pattern, self.line)
			if not match:
				print('Error: wrong command format')
				return None
			if match.group(0) == self.line:
				st_name_pattern = r'"[\s|a-z|A-Z]+"'
				try:
					st_name = re.findall(st_name_pattern, self.line)[0].lower()
				except:
					print('Error: street name must only contain Alphabetical and space characters')
					return None
				try:
					del self.st_dict[st_name]
					return self.st_dict
				except:
					print('Error: The street you input does not exist')
					return None
			else:
				print('Error: wrong command format')
				return None
		# change
		elif self.line.startswith('c '):
			pattern = r'c\s+".+"\s+(\(\s*[ 0-9]+\s*,\s*[ 0-9]+\s*\)\s*)+'
			match = re.match(pattern, self.line)
			if not match:
				print('Error: wrong command format')
				return None
			if match.group(0) == self.line:
				st_name_pattern = r'"[\s|a-z|A-Z]+"'
				try:
					st_name = re.findall(st_name_pattern, self.line)[0].lower()
				except:
					print('Error: street name must only contain Alphabetical and space characters')
					return None
				if st_name not in self.st_dict.keys():
					print('Error: the street you wanna change does not exist')
					return None
				else:
					points_pattern = r'\s*-?[ 0-9]+\s*,\s*-?[ 0-9]+\s*'
					points = re.findall(points_pattern, self.line)
					if len(points) <2:
						print('Error: invalid input:', self.line)
						return None
					st_points = []
					for p in points:
						nums = p.split(',')
						x_axis = float(nums[0].replace(' ',''))
						y_axis = float(nums[1].replace(' ',''))
						point_location = tuple([x_axis, y_axis])
						st_points.append(point_location)
					self.st_dict[st_name] = st_points
					return self.st_dict
			else:
				print('Error: wrong command format')
				return None
		elif self.line == '':
			return None
		else:
			print('Error: wrong command: ',self.line)
			return None
