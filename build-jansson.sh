#!/bin/sh
cd jansson
autoreconf -fi
./configure
make
sudo make install