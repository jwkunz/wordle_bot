# wordle_bot
A bot for playing World and giving suggestions based on the maximum entropy method.

I eventually will give it a GUI, but as of right now the main_XXXX.cpp files can be compiled for a terminal based expierence.

- main_game_engine.cpp : A terminal Wordle implementation without suggestions (no cheating)
- main_game_engine_recomendations.cpp : A terminal Wordle implementation with suggestions (cheating)
- main_recomendation_engine.cpp : A terminal tool to give suggestions based on a external Wordle game (cheating)
- main_entropy_list.cpp : Will evalauate a word list and compute the entropy for each word (to learn good guesses)

For example, to compile into the target directory with the g++ toolchain:

  g++ -std=c++17 -o3 ${file} -o ${workspaceFolder}\\target\\${fileBasenameNoExtension}.exe
  
The data directory contains .txt files with lists of words in ASCII and separated by a newline.  
Data can be added here, adjusted in the main_XXXX.cpp file, and recompiled.

For reference a list of good first guess words are:

- Entropy: 9.6177, Word: sense
- Entropy: 9.37056, Word: seeds
- Entropy: 9.09607, Word: seems
- Entropy: 8.7247, Word: seeks
- Entropy: 8.5668, Word: stats
- Entropy: 8.53366, Word: sells
- Entropy: 8.51993, Word: tests
- Entropy: 8.07233, Word: trees
- Entropy: 8.02845, Word: ellen
- Entropy: 7.98284, Word: lease
