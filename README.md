# Compiler Design

Mini Compiler for Python 3.x with support for **FOR,WHILE constructs and other basic syntax**.

Pictures present in the report.

## Running the code

We know that there are too many folders in the repo. Go to [FINAL](https://github.com/sreyansb/Compiler_Design/tree/master/FINAL) and run `chmod +x work.sh`. Then simply run `work.sh` and you will get the outputs.

The parser file is [`parser_file_with_values.y`](https://github.com/sreyansb/Python_Mini_Compiler/blob/master/FINAL/parser_file_with_value.y) and the lexer file is [`lex_file.l`](https://github.com/sreyansb/Python_Mini_Compiler/blob/master/FINAL/lex_file.l)
The tests file are in [`Code_Optimization/tests`](https://github.com/sreyansb/Python_Mini_Compiler/tree/master/FINAL/Code_Optimization/tests) folder of the [`FINAL`](https://github.com/sreyansb/Compiler_Design/tree/master/FINAL) folder.
On the quads (`quads.tsv`) generated in the [`Code_Optimization/non_optimized`](https://github.com/sreyansb/Python_Mini_Compiler/tree/master/FINAL/Code_Optimization/non_optimized) folder, you can run the python optimization scripts. Just go through the code of each of the optimization scipts, you will understand!

## Phases handled:
* Lexical Phase
* Syntactic Phase
* Semantic Phase
* ICG generation with TAC in quads format
* Code Optimization
  * Common Sub-Expression Elimination
  * Dead Code Elimination
  * Constant Folding and Propagation
  * Loop Invariant Code Movement

## Acknowledgement
* https://github.com/sharanyavenkat25/CD-Project : The Grammar for the language
* https://github.com/craterkamath/Python_Mini_Compiler : Amazing way to get the indentation!
* https://github.com/anagha1999/CD-Project : Actions and Code Optimization part

## Issues
* The grammar is somewhat not right because it cannot handle multiple loops (but it can handle nested loops!).
* All pathways haven't been tested, some issues might crop up: would love to hear them!

Feel free to use the code (as long as you are not caught by your teachers!). I don't think I can stop you from using a public repo either ways!

<div align="center">

Made as a part of Compiler Design Subject for 6th semester by [Revanth](https://github.com/RevanthBabuPN), [Sahith](https://github.com/Sahith02) and [Sreyans](https://github.com/sreyansb).

 </div>
