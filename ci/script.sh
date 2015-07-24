#!/bin/sh

if [ "$SUITE" = "build" ]; then
    make

elif [ "$SUITE" = "tests" ]; then
    make test && ./test.elf

else
    echo "Unknown suite variable - '$SUITE'. Terminating ..."
    exit 1
fi
