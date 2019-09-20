/***
   prepared for CS435 Project 1 part 2
**/

#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

// function to run encryption function (m^e) % n faster for large values of e and n
// given (a^2) % n = (a % n)^2 % n
BigUnsigned expMod(BigUnsigned base, BigUnsigned power, BigUnsigned mod) {
   // stopping case
   if (power < 2) return base % mod;

   // recursive case
   BigUnsigned result = expMod(base, power / 2, mod);
   result *= result;
   if (power % 2 != 0) result *= (BigUnsigned(base) % mod);

   return result % mod;
} 
 
int main(int argc, char *argv[])
{
   //demonstrating how sha256 works
   std::string input = "testing";
   std::string output1 = sha256(input);
   std::cout << "sha256('"<< input << "'):" << output1 << "\n";
   
   //demo bigInt works here
   BigUnsigned a = stringToBigUnsigned("124338907642982722929222542626327282");
   BigUnsigned b = stringToBigUnsigned("124338907642977775546469426263278643");
   std::cout << "big a = " <<a<<"\n";
   std::cout << "big b = " <<b<<"\n";
   std::cout << "big a*b = " <<a*b<<"\n";

   //Second part of your project starts here
   if (argc != 3 || (argv[1][0]!='s' && argv[1][0]!='v')) 
      std::cout << "wrong format! should be \"a.exe s filename\"";
   else {
      std::string filename = argv[2];
      
            
      //read the file
      std::streampos begin,end;
      std::ifstream myfile (filename.c_str(), std::ios::binary);
      begin = myfile.tellg();
      myfile.seekg (0, std::ios::end);
      end = myfile.tellg();
      std::streampos size = end-begin;
      //std::cout << "size of the file: " << size << " bytes.\n"; //size of the file
      
      myfile.seekg (0, std::ios::beg);
      char * memblock = new char[size];
      myfile.read (memblock, size); //read file; it's saved in the char array memblock
      myfile.close();
      
      std::string copyOFfile = filename+".Copy"; 
      std::ofstream myfile2 (copyOFfile.c_str(), std::ios::binary);
      myfile2.write (memblock, size); //write to a file
      myfile2.close();
      
      //std::cout<<memblock;
        
      
      try {
         // generate the SHA hash from the file contents
         std::string hashStr = sha256(memblock);
         std::cout << "\nHash from file: [" << hashStr << "]" << std::endl;

         // file containing hash signature
         std::string signFilename = filename + ".signature";

         if (argv[1][0]=='s') {
            std::cout << "\n"<<"Need to sign the doc.\n";
            //.....

            // get decryption key pair (d, n)
            std::string dStr, nStr;
            std::ifstream keyFile("d_n.txt");
            getline(keyFile, dStr);
            getline(keyFile, nStr);
            keyFile.close();

            BigUnsigned d = stringToBigUnsigned(dStr);
            BigUnsigned n = stringToBigUnsigned(nStr);

            // decrypt the hash to generate the signature
            std::vector<BigUnsigned> signature;
            std::cout << "Generating signature..." << std::endl;
            for (char c : hashStr) {
               // convert each character to ASCII code to run the encryption function
               // of (m^d) % n given value m where m < n
               signature.push_back(expMod(BigUnsigned(static_cast<int>(c)), d, n));
            }

            /* driver code to test encryption key
            std::string eStr;
            keyFile.open("e_n.txt");
            getline(keyFile, eStr);
            keyFile.close();
            BigUnsigned e = stringToBigUnsigned(eStr);

            std::cout << "\nhash from encryption: ";
            for (BigUnsigned i : signature) {
               std::cout << static_cast<char>(expMod(i, e, n).toInt());
            }
            std::cout << std::endl;
            */

            // save the signature
            std::cout << "Saving signature..." << std::endl;
            std::ofstream signFile(signFilename.c_str(), std::ios::out);
            for (BigUnsigned i : signature) {
               signFile << i << std::endl;
            }
            signFile.close();
            std::cout << "Saved to file " << signFilename << std::endl;
         }
         else {
            std::cout << "\n"<<"Need to verify the doc.\n";
            //.....

            // get encryption key pair (e, n)
            std::string eStr, nStr;
            std::ifstream keyFile("e_n.txt");
            getline(keyFile, eStr);
            getline(keyFile, nStr);
            keyFile.close();

            BigUnsigned e = stringToBigUnsigned(eStr);
            BigUnsigned n = stringToBigUnsigned(nStr);

            // get the signature from the file
            std::cout << "Getting signature..." << std::endl;
            std::vector<BigUnsigned> signature;

            std::ifstream signFile(signFilename.c_str(), std::ios::in);
            if (!signFile) throw("Signature file not found. Try running the program in signing mode first.\n");
            while (!signFile.eof()) {
               std::string s;
               getline(signFile, s);
               if (s != "" && s != " ") signature.push_back(stringToBigUnsigned(s));
            }
            signFile.close();

            // encrypt the existing signature to convert it back to a hash
            std::cout << "Getting hash..." << std::endl;
            std::string newHash = "";
            for (BigUnsigned i : signature) {
               newHash += static_cast<char>(expMod(i, e, n).toInt());
            }

            // compare the two hashes
            std::cout << "Hash from encryption: [" << newHash  << "]" << std::endl;
            if (newHash == hashStr) {
               std::cout << "The hashes match. The document is authentic." << std::endl;
            }
            else {
               std::cout << "The hashes are different. The document has been modified since signing." << std::endl;
            }
         }
      }
      catch (const char* c) {
         std::cerr << c << std::endl;
      }

      delete[] memblock;
    }
    return 0;
}