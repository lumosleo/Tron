#!/bin/bash

#checks if the 3 libraries are present if not then installs them 
#https://github.com/lumosleo/

if [ $(dpkg-query -W -f='${Status}' libsdl1.2-dev 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  echo "---------------installing libsdl1.2-dev now------------------------"
  apt-get install nano;
else
  echo "--------------libsdl1.2-dev already installed----------------------"

fi
#check for SDL_mixer library
if [ $(dpkg-query -W -f='${Status}' libsdl-mixer1.2-dev 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  echo "-------------installing libsdl-mixer1.2-dev now--------------------"
  apt-get install libsdl-mixer1.2-dev;
else
  echo "-----------libsdl-mixer1.2-dev already installed-------------------"
fi

#check for SOIL library
if [ $(dpkg-query -W -f='${Status}' libsoil-dev 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  echo "---------------installing libsoil-dev now--------------------------"
  apt-get install libsoil-dev;
else
  echo "---------------libsoil already installed---------------------------"
fi

