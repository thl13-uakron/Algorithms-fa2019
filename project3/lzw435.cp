#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector> 
#include <unordered_map>
#include <sys/stat.h>

/*
  This code is derived from LZW@RosettaCode for UA CS435 
*/ 


// constant parameters
const int BYTESIZE = 8; // minimum number of bits that be extracted from the file at a time
const int BITS = 12; // size of each code
const int ASCII_SIZE = 256; // number of ASCII characters


// representation for bit sequences
typedef std::string bitfield_t;


// helper functions for handling bit representations

bitfield_t int2BinaryString(int c, int cl) {
      bitfield_t p = ""; //a binary code string with code length = cl
      int code = c;
      while (c>0) {         
       if (c%2==0)
            p="0"+p;
         else
            p="1"+p;
         c=c>>1;   
      }
      int zeros = cl-p.size();
      if (zeros<0) {
         std::cout << "\nWarning: Overflow. code " << code <<" is too big to be coded by " << cl <<" bits!\n";
         p = p.substr(p.size()-cl);
      }
      else {
         for (int i=0; i<zeros; i++)  //pad 0s to left of the binary code if needed
            p = "0" + p;
      }
      return p;
}
int binaryString2Int(bitfield_t p) {
   int code = 0;
   if (p.size()>0) {
      if (p.at(0)=='1') 
         code = 1;
      p = p.substr(1);
      while (p.size()>0) { 
         code = code << 1; 
       if (p.at(0)=='1')
            code++;
         p = p.substr(1);
      }
   }
   return code;
}

// get number of bits in sequence
int get_bit_length(bitfield_t bits) {
  return bits.length();
}
// get certain number of bits on left side of sequence
bitfield_t get_left_bits(bitfield_t bits, int numBits) {
  return bits.substr(0, numBits);
}
// get certain number of bits of right side of sequence
bitfield_t get_right_bits(bitfield_t bits, int numBits) {
  return bits.substr(bits.length() - numBits);
}
// get bit sequence from concactenating two bit sequences
bitfield_t append_bits(bitfield_t left, bitfield_t right) {
  return left + right;
}
// read the smallest possible number of bits from a file stream to a bitfield
bitfield_t read_byte(std::ifstream &infileStream) {
  unsigned char c;
  infileStream >> c;
  return int2BinaryString(static_cast<int>(c), BYTESIZE);
}
// write the smallest possible number of bits from a bitfield to a file stream
void write_byte(bitfield_t bits, std::ofstream &outfileStream) {
  outfileStream << static_cast<unsigned char>(binaryString2Int(get_left_bits(bits, BYTESIZE)));
}

// given the name of a source file and an output file
// use LZW to write the compressed contents of the input file to the output file
void compress_file(std::string infileName, std::string outfileName) {
  
    std::ifstream infileStream(infileName);
    std::ofstream outfileStream(outfileName);

    // number of bits per code
    int codeLength = BITS;

    // list of sequence-to-code mappings
    std::unordered_map<std::string, int> mappings;
    for (int i = 0; i < ASCII_SIZE; ++i) {
      std::string s = "";
      s += static_cast<unsigned char>(i);
      mappings[s] = i;
    }
    // number of mappings recorded so far
    int numMappings = ASCII_SIZE;

    // hold sequence of characters read from input file
    std::string charSequence = ""; // prior to current character
    std::string newCharSequence; // including current character

    // hold sequence of bits to write to output file
    bitfield_t bitSequence = "";

    // hold last character read from file
    unsigned char currentChar;

    // bytes in file and number of bytes recorded so far
    struct stat infileStat;
    stat(infileName.c_str(), &infileStat);
    long numBytes = infileStat.st_size;
    long bytesRead = 0;

    // read and compress contents of input file
    while (bytesRead < numBytes) {
      // record character
      infileStream >> currentChar;

      // check for mapping of character sequence formed by new character appended to existing sequence
      newCharSequence = charSequence;
      newCharSequence += currentChar;
      if (mappings.find(newCharSequence) == mappings.end()) {
        // record bit code for existing character sequence into output file
        bitSequence = append_bits(bitSequence, int2BinaryString(mappings[charSequence], codeLength));

        // map new sequence
        if (numMappings < (1 << codeLength)) {
          mappings[newCharSequence] = numMappings;
          ++numMappings;
        }

        // reset character sequence
        charSequence = "";
        charSequence += currentChar;
      }
      else {
        // add character to existing sequence
        charSequence = newCharSequence;
      }

      // write bits to output file in smallest possible units
      while (get_bit_length(bitSequence) >= BYTESIZE) {
        write_byte(bitSequence, outfileStream);
        bitSequence = get_right_bits(bitSequence, get_bit_length(bitSequence) - BYTESIZE);
      }

      ++bytesRead;
    }

    // record code for remaining character sequence to file if needed
    if (get_bit_length(charSequence) > 0) 
      bitSequence = append_bits(bitSequence, int2BinaryString(mappings[charSequence], codeLength));

    // write remaining bits to output file, add bits if needed to form a full byte
    while (get_bit_length(bitSequence) > 0) {
      if (get_bit_length(bitSequence) < BYTESIZE) {
        bitSequence = append_bits(bitSequence, int2BinaryString(0, BYTESIZE - get_bit_length(bitSequence)));
      }
      write_byte(bitSequence, outfileStream);
      bitSequence = get_right_bits(bitSequence, get_bit_length(bitSequence) - BYTESIZE);
    }

    infileStream.close();
    outfileStream.close();
}

