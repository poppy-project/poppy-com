#!/bin/sh

if [ "$SUITE" = "build" ]; then
    sudo apt-get update -qq
    sudo apt-get install -qq doxygen graphviz avr-libc gcc-avr binutils-avr

elif [ "$SUITE" = "tests" ]; then
    sudo pip install cpp-coveralls

else
    echo "Unknown suite variable - '$SUITE'. Terminating ..."
    exit 1
fi
