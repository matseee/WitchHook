# WitchHook
simple internal Witcher 3 cheat written in c++. After the DLL is injected into the game process, a new thread is created. In this thread there is an endless loop in which the cheat logic is executed. Since the cheat was loaded as a DLL into the game, it is possible to directly access the memory area of the game.

The gold cheat is simply a reading of the current gold value, which is then increased by 500. The unlimited vitality cheat works by [hooking](http://jbremer.org/x86-api-hooking-demystified/) the original vitality reduction function and rewriting the value reduction in assembler. The teleport hack first reads the position of the waymarker and then writes the player's position to these coordinates.

- F1: add 500 gold
- F2: unl. vitality
- F3: teleport to waypoint

## Tools used
- [CheatEngine](https://cheatengine.org/)
- [Visual Studio 2022](https://visualstudio.microsoft.com/de/)

## Credits
- [guidedhacking.com](www.guidedhacking.com)