# Color Palettes

## Overwiev
Programme is a recreation of classic japanese game Sokoban with custom-made textures. It allows the user to create their own maps.

![Game window](projectImages/game.png)

## Features
1. Game window is resizeable  
2. Parked boxes are marked as done
3. Counter keeps track of your time

### Setup
Before downloading make sure you have cmake installed. To run the programme, run commands:

```
git clone https://github.com/zagalahubert/Sokoban.git
cd SokobanGame
cmake -B build -A x64
cmake --build build --config Release
cd build/Release
.\SokobanGame.exe    
```

## Technologies
Technologies used:
1. SFML
2. C++
3. CMake
4. Roboto font (Apache 2.0)

## Coming...
Incorporating ImGui to allow the user to load in their own map easier.

