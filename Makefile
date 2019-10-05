SHELL := /usr/bin/env bash

CC = sdcc
LD = sdcc
AR = sdar
CDEFS = -DSTM8S103

include utilities/builder/Makefile

.PHONY: build clean

build: $(BIN_DIR)/app.lib

clean:
	rm -rf $(BIN_DIR) || true