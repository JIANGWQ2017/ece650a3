from line_intersect_discriminator import LineIntersectDiscriminator


class GraphGenerator:

	def __init__(self, st_dict,exist_node):
		self.st_dict = st_dict
		self.v_dict = []
		self.edges = []
		self.exist_node = exist_node
		
	def generateGraph(self):
		lineID = LineIntersectDiscriminator()
		checked_key = []
		intersection = []
		vertex = []
		candidate_edge = []
		v_dict = {}
		
		for key1, value1 in self.st_dict.items():
			checked_key.append(key1)
			for key2, value2 in self.st_dict.items():
				if key2 not in checked_key and key2 != key1:
					for i in range(len(value1)-1):
						try:
							line1 = tuple([value1[i], value1[i+1]])
						except:
							break
						for i in range(len(value2)-1):
							try:
								line2 = tuple([value2[i], value2[i+1]])
							except:
								break
							Ps, Vs = lineID.lineIntersectDiscriminate(line1, line2)
							if Ps and Vs:
								for p in Ps:
									for v in Vs:
										if v not in Ps:
											candidate_edge.append([p,v])
								intersection.extend(Ps)
								vertex.extend(Vs)
								
		# remove duplicated vertex
		vertex = list(set(vertex))
		intersection = list(set(intersection))
		
		# index those vertex
		count = 0
		exist_node_reverse = {v:k for k,v in self.exist_node.items()}
		for i in range(len(vertex)):
			if vertex[i] not in self.exist_node.values():
				while count in self.exist_node.keys():
					count += 1
				v_dict[count] = vertex[i]
				self.exist_node[count] = vertex[i]
			else:
				v_dict[exist_node_reverse[vertex[i]]] = vertex[i]
			
		# fetch index according to coordinate
		v_dict_reverse = {v:k for k,v in v_dict.items()}
		
		# if intersection on segment, modify edges
		for e in candidate_edge:
			for p in intersection:
				if p != e[0]:
					if lineID.isPointOnSegment(e,p):
						#print('{} on segment {}'.format(p,e))
						e[1] = p
		for e in candidate_edge:
			e[0] = v_dict_reverse[e[0]]
			e[1] = v_dict_reverse[e[1]]
		candidate_edge = list(set([tuple(e) for e in candidate_edge]))
		
		# remove duplicated edges e.g.(5,1)(1,5)
		es = []
		es.extend(candidate_edge)
		for i in range(len(es)-1):
			for j in range(i+1,len(es)):
				if set(es[i]) == set(es[j]):
					candidate_edge.remove(es[j])
		edges = []
		edges.extend(candidate_edge)
		self.v_dict = v_dict
		self.edges = edges	
	
	
	def printGraph(self):
		print('V {0}'.format(max(self.v_dict.keys())))
		t = []
		for e in range(len(self.edges)):
			if not e == len(self.edges)-1:
				t.append('<{0},{1}>,'.format(self.edges[e][0],self.edges[e][1]))
			else:
				t.append('<{0},{1}>'.format(self.edges[e][0],self.edges[e][1]))
		res = "E {"
		for i in t:
			res += i
		res+="}"
		print(res)