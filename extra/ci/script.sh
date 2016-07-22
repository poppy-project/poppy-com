#!/bin/sh

if [ "$SUITE" = "build" ]; then
    make

elif [ "$SUITE" = "tests" ]; then
    make HAL=stub && ./extra/examples/tests/tests_stub.elf

else
    echo "Unknown suite variable - '$SUITE'. Terminating ..."
    exit 1
fi
