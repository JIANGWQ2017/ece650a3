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
//			vnum = vmatchResult[2];
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
		else if(line.find("s")==0)
		{
			bool flag = true;
			vector <int>startend;

			string t1 = line.substr(2,1);
			int v1 = stoi(t1);
			string t2 = line.substr(4,1);
			int v2 = stoi(t2);
			if (v1 >(vertnum-1) or v2>(vertnum-1))
			{
				cout<<"Error: out of range\n";
				flag = false;
			}
			else
			{
				startend.push_back(v1);
				startend.push_back(v2);
			}
			if (flag == false)
			{
				continue;
			}
			int dist[maxnum];
			int prev[maxnum];
			for (int i = 0; i < vertnum; i++)
				dist[i] = inf;
			DistanceMatrix dm;
			vector<vector<int> > DM = dm.computeDistanceMatrix(edges, vertnum);
			if (dm.shortestPath(vertnum, startend[0], DM, dist, prev, startend[1]))
			{
				dm.showPath();
			}
			else 
			{
				continue;
			}
			
		}
		else
		{
			cout << "Error: Invalid input: "<<line<< endl;
			continue;
		}
	}
	return 0;
}


void DistanceMatrix::Dijkstra(int n, int v, int *dist, int *prev, vector<vector<int> > dm)
{
	bool s[maxnum];
	//init 
	for (int i = 0; i < n; ++i)
	{
		dist[i] = dm[v][i];
		s[i] = 0;
		if (dist[i] == inf)
			prev[i] = -1;
		else
			prev[i] = v;
	}
	dist[v] = 0;
	s[v] = 1;

	for (int i = 1; i < n; ++i) 
	{
		int tmp = inf;
		int u = v;
		for (int j = 0; j < n; ++j)
			if ((!s[j]) && dist[j] < tmp)
			{
				u = j;
				tmp = dist[j];
			}
		s[u] = 1;
		for (int j = 0; j < n; ++j)
			if ((!s[j]) && dm[u][j] < inf)
			{
				int newdist = dist[u] + dm[u][j];
				if (newdist < dist[j])
				{
					dist[j] = newdist;
					prev[j] = u;
				}
			}
	}
}

vector<vector<int> > DistanceMatrix::computeDistanceMatrix(vector<int> edges, int vnum)
{
	vector<vector<int> > dm(vnum, vector<int>(vnum));
	size = vnum;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j)
			{
				dm[i][j] = 0;
			}
			else
			{
				dm[i][j] = inf;
			}
		}
	}
	for (int i = 0; i < edges.size(); i = i + 2)
	{
		int start = edges[i];
		int end = edges[i + 1];
		dm[start][end] = 1;
		dm[end][start] = 1;
	}
	return dm;
}


bool  DistanceMatrix::shortestPath(int n, int v, vector<vector<int> > dm, int *dist, int *prev, int end)
{

	Dijkstra(size, v, dist, prev, dm);
	path.push_back(end);
	int node = end;
	while (node != v)
	{
		node = prev[node];
		if (node == -1)
		{
			cout << "Error: no such path\n";
			return false;
		}
		path.push_back(node);
	}
	return true;
}

void DistanceMatrix::showPath()
{
	for (int i = path.size() - 1; i >= 0; i--)
	{
		if (i != 0)
		{
			cout << path[i] << '-';
		}
		else
		{
			cout << path[i];
		}
	}
	cout << '\n';
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
