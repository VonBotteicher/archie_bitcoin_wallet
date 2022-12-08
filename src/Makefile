CC=g++
CFLAGS=-std=c++11
PKG_CONFIG_PATH=/home/von/libbitcoin/lib/pkgconfig/

PKG_CONFIG_SYS=$$(PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags --libs libbitcoin-system)
PKG_CONFIG_EXP=$$(PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags --libs libbitcoin-explorer)
PKG_CONFIG_CLIENT=$$(PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags --libs libbitcoin-client)
PKG_CONFIG_PROTO=$$(PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags --libs libbitcoin-protocol)
PKG_CONFIG_SERVER=$$(PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags --libs libbitcoin-server)
BUILD_DIR=../build
LIB_JSON=-ljansson
PKG_CONFIG_LIB_JSON=$$(pkg-config --cflags --libs jansson)

hello_btc: hello_btc.cpp 
	$(CC) -o $(BUILD_DIR)/hello_btc hello_btc.cpp $(CFLAGS) $(PKG_CONFIG_CLIENT)

address_generator: address_generator.cpp 
	$(CC) -o $(BUILD_DIR)/address_generator address_generator.cpp $(CFLAGS) $(PKG_CONFIG_EXP) $(PKG_CONFIG_SYS)

jansson_test: jansson_test.cpp 
	$(CC) -o $(BUILD_DIR)/jansson_test jansson_test.cpp $(CFLAGS) $(PKG_CONFIG_LIB_JSON)