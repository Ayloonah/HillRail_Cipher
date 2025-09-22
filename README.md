# HillRail Encryption Program  

HOW TO USE:  
- Make sure you are on a system with **gcc** installed.  
- Compile the program with the following command:
`gcc -O2 -std=c11 -o hillrail hillrail.c`
- Run the program from the command line in this format:  
`./hillrail encrypt key.txt plain.txt <depth>`
- The arguments are as follows:
  - `hillrail` is the name of the program
  - `encrypt` is an argument that must be included
  - `key.txt` is the Hill cipher key matrix file
  - `plain.txt` is the plaintext file
  - `<depth>` is the integer Rail Fence depth (> 0)
- The files should be formatted as follows:
  - `key.txt` should have an integer **n** indicating the dimension of the matrix on the first line followed by the matrix itself, distributed over **n** number of lines containing **n** number of integers
  - `plain.txt` should have whatever text you wish to have encrypted

Hello,  

For my **Security in Computing** class at the University of Central Florida, I was tasked with implementing a program that performs a two-stage encryption process. The program first encrypts a message using the Hill cipher, which uses matrix multiplication and modulo 26 to substitute blocks of text with different characters, and then applies a Rail Fence transposition cipher, which involves writing the text in a wave pattern across multiple lines and reading it line by line, with a given depth.

Here were the instructions we were given: 
- The program needed to accept exactly four parameters in this order: the literal command encrypt, path to the key file, path to the plaintext file, and the Rail Fence depth d (integer >= 2).
- We needed to print four sections to the terminal: Key matrix, Plaintext, Ciphertext, and Depth. Each section had to be titled, and a new line needed to separate each one of them. 
  - **Key matrix**: The matrix had to be printed as parsed, meaning it had to have **n** number of lines with **n** number of integers each, separated by tabs.
  - **Plaintext**: Before printing it, we needed to remove any whitespace and unwanted characters from the plaintext. We then needed to make sure it was all in uppercase characters, and we had to print it 80 characters per line.
  - **Ciphertext**: After applying both ciphers to the paintext, we needed to print it once more, still 80 uppercase characters per line. 
  - **Depth**: On the same line as the title, we had to print the integer value for the Rail Fence depth.
- We were free to use as many or as few functions as we wanted, and use either static or dynamic memory allocation. As long as the output matched what was expected exactly, we would have full marks.

How I tackled this assignment: 
- I first wrote function prototypes for what needed to be done, including comments on what each function should do. 
- I then worked on a function at a time, testing each one as I went to ensure that it worked as expected. 
- I did not want to work on this with minimum efforts only; I used dynamic memory allocation where applicable, did not make use of global variables, split my code into functions instead of having everything in main.
- Once my base code was written, I tested it against a few of the cases, only running the provided testing shell script once I was confident that the program was working as intended. 
- The shell script picked up on issues my terminal missed, so I kept debugging until I passed every case. 

Here is my takeaway from this project:

**Positives:**  
- I have learned how to write code that accurately applied matrix multiplication modulo 26 for the Hill cipher, something that did not come to me intuitively.
- I have gained practice with file parsing, formatted output, and string manipulation.
- Successfully passing all of the test cases the code was thrown against was extremely rewarding. 

**Negatives:**  
- Making sure that the output format matched the exact specifications took a lot more time than expected. For example, when I thought my code was working well, the shell script found null characters that weren't visible without converting the text file to binary. This is something I had not accounted for. I have therefore learned that I will always need to allocate more time in case of potential unexpected bugs.

Overall, I am happy with the final result and the experience I gained in both programming and cryptography concepts!
