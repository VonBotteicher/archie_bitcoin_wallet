#!/bin/sh
PREFIX_DIR=/home/$(whoami)/libbitcoin
rm -rf ${PREFIX_DIR}
cd libbitcoin-explorer
PARALLEL=12 ./install.sh --prefix=${PREFIX_DIR} --build-boost --build-zmq
cd ../libbitcoin-server
PARALLEL=12 ./install.sh --prefix=${PREFIX_DIR}