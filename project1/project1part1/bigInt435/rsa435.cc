// You need to complete this program for a part of your first project.

// When run, this program will:
// 1) Generate two large prime numbers p and q
// 2) Save p and q to a file named p_q.txt
// 4) Generate encryption key pair (e, n) and decryption key pair (d, n) for n = p* q
// 5) Save (e, n) and (d, n) to files name e_n.txt and d_n.txt, respectively

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <fstream>


// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

BigUnsigned getPrime(int minBits) { // radnomly generate a large prime number with at least a certain number of bits
   // get to minimum value
   BigUnsigned p(1);
   p <<= minBits;

   // make random modification to value

   // adjust until primality test passed

   return p;
}
bool isPrime(BigUnsigned p, int k) { // use Fermat test to check if a number is prime
   // check k random numbers between 2 and p and return true if all 
   // of them pass the condition of i^(p - 1) % p == 1
   // higher k is more accurate but slower
   return true;
}

BigUnsigned getE(BigUnsigned chi) { // get public key component
   // given e as any odd integer with no common factors with chi (other than 1)
   BigUnsigned e(1);
   return e;
}

BigUnsigned getD(BigUnsigned e, BigUnsigned chi) { // get private key component
   // given d as the integer where de % chi = 1
   BigUnsigned d(1);
   return d;
}

int main(){
	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {
		      
      /*std::cout << "a couple of test cases for 3460:435/535 Algorithms!!!\n";
      BigUnsigned big1 = BigUnsigned(1);
      for (int i=0;i<400;i++) {
         big1 = big1*10 +rand();
      }
      std::cout << "my big1 !!!\n";
      std::cout << big1;
      BigUnsigned big2 = BigUnsigned(1);
      for (int i=0;i<400;i++) {
         big2 = big2*10 +rand();
      }
      std::cout << "my big2 !!!\n";
      std::cout << big2;
      std::cout << "my big3 = big1*big2 !!!\n";
      BigUnsigned big3 = big1*big2;
      std::cout <<big3;
      std::cout << "my big3/big2 !!!\n";
      std::cout <<big3/big2;*/

      std::ofstream fileStream;

      // get inital parameters
      BigUnsigned p = getPrime(512); 
      BigUnsigned q = getPrime(512);
      
      // save primes to file
      fileStream.open("p_q.txt", std::fstream::out);
      fileStream << p << " " << q;
      fileStream.close();

      // get key parameters
      BigUnsigned chi = (p - 1) * (q - 1);
      BigUnsigned e = getE(chi);
      BigUnsigned d = getD(e, chi);      
      BigUnsigned n = p * q;

      // save keys to files
      fileStream.open("e_n.txt");
      fileStream << e << " " << n;
      fileStream.close();

      fileStream.open("d_n.txt");
      fileStream << d << " " << n;
      fileStream.close();
      
	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}
