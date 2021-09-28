# the-tunnelman-game-cplusplus
A real-time game where the player has to dig through earth to find oil and run away from protestors who can kill the player. The player can pick up helpful items, advance to the next level, and accumulate a high score.

<br/>

**Project context:** <br/>

This was a school project where I had to program the game characters and objects for The TunnelMan Game. 

This was the first project of significantly large size that I wrote in my life. There was a quick deadline for this project so a lot of the code is not clean and minimal. I aim to write code of better quality in my future projects.

<br/>

**Main files and folders:** <br/>

These files are found in the TunnelMan folder of the root of the project.

  - The graphics and sound effects of the game characters and objects during game play can be found in the Assets and irrKlang folders.

  - Actor.h, Actor.cpp, StudentWorld.h, and StudentWorld.cpp were the files in which the classes for the game characters, game objects, and the game universe that they interact with were written. These are where most of my work was done.

  - The purpose of the code in the remaining files is as follows: <br/>
    - GameConstants.h (just game constants)
    - GraphObject.h (Controls how a game object looks on the screen)
    - SpritManager.h (Also controls how a game object looks on the screen)
    - SoundFX.h (Controls the sounds during game play)
    - freeglut.h (Includes the freeglut library into this project)
    - freeglut_ext.h (Includes non-GLUT-compatible extensions to the freeglut library into this project)
    - freeglut_std.h (Includes the GLUT-compatible part of the freeglut library into this project)
    - GameController.h (Creates a class for the game controller, that does things like start and end the game, and draw the game objects at the beginning, etc)
    - GameController.cpp (Contains the methods of the game controller class)
    - GameWorld.h (Creates a class that controls basic things about the game universe like game stats, more advanced features of the universe were implemented in StudentWorld.h and StudentWorld.cpp)
    - GameWorld.cpp (Contains the methods of the class from GameWorld.h)
    - main.cpp (Starts the program and runs the game)

  - All files other than Actor.h, Actor.cpp, StudentWorld.h, and StudentWorld.cpp were provided by the instructor of the course. Aside from minor modifications to some functions and class methods, only the values in these files were modified so as to match instructor's specifications.

<br/>

**Other files and folders:** <br/>

All other files in the repository that were not previously mentioned are related to the Microsoft Visual Studio configuration and the other external libraries that were used in this game. It is important that these remain unchanged after cloning if you would like to test this program.

<br/>


**The classes in Actor.h and Actor.cpp:** <br/>

These files are where all the game objects and game items (i.e. The Actors) were created.

The classes were designed according to the specifications provided by the instructor.

Inheritance in the Actor classes was designed as follows:
1. There are many types of GameObjects (all game objects are graph objects):
    - Earths
    - Boulders
    - The TunnelMan (This is the player. There is only one TunnelMan throughout the game)
    - Protestors (These are the enemies that can kill the player)

2. There is a more specific class of GameObjects called Goodies. All Goodies are also GameObjects so Goodies have access to the methods of the GameObjects base class unless the methods in the base class are specifically overwritten. These are the types of Goodies:
    - Barrels
    - Gold nuggets
    - Sonars
    - Pools of water

<br/>

**The StudentWorld class in StudentWorld.h and StudentWorld.cpp:** <br/>

The StudentWorld class represents the universe of the game. The member variables of the Student World represent some state of the game. The methods are used to implement the ways in which the game universe interacts with the game objects and items, and also how the game objects interact with each other.

<br/>

**Specification document** <br/>
The code for the Actor classes and the StudentWorld class can be fully explored by testing the program according to specifications in the document titled tunnelman-game-specifications.pdf attached in the repository.

<br/>

Notes on specifications:

1. The specifications file talks about hardcore protestors in addition to normal protestors. None of the functionality for the hardcore protestors was implemented.
2. It was also stated that protestors should have a leaving-the-oil-field state. I did not implement any of the functionality associated with the leaving-the-oil-field state including: <br/>
2.1. Creating an exit <br/>
2.2. Implementing a maze search function to find the quickest path to the exit<br/>
2.3. Instead of putting protestors in a leave the oil field state, I just set their status to dead immediately after their hitpoints dropped to zero or below. This caused them to immediately deleted by the move() function. <br/>


<br/>

**How to install and test the program (Use only Microsoft Visual Studio)** <br/>
This project was developed using Microsoft Visual Studio 2019 IDE on a Windows computer. 

<br/>

It is unlikely that you will successfully get the solution to run using an IDE other than Microsoft Visual Studio. To maximise the chance that the program will run successfully, please
1) Make sure you are using a Windows computer
2) Download and install the Microsoft Visual Studio 2019 IDE according to this guide: https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-160

<br/>

When following the guide, ensure that you specifically choose the option to install the Community 2019 Visual Studio rather than installing the Professional 2019 or Enterprise 2019 versions. 

<br/>

After the installer is installed, customize your installation of Visual Studio Community 2019 by selecting the "Desktop development with C++" workload only. Once done, don't add or remove anything else. All necessary individual components have been added upon your selection of the "Desktop development with C++" workload. Then click to install.

<br/>

Once installed, git clone the project to a local folder of your choice. This may take 5-10 minutes. Once cloned, navigate to that local folder and select the TunnelMan.sln file. Then, Right Click > Open with > Microsoft Visual Studio 2019. This will get Visual Studio to open your project.

<br/>

If Visual Studio prompts you to retarget the solution (which is unlikely), just follow the given instructions to retarget the solution. 

<br/>

Then press Ctrl + Alt + F7 to Rebuild Solution (The solution should compile as I have previously tried). If the solution does not compile, it is likely that there are errors can be quickly fixed. I won't provide solutions to compile errors you may face because I cannot anticipate what will happen on other machines. Once the program has compiled successfully, press Ctrl + F5 (or Start Without Debugging) and the game should start running.

