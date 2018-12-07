// based on the example from
// https://stackoverflow.com/questions/13041416/redirect-stdout-of-two-processes-to-another-processs-stdin-in-linux-c
#include<vector>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <limits>
#include <pthread.h>
#include<algorithm>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <bits/stdc++.h> 
#include <time.h>
const int NUM_THREADS=4;
using namespace std;

//global variable
int vertnum = 0;
vector<int> edge;
vector<pair<int,int>> edge_pair;



// needed structure or functions 
class EIterator {
private:
	std::string input;
	int cursor = 0;

public:
	EIterator(std::string);
	std::pair<int, int> getNext();
	bool hasNext();
};
struct node 
{ 
	int value; 
	int index; 
}; 
bool cmp(struct node a, struct node b) 
{ 
	if(a.value < b.value)
	{ 
		return true; 
	} 
	return false; 
} 



// entry of input parsing function
void* input(void*)
{

	bool vflag =false;
	bool eflag = false;
	/////////// input part /////////
	// read a line of input until EOL and store in a string
	while (!std::cin.eof()) 
	{
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			continue;
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
				edge_pair.push_back(e);
			}
			eflag = true;
		}
		else
		{
			cout << "Error: Invalid input: "<<line<< endl;
			continue;
		}
		if(eflag and vflag)
		{
			if(edge.size()==0)
			{
				cout<<endl;
				continue;
			}
			else
			{
				break;
			}
		}
	}
	return 0;
}


// entry of approx method 1
void* APPROX_VC_1(void*)
{
	int vertex_count[vertnum];
	for(int i =0;i<vertnum;i++)
	{
		vertex_count[i]=0;
	}
	for(int i =0;i<edge.size();i++)
	{
		vertex_count[edge[i]] = vertex_count[edge[i]]+1;
	}

	node* nodes=new node[vertnum];
	for(int i =0; i<vertnum;i++)
	{
		nodes[i].index = i;
		nodes[i].value = vertex_count[i];
	}
	sort(nodes,nodes+vertnum,cmp);
	
	vector<pair<int,int>> temp(edge_pair);
	vector<int> vc;
	int c = vertnum-1;
	while(!temp.empty())
	{
		int most_incident = nodes[c].index;
		c = c-1;
		vc.push_back(most_incident);
		//remove connected edge
		vector<pair<int,int>> t;
		for(int i=0;i<temp.size();i++)
		{
			if(most_incident!=temp[i].first and most_incident!=temp[i].second)
			{
				t.push_back(edge_pair[i]);
			}
		}
		temp = t;
	}
	sort(vc.begin(),vc.end());
	// output result
	cout<<"APPROX_VC_1: ";
	for(int i=0;i<vc.size();i++)
	{
		if(i!=vc.size()-1)
		{
			cout<<vc[i]<<',';
		}
		else
		{
			cout<<vc[i]<<"\n";
		}
		
	}
	return 0;
}



// the entry of approx method 2
void* APPROX_VC_2(void*)
{
	vector<int> vc;
	vector<pair<int,int>> temp(edge_pair);
	while(!temp.empty())
	{
		pair<int,int> pick = temp.back();
		temp.pop_back();
		int p1 = pick.first;
		int p2 = pick.second;
		vc.push_back(p1);
		vc.push_back(p2);
		vector<pair<int,int>> c;
		for(int i=0;i<temp.size();i++)
		{
			if(temp[i].first!=p1 and temp[i].first!=p2 and temp[i].second!=p2 and temp[i].second!=p1)
			{
				c.push_back(temp[i]);
			}
		}
		temp = c;
	}
	// sort vector
	sort(vc.begin(),vc.end());
	cout<<"APPROX_VC_2: ";
	for(int i=0;i<vc.size();i++)
	{
		if(i!=vc.size()-1)
		{
			cout<<vc[i]<<',';
		}
		else
		{
			cout<<vc[i]<<"\n";
		}
		
	}
return 0;
}


void* CNF_SAT_VC(void*)
{
	int left = 1;
	int right = vertnum;
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
	sort(vc.begin(),vc.end());
	cout<<"CNF_SAT_VC: ";
	for(int i=0;i<vc.size();i++)
	{
		if(i!=vc.size()-1)
		{
			cout<<vc[i]<<',';
		}
		else
		{
			cout<<vc[i]<<"\n";
		}
	}
return 0;
}

static void pclock(clockid_t cid)
{
	struct timespec ts;
	clock_gettime(cid, &ts);
	cout<<"Nano_seconds: "<<ts.tv_nsec<<"\n"; 
}


int main(int argc, char **argv)
{
	pthread_t tids[NUM_THREADS];
	clockid_t cids[NUM_THREADS-1];
	pthread_attr_t attr;
	void *status;
	// set thread joinable 
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	// input thread
	int ret = pthread_create(&tids[0],NULL,input,NULL);
	if (ret!=0)
	{
		cout<<"Error: cannot create input thread,error code: "<<ret<<"\n";
	}
	pthread_attr_destroy(&attr);
	// wait until input thread finished 
	int rc = pthread_join(tids[0], &status);
	if (rc){
		cout << "Error:unable to join," << rc << endl;
		exit(-1);
	}
	// solution 1
	int ret_a1 = pthread_create(&tids[1],NULL,APPROX_VC_1,NULL);
	if (ret_a1!=0)
	{
		cout<<"Error: cannot create solution thread,error code: "<<ret_a1<<"\n";
	}
	int s1 = pthread_getcpuclockid(tids[1], &cids[0]);
	if (s1 != 0)
	{
		cout<<"Error: Cannot compute running time for APPROX_VC_1"<<"\n";
	}
	// solution 2
	int ret_a2 = pthread_create(&tids[2],NULL,APPROX_VC_2,NULL);
	if (ret_a2!=0)
	{
		cout<<"Error: cannot create solution thread,error code: "<<ret_a2<<"\n";
	}
	int s2 = pthread_getcpuclockid(tids[2], &cids[1]);
	if (s2 != 0)
	{
		cout<<"Error: Cannot compute running time for APPROX_VC_2"<<"\n";
	}
	// solution 3
	int ret_a3 = pthread_create(&tids[3],NULL,CNF_SAT_VC,NULL);
	if(ret_a3!=0)
	{
		cout<<"Error: cannot create solution thread, error code: "<<ret_a3<<"\n";
	}
	int s3 = pthread_getcpuclockid(tids[3], &cids[2]);
	if (s3 != 0)
	{
		cout<<"Error: Cannot compute running time for CNF_SAT_VC"<<"\n";
	}
	
	//ouput runing time
	for(int i=0;i<NUM_THREADS-1;i++)
	{
		pclock(cids[0]);
	}
	
	
	pthread_exit(NULL);
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