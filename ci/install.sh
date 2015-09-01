#!/bin/sh

if [ "$SUITE" = "build" ]; then
    sudo apt-get install python-software-properties
    sudo apt-add-repository -y ppa:libreoffice/libreoffice-4-2
    sudo apt-get update
    sudo apt-get install doxygen graphviz avr-libc gcc-avr binutils-avr

elif [ "$SUITE" = "tests" ]; then
    sudo pip install cpp-coveralls

else
    echo "Unknown suite variable - '$SUITE'. Terminating ..."
    exit 1
fi
