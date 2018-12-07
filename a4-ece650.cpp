#include <iostream>
#include <vector>
#include <string>
//#include <regex>
using namespace std;


string line;
int vertnum = 0;
vector<int> edges;
const int inf = 999999;
const int maxnum = 10000;

class DistanceMatrix
{
private:
	int size;
	vector<int> path;
	void Dijkstra(int n, int v, int *dist, int *prev, vector<vector<int> > dm);

public:
	vector<vector<int> > computeDistanceMatrix(vector<int> edges, int vnum);
	bool shortestPath(int n, int v, vector<vector<int> > dm, int *dist, int *prev, int end);
	void showPath();
	int Size()
	{
		return size;
	}
};

class EIterator {
private:
	std::string input;
	int cursor = 0;

public:
	EIterator(std::string);
	std::pair<int, int> getNext();
	bool hasNext();


};




int main()
{
	while (!std::cin.eof()) {
		// read a line of input until EOL and store in a string
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;

		if(line.find("V")==0)
		{
			vertnum = 0;
			cout<<line<<endl;
			string vnum;
			vnum = line.substr(2);
			vertnum = stoi(vnum);
		}

		else if(line.find("E")==0)
		{	
			edges.clear();
			cout<<line<<endl;
			int value;
			string t1 = line.substr(2);
			EIterator eiterator = EIterator(t1);
			while (eiterator.hasNext())
			{
				std::pair<int,int> e = eiterator.getNext();
				edges.push_back(e.first);
				edges.push_back(e.second);
			}

		}
		//else if (regex_match(line,smatchResult,spattern))
		else
		{
			cout << "Error: Invalid input: "<<line<< endl;
			continue;
		}
	}
	return 0;
}


	for (int i = 0; i < edges.size(); i = i + 2)
	{
		int start = edges[i];
		int end = edges[i + 1];
		dm[start][end] = 1;
		dm[end][start] = 1;
	}




EIterator::EIterator(std::string input) {
	if (input[0] == '{') {
		this->cursor = 1;
		this->input = input;
	} else {
		throw "Error: *Unexpected input.";
	}

}

std::pair<int, int> EIterator::getNext() {
	int first = 0, second = 0;
	int *which = &first;
	while(this->input[this->cursor]!='>'){
		switch (this->input[cursor]) {
			case '<':
				which = &first;
				break;
			case ',':
				which = &second;
				break;
			default:  // number
				*which = (*which * 10) + (input[cursor] - '0');
		}
		cursor++;

	}
	cursor++;
	if (this->input[cursor] == ',') {
		cursor++;
	}


	return std::pair<int, int>{first, second};
}

bool EIterator::hasNext() {
	return this->input[this->cursor] != '}';
}
