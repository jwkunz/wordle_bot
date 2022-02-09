# wordle_bot
A bot for playing World and giving suggestions based on the maximum entropy method.

I eventually will give it a GUI, but as of right now the main_XXXX.cpp files can be compiled for a terminal based expierence.

- main_game_engine.cpp : A terminal Wordle implementation without suggestions (no cheating)
- main_game_engine_recomendations.cpp : A terminal Wordle implementation with suggestions (cheating)
- main_recomendation_engine.cpp : A terminal tool to give suggestions based on a external Wordle game (cheating)

For example, to compile into the target directory:

  g++ -std=c++17 -o3 ${file} -o ${workspaceFolder}\\target\\${fileBasenameNoExtension}.exe -fopenmp


