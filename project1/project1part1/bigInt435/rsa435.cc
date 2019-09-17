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
// given rule (a * b) % n = (a % n) * (b % n) % n
// and thus that (x^2) % n == (x % n)^2 % n
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
   BigUnsigned e(rand());

   do {
      e += rand();
      if (e % 2 == 0) e += 1;
   } while (GCD(e, chi) > 1);

   return e;
}
// use Extended Euclidean algorithm to get private key component
BigUnsigned getD(BigUnsigned e, BigUnsigned chi) { 
   // given d as the integer where de % chi == 1 
   // or (d * e) + (r * chi) == 1
   BigInteger d(0), newD(1);
   BigInteger r(chi), newR(e);
   BigInteger quotient(0), temp(0);

   while (newR != 0) {
      quotient = r / newR;

      temp = d;
      d = newD;
      newD = temp - quotient * newD;

      temp = r;
      r = newR;
      newR = temp - quotient * newR;
   }

   if (d < 0) d += chi;
   return d.getMagnitude();
}

template <typename B1, typename B2>
void saveToFile(std::string fileName, B1 x, B2 y) {
   try {
      std::ofstream fileStream(fileName.c_str(), std::ios::out);
      if (!fileStream) {
         throw(std::runtime_error("failed to open file"));
      }

      fileStream << x << std::endl;
      fileStream << y << std::flush;
      fileStream.close();
   }
   catch(std::runtime_error err) {
      std::cerr << "[exception] " << err.what() << std::endl;
   }
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

      // get inital parameters
      std::cout << "\ncalculating p..." << std::endl;
      BigUnsigned p = getPrime(512);       
      std::cout << "p: " << p << std::endl;

      std::cout << "\ncalculating q..." << std::endl;
      BigUnsigned q = getPrime(512);
      std::cout << "q: " << q << std::endl;
      
      // save primes to file
      std::cout << "\nsaving p and q..." << std::endl;
      saveToFile("p_q.txt", p, q);

      // get key parameters
      BigUnsigned chi = (p - 1) * (q - 1);

      std::cout << "\ncalculating e..." << std::endl;
      BigUnsigned e = getE(chi);
      std::cout << "e: " << e << std::endl;

      std::cout << "\ncalculating d..." << std::endl;
      BigUnsigned d = getD(e, chi);      
      std::cout << "d: " << d << std::endl;

      BigUnsigned n = p * q;
      std::cout << "\nn: " << n << std::endl;

      // save keys to files
      std::cout << "\nsaving e and n..." << std::endl;
      saveToFile("e_n.txt", e, n);

      std::cout << "\nsaving d and n..." << std::endl;
      saveToFile("d_n.txt", d, n);
      
	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}
