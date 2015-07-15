Menu.cpp is the one that loads up the main homescreen

Tron.cpp is where the main code for game and the audio track is there.

Note there is one drawback of using SDL_mixer library to play sounds . the sound file is played by a different process that you have no way of controlling once you execute your code.

so you need to manually fork the process and then create and destroy it explicitly .

before starting the game you'll need to install the required libraries.
just run the library.sh bash script first and then run the start.sh bash script 

to start game just run the start.sh shell script it'll compile and run game.

