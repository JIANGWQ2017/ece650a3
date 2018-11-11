// an example of reading random numbers from /dev/urandom
// https://stackoverflow.com/questions/35726331/c-extracting-random-numbers-from-dev-urandom
#include <iostream>
#include <fstream>
#include<limits>
int main(void) {

unsigned int imin = std::numeric_limits<unsigned int>::min(); // minimum value
unsigned int imax = std::numeric_limits<unsigned int>::max();

	// open /dev/urandom to read
	std::ifstream urandom("/dev/urandom");

	// check that it did not fail
	if (urandom.fail()) {
		std::cerr << "Error: cannot open /dev/urandom\n";
		return 1;
	}

	// read a random 8-bit value.
	// Have to use read() method for low-level reading
	char ch = 'a';
	urandom.read(&ch, 1);
	// cast to integer to see the numeric value of the character
	std::cout << "Random character: " << (unsigned int)ch << "\n";
	
	// read another 8-bit value
	urandom.read(&ch, 1);
	std::cout << "Random character: " << (unsigned int)ch << "\n";

	// read a random unsigned int
	unsigned int num = 42;
	urandom.read((char*)&num, sizeof(int));
	std::cout<<"num"<<num<<"\n";
	num = 8*(float)(num-imin)/(imax-imin)+2;
	std::cout << "Random character: " << num << "\n";
	std::cout<<"imin"<<imin<<"imax"<<imax<<"\n";
	
	
	// close random stream
	urandom.close();
	return 0;
}