// give the name of an input file containing LZW-compressed data, and an output file
// use LZW to decompress the input file contents and write them to the output file
void decompress_file(std::string infileName, std::string outfileName) {
  
    std::ifstream infileStream(infileName);
    std::ofstream outfileStream(outfileName);

    if (!infileStream) {
      throw std::runtime_error("Failed to open file " + infileName);
    }

    // number of bits per code
    int codeLength = BITS;

    // list of code-to-sequence mappings
    std::vector<std::string> mappings;
    for (int i = 0; i < ASCII_SIZE; ++i) {
      mappings.push_back("");
      mappings[i] += static_cast<unsigned char>(i);
    }

    // hold sequence of bits from file stream
    // containing the bits that make up the code
    bitfield_t bitSequence = "";

    // hold current code and decompressed word
    int currentCode;
    std::string currentWord;

    // hold next code and decompressed word
    int nextCode;
    std::string nextWord;

    // bytes in file and number of bytes recorded so far
    struct stat infileStat;
    stat(infileName.c_str(), &infileStat);
    long numBytes = infileStat.st_size;
    long bytesRead = 0;

    // get first code from file
    while (bitSequence.length() < codeLength) {
      // extract bytes until bit sequence is long enough to contain code
      bitSequence = append_bits(bitSequence, read_byte(infileStream));
      ++bytesRead;
    }
    // retrieve code from bit sequence, remove portion of sequence containing code
    currentCode = binaryString2Int(get_left_bits(bitSequence, codeLength)); 
    bitSequence = get_right_bits(bitSequence, get_bit_length(bitSequence) - codeLength);
    currentWord = mappings[currentCode]; // get mapping of code
    outfileStream << currentWord; // put first character into output file
    // std::cout << currentCode << " " << currentWord << std::endl;

    // get remaining codes from file
    while (bytesRead < numBytes) {
      while (bitSequence.length() < codeLength) {
        // extract bytes until bit sequence is long enough to contain code
        bitSequence = append_bits(bitSequence, read_byte(infileStream));
        ++bytesRead;
      }
      // retrieve code from bit sequence, remove portion of sequence containing code
      nextCode = binaryString2Int(get_left_bits(bitSequence, codeLength)); 
      bitSequence = get_right_bits(bitSequence, get_bit_length(bitSequence) - codeLength);

      // get mapping of code
      if (nextCode >= mappings.size()) {
        // special case
        nextWord = currentWord + currentWord.at(0);
      }
      else {
        // normal case
        nextWord = mappings[nextCode];
      }

      // record new mapping
      if (mappings.size() < (1 << codeLength)) {
        mappings.push_back(currentWord);
        mappings[mappings.size() - 1] += nextWord.at(0);
      }

      currentWord = nextWord; // record sequence for use in next iteration
      outfileStream << currentWord; // put sequence into output file
      // std::cout << nextCode << " " << nextWord << std::endl;
    }

    // for (int i = ASCII_SIZE; i < mappings.size(); ++i) std::cout << i << " " << mappings[i] << std::endl;

    infileStream.close();
    outfileStream.close();
}
 


int main(int argc, char* argv[]) {
  /*
  std::vector<int> compressed;
  compress("AAAAAAABBBBBB", std::back_inserter(compressed));
  copy(compressed.begin(), compressed.end(), std::ostream_iterator<int>(std::cout, ", "));
  std::cout << std::endl;
  std::string decompressed = decompress(compressed.begin(), compressed.end());
  std::cout << decompressed << std::endl;
  
  binaryIODemo(compressed);
  */

  if (argc < 3) {
    std::cout << "Not enough command line arguments detected. Check readme.txt for execution instructions.\n";
  }
  else {

    std::string infileName = argv[2];
    std::string outfileName = "";

    if (strcmp(argv[1], "c") == 0 || strcmp(argv[1], "C") == 0) {
      outfileName = infileName + ".lzw";
      std::cout << "Compressing file " << infileName << std::endl;
      compress_file(infileName, outfileName);
    }
    else {
      outfileName = infileName;
      outfileName.replace(outfileName.find(".lzw"), 4, "2");
      std::cout << "Expanding file " << infileName << std::endl;
      decompress_file(infileName, outfileName);
    }

    std::cout << "Results written to " << outfileName << std::endl;

  }
  
  return 0;
}
