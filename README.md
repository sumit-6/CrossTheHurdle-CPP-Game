# CrossTheHurdle - CPP Game

**It is an interactive game developed using C++ and computer vision. We have used openCV (an open source library) to develop this game. In this game, you have to cross the hurdles without touching them virtually. Your player will be your pen with which you have to pass the hurdles and score points. Tip of your pen will have a small tracing and you have to move your pen in order to avoid hurdles. There will be some gap between the hurdles and we have to pass between these gaps in such a manner that no even the tracing of pen touches the hurdle.**

# Description🧠
Place your pen in front of the camera and start playing the game. Move your pen while keeping it in the frame, so that you don't hit any of the hurdles. On the top left corner, score will appear and it will increase by 1 every second. On getting stuck, game will stop and user will be provided with two options, either you restart the game by pressing any key except for the "Enter key" or exit the game by pressing the "Enter key".  

# Run Demo💻
There is only one cpp file in this project: CrossTheHurdle - Game.cpp
In the `main()` scope, there are two arrays, namely `vector<int> colors[]` and `Scalar markers[]`.

`vector<int> colors[]` will have vector\<int> stored in them. And each of them will have 6 integers stored in them. These six variables represents hue (lower and upper), saturation (lower and upper) and value (lower and upper).

`Scalar markers[]` will have Scalar stored in them. And each of them will store the BGR pixel value for the color of the tracing for a given pen detected.

Steps to run the project :-
1. Set the values for `colors[]` and `markers[]` in the `main()` scope.
2. Pass these two arrays as parameters to the constructor of object created in the `main()` scope.
3. Run the program and start playing.
