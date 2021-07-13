SRC_DIR := ./src/cpp
BUILD_DIR := ./src
TARGET := cpp.js

STD_FLAG := -std=c++11

WASM_FLAG := WASM=1

all:
	em++ ${SRC_DIR}/main.cpp \
	--bind \
	${STD_FLAG} \
	-s ${WASM_FLAG} \
	-O3 \
	-o ${BUILD_DIR}/${TARGET}
