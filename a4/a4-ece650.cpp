#include <iostream>
#include <vector>
#include <string>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <bits/stdc++.h> 
//#include <regex>
using namespace std;


string line;
int vertnum = 0;
vector<int> edge;

bool vflag = false;
bool eflag = false;

class EIterator {
private:
	std::string input;
	int cursor = 0;

public:
	EIterator(std::string);
	std::pair<int, int> getNext();
	bool hasNext();


};


vector<int> isVC(int vertnum,int left,int right)
{
	int k;
	k = (right+left)/2;
	//Minisat::Solver solver;
	vector<int> vc;
	while(left<=right)
	{
		Minisat::Solver solver;
		//init x[i][j]
		Minisat::Lit x[k+1][vertnum];
		for(int i = 0;i<k;i++)
		{
			for(int j = 0;j<vertnum;j++)
			{
				x[i][j] = Minisat::mkLit(solver.newVar());
			}
		}
		// second constrain 
		for(int vid = 0; vid<vertnum;vid++)
		{
			for(int i=0;i<k;i++)
			{
				for(int j =i+1;j<k;j++)
				{
					solver.addClause(~x[i][vid],~x[j][vid]);
					
				}
			}
		}
		// third constrains and first constrain
		for(int vid = 0;vid<k;vid++)
		{
			for(int i =0; i<vertnum;i++)
			{
				for(int j =i+1;j<vertnum;j++)
				{
					solver.addClause(~x[vid][i],~x[vid][j]);
				}
			}
			
			
			Minisat::vec<Minisat::Lit>ps;
			for(int i=0;i<vertnum;i++)
			{
				ps.push(x[vid][i]);
			}
			solver.addClause(ps);
		}

		
		for (int i = 0; i < edge.size(); i = i + 2)
		{
			Minisat::vec<Minisat::Lit>ps;
			int start = edge[i];
			int end = edge[i + 1];
			for(int i =0;i<k;i++)
			{	
				//cout<<start<<' '<<end<<'\n';
				ps.push(x[i][start]);
				ps.push(x[i][end]);
			}
			solver.addClause(ps);
		}


		bool result = solver.solve();
		//cout<<result<<' '<<"k:"<<k<<'\n';
		if(result==true)
		{
			vc.clear();
			for(int i=0;i<k;i++)
			{
				for(int j=0;j<vertnum;j++)
				{
					if(solver.modelValue(x[i][j])==Minisat::l_True)
					{
						vc.push_back(j);
						//cout<<"vc push back j: "<<j<<"WHEN I: "<<i<<" k: "<<k<<'\n';
					}
						
				}
			}
			right=k-1;
		}
		else
		{
			left=k+1;
		}
		k= (right+left)/2;

	}
	
	return vc;
	
}


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
			string vnum;
			vnum = line.substr(2);
			vertnum = stoi(vnum);
			vflag = true;
		}

		else if(line.find("E")==0)
		{	
			edge.clear();
			int value;
			string t1 = line.substr(2);
			EIterator eiterator = EIterator(t1);
			while (eiterator.hasNext())
			{
				std::pair<int,int> e = eiterator.getNext();
				edge.push_back(e.first);
				edge.push_back(e.second);
			}

			eflag = true;
		}
		else
		{
			cout << "Error: Invalid input: "<<line<< endl;
			continue;
		}
		// vertex cover part
		if(eflag and vflag)
		{
			if(edge.size()==0)
			{
				cout<<endl;
				continue;
			}
			vector<int> vc;
			vc = isVC(vertnum,1,vertnum);
			sort(vc.begin(), vc.end()); 
			for(int index=0;index<vc.size();index++)
			{
				cout<<vc[index]<<' ';
			}
			cout<<endl;
		}
		
		
		

		
		
		
	}
	return 0;
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
