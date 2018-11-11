#include<vector>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
using namespace std;
int main(int argc, char** argv)
{
	string cvalue;
	string svalue;
	string nvalue;
	string lvalue;
	int cint_value = 20;
	int sint_value = 10;
	int nint_value = 5;
	int lint_value = 5;
	int index;
	int in;
	opterr = 0;
	// expected options are '-s k', '-n k', '-c k' and '-l k'
	while ((in = getopt (argc, argv, "s:n:c:l:")) != -1)
		switch (in)
		{
		case 's':
			svalue = optarg;
			sint_value = atoi(svalue.c_str());
			if(sint_value<2)
			{
			std::cout<<"Error: value for s command should be equal or greater than 2"<<endl;
			return 1;
			}
			break;
		case 'n':
			nvalue = optarg;
			nint_value = atoi(nvalue.c_str());
			if(nint_value<1)
			{
				std::cout<<"Error: value for n command should be equal or greater than 1"<<endl;
				return 1;
			}
			break;
		case 'c':
			cvalue = optarg;
			cint_value = atoi(cvalue.c_str());
			if(cint_value<1)
			{
				std::cout<<"Error: value for c command should be equal or greater than 1"<<endl;
				return 1;
			}
			break;
		case 'l':
			lvalue = optarg;
			lint_value = atoi(lvalue.c_str());
			if(lint_value<5)
			{
				std::cout<<"Error: value for l command should be equal or greater than 5"<<endl;
				return 1;
			}
			break;
		case '?':
			if (optopt == 'c' or optopt == 's' or optopt == 'n' or optopt == 'l')
				cerr << "Error: option -" << optopt
						  << " requires an argument." << std::endl;
			else
				std::cerr << "Error: unknown option: " << optopt << std::endl;
			return 1;
		default:
			return 0;
		}
	if (optind < argc) 
	{
		std::cout << "Found positional arguments\n";
		for (index = optind; index < argc; index++)
			std::cout << "Non-option argument: " << argv[index] << "\n";
	}
	
	std::ifstream urandom("/dev/urandom");
	// check that it did not fail
	if (urandom.fail()) {
		std::cerr << "Error: cannot open /dev/urandom\n";
		return 1;
	}

	unsigned int uimin = std::numeric_limits<unsigned int>::min();
	unsigned int uimax = std::numeric_limits<unsigned int>::max();
	signed int imin = std::numeric_limits<int>::min();
	signed int imax = std::numeric_limits<int>::max();
	// generate num of street
	unsigned int snum = 42;
	urandom.read((char*)&snum, sizeof(int));
	snum = (sint_value-2)*(float)(snum-uimin)/(uimax-uimin)+2;
	
	// generate num of line segment
	unsigned int nnum = 41;
	urandom.read((char*)&nnum, sizeof(int));
	nnum = (nint_value-1)*(float)(nnum-uimin)/(uimax-uimin)+1;
	
	// generate sleep time 
	unsigned int lnum = 40;
	urandom.read((char*)&lnum, sizeof(int));
	lnum = (lint_value-5)*(float)(lnum-uimin)/(uimax-uimin)+5;

int G=0;
	while(true)
	{
		G++;
		if(G>1)
		{
			string t = "\"";
			// remove streets before generate next graph 
			for(int s=0;s<snum;s++)
			{
				t = t+"s";
				string p = "r "+t+"\""+"\n";
				std::cout<<p;
			}
			std::cout<<flush;
		}

		string streetname = "\"";
		for(int s =0; s<snum; s++)
		{
			streetname = streetname+"s";
			string temp = streetname+"\"";
			string street = "a "+temp+" ";
			for(int line =0;line<nnum*2;line++)
			{
				if( line%2==0)
				{
				signed int cnum = 42;
				urandom.read((char*)&cnum, sizeof(int));
				cnum = cint_value*(float)cnum/imax;
				street = street + "("+to_string(cnum)+',';
				}
				else
				{
				signed int cnum = 42;
				urandom.read((char*)&cnum, sizeof(int));
				cnum = cint_value*(float)cnum/imax;
				street = street + to_string(cnum)+") ";
				}
			}
			street = street + "\n";
			std::cout<<street;
		}
		std::cout<<'g'<<"\n";
		std::cout<<flush;
		sleep(lnum);
	}
	// close random stream
	urandom.close();
	return 0;
}