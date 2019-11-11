README

    This is a program that can compress data files using the LZW (Lempel-Ziv-
    Welch) method in which an expanded range of encodings is used, allowing 
    multi-character sequences detected in the message to each be mapped to a 
    single code.


    To execute this program:

        A) Run the executable "lzw435" in the terminal with two command line 
        arguments:

            1) A character specifying the mode for running the program:

                - "c" specifies compression, where the program takes the source 
                data and creates a compressed message.

                - "e" specifies expansion, where the programs takes the 
                compressed data recreates the original representation.

            2) The name of the file containing the message to run the program 
            on, either the source message or the encoded message.

        B) If the executable is not present, compile the program by running 
        "make" or "make all"

        C) A modified version of this program is contained in the executable 
        "lzw435m", which accepts the same command line arguments as "lzw435" 
        and can be compiled through "make m".


    Upon execution, this program will:

        A) In compressing mode:

            1) Initialize a table of 12-bit codes with the first 256 positions 
            occupied by individual ASCII characters

            2) Open the source file and create a stream to the compressed file

            3) For each byte in the file:

                i) Add the byte to the existing recorded sequence, which starts 
                out empty

                ii) If the new sequence isn't mapped to a code: 

                    1) Write the binary code for the existing sequence into the 
                    compressed file 

                    2) Map the new sequence to the next available code if 
                    there's still room in the table

                    3) Reset the sequence to contain only the current byte

            4) Close the filestream once every byte in the source file has been 
            handled

        B) In expanding mode:

            1) Initialize a table of 12-bit codes with the first 256 
            positions occupied by individual ASCII characters

            2) Open the compressed file and create a stream to the 
            reconstructed file

            3) Read bytes from the file and keep track of each grouping of 12 
            bits (one full byte + half a byte) 

            4) For every 12 bits:

                i) Find the sequence S1 corresponding to these 12 bits, as well 
                as sequence S2 corresponding to the next 12 bits

                ii) Record S1 concactenated with the first letter of S2 as the 
                sequence corresponding to the next available code in the table

                iii) If the code of S2 hasn't been recorded yet, S2 would have 
                to equal S1 concactenated with the first letter of itself.

            5) Close the filestream once there's not enough bits left to form a
            12-bit sequence


        In the modified version, instead of using fixed 12-bit codes, 256 
        9-bit codes are used, followed by 512 10-bit codes, 1024 11-bit codes, 
        and so on. Everything else remains the same.


CONTACT

    This implementation was written by Thomas Li (thl13@zips.uakron.edu)