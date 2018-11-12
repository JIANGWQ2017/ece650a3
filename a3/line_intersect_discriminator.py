class LineIntersectDiscriminator:

	def __init__(self):
		pass

	def lineIntersectDiscriminate(self, line1, line2):
		a1, b1, c1 = self.getLinePara(line1)
		a2, b2, c2 = self.getLinePara(line2)
		d = a1 * b2 - a2 * b1
		p = [0, 0]
		# if d ==0;	 parallel
		if d == 0:
			# could have two intersection
			p_list = []# store crossing point
			v_list = []# store vertex
			# exactly one line ; if two line segment are parrllel but not on one line, there cannot exist intersections.
			if a1*c2-a2*c1==0 and b1*c2-b2*c1 == 0:
				p = [0, 0]
				if max(line2[0][0], line2[1][0]) >= line1[0][0] >= min(line2[0][0], line2[1][0]):
					if max(line2[0][1], line2[1][1]) >= line1[0][1] >= min(line2[0][1], line2[1][1]):
						p[0], p[1] = line1[0][0], line1[0][1]
						p_list.append(tuple(p))
				if max(line2[0][0], line2[1][0]) >= line1[1][0] >= min(line2[0][0], line2[1][0]):
					if max(line2[0][1], line2[1][1]) >= line1[1][1] >= min(line2[0][1], line2[1][1]):
						p[0], p[1] = line1[1][0], line1[1][1]
						p_list.append(tuple(p))
				if max(line1[0][0], line1[1][0]) >= line2[0][0] >= min(line1[0][0], line1[1][0]):
					if max(line1[0][1], line1[1][1]) >= line2[0][1] >= min(line1[0][1], line1[1][1]):
						p[0], p[1] = line2[0][0], line2[0][1]
						p_list.append(tuple(p))
				if max(line1[0][0], line1[1][0]) >= line2[1][0] >= min(line1[0][0], line1[1][0]):
					if max(line1[0][1], line1[1][1]) >= line2[1][1] >= min(line1[0][1], line1[1][1]):
						p[0], p[1] = line2[1][0], line2[1][1]
						p_list.append(tuple(p))
				if p_list:
					v_list = p_list+[tuple([line1[0][0], line1[0][1]]), tuple([line1[1][0], line1[1][1]]),
					tuple([line2[0][0], line2[0][1]]), tuple([line2[1][0], line2[1][1]])]
				return p_list, v_list
			
		# not parallel
		else:
			p[0] = round((b1 * c2 - b2 * c1) * 1.0 / d, 3)
			p[1] = round((c1 * a2 - c2 * a1) * 1.0 / d, 3)
		p = tuple(p)
		if self.isInSegment(p, line1, line2):
			return [p],[p, tuple([line1[0][0], line1[0][1]]), tuple([line1[1][0], line1[1][1]]),
					tuple([line2[0][0], line2[0][1]]), tuple([line2[1][0], line2[1][1]])]
		else:
			# not parallel but does not have intersection
			return [], []

	def isInSegment(self, p, line, line2):
		# if line1 is vertical
		if line[0][0] == line[1][0]:
			# included endpoints
			if max(line[0][1], line[1][1]) >= p[1] >= min(line[0][1], line[1][1]):
				if max(line2[0][0], line2[1][0]) >= p[0] >= min(line2[0][0], line2[1][0]):
					return True
		# if line1 is horizontal
		elif line[0][1] == line[1][1]:
			# included endpoints
			if max(line[0][0], line[1][0]) >= p[0] >= min(line[0][0], line[1][0]) and p[0]:
				if max(line2[0][1], line2[1][1]) >= p[1] >= min(line2[0][1], line2[1][1]):
					return True
		else:
			if max(line[0][0], line[1][0]) >= p[0] >= min(line[0][0], line[1][0]):
				if max(line2[0][1], line2[1][1]) >= p[1] >= min(line2[0][1], line2[1][1]) and  \
						max(line2[0][0], line2[1][0]) >= p[0] >= min(line2[0][0], line2[1][0]):
					return True
		return False

	def getLinePara(self, line):
		# ax+by+c = 0
		a = line[0][1] - line[1][1]
		b = line[1][0] - line[0][0]
		c = line[0][0] * line[1][1] - line[1][0] * line[0][1]
		return a, b, c

	def isPointOnSegment(self,line,p):
		a,b,c = self.getLinePara(line)
		#print('line{} , p{} : {}'.format(line,p,abs(a*p[0]+b*p[1]+c)))
		if abs(a*p[0]+b*p[1]+c)<0.004:
			if max(line[0][0],line[1][0]) >= p[0] >= min(line[0][0],line[1][0]):
				if max(line[0][1],line[1][1]) >= p[1] >= min(line[0][1],line[1][1]):
					return True
		return False




