#!/bin/sh

if test -x streams && test -d Resources ; then
    cd Resources
    ../streams
else
    echo "Go build this Game of the Century" >&2
    exit 1
fi
