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

// recursive function to find a^(p-q) % p faster for fermat test
// given rule (x^2) % n == (x % n)^2 % n
BigUnsigned expMod(int base, BigUnsigned power, BigUnsigned mod) {
   // stopping case
   if (power < 2) return BigUnsigned(base) % mod;

   // recursive case
   BigUnsigned result = expMod(base, power / 2, mod);
   result *= result;
   if (power % 2 != 0) result *= (BigUnsigned(base) % mod);

   return result % mod;
} 
// use Fermat test to check if a number is prime
bool isPrime(BigUnsigned p, int k) {
   // check condition a^(p-1) % p == 1 for k values of a starting from 2 and less than p
   // higher values of k provide more certainty but are slower
   for (int a = 2; a < k + 2; ++a) {
      if (expMod(a, p - 1, p) != 1) return false;
   }
   return true;
}
// radnomly generate a large prime number with at least a certain number of bits
BigUnsigned getPrime(int minBits) { 
   // get to minimum value
   BigUnsigned p(1);
   p <<= minBits;

   // make random modification to value
   p *= rand();

   // adjust until primality test passed
   do {
      p += rand();
      p *= 2;
      p += 1;
   } while (!isPrime(p, 1)); 

   return p;
}

// helper function to determine gcd between e and chi
BigUnsigned GCD(BigUnsigned x, BigUnsigned y) {
   if (x > y) {
      if (y == 0) return x;
      return GCD(x % y, y);
   }
   if (x == 0) return y;
   return GCD(x, y % x);
}
// get public key component
BigUnsigned getE(BigUnsigned chi) { 
   // given e as any odd integer with no common factors with chi (other than 1)
   BigUnsigned e(1);
   return e;
}
// get private key component
BigUnsigned getD(BigUnsigned e, BigUnsigned chi) { 
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

      srand(time(0));

      std::fstream fileStream;

      // get inital parameters
      BigUnsigned p = getPrime(512);       
      std::cout << "p: " << p << std::endl;
      BigUnsigned q = getPrime(512);
      std::cout << "q: " << q << std::endl;
      std::cout << GCD(p, q) << std::endl;
      
      // save primes to file
      fileStream.open("p_q.txt", std::ios::out);
      fileStream << p << std::endl;
      fileStream << q << std::flush;
      if (fileStream) std::cout << "p and q saved to file" << std::endl;
      else std::cout << "file output failed for p and q" << std::endl;
      fileStream.close();

      // get key parameters
      BigUnsigned chi = (p - 1) * (q - 1);
      BigUnsigned e = getE(chi);
      BigUnsigned d = getD(e, chi);      
      BigUnsigned n = p * q;

      // save keys to files
      fileStream.open("e_n.txt", std::ios::out);
      fileStream << e << std::endl;
      fileStream << n;
      fileStream.close();

      fileStream.open("d_n.txt", std::ios::out);
      fileStream << d << std::endl;
      fileStream << n;
      fileStream.close();
      
	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}
