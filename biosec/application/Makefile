# Java Makefile for Linux
# Directory structure:
# src/  → .java files
# build/ → .class output
# dist/ → final JAR

JAVAC   = javac
JAVA    = java
JAR     = jar

SRC_DIR = src
BIN_DIR = build
DIST_DIR = dist

MAIN_CLASS = com.example.Main
JAR_FILE = app.jar

# Find all .java files
SOURCES = $(shell find $(SRC_DIR) -name "*.java")

# Convert java source list to class list
CLASSES = $(SOURCES:$(SRC_DIR)/%.java=$(BIN_DIR)/%.class)

all: setup compile jar

setup:
	mkdir -p $(BIN_DIR)
	mkdir -p $(DIST_DIR)

compile: $(CLASSES)

$(BIN_DIR)/%.class: $(SRC_DIR)/%.java
	@echo "Compiling $< ..."
	@mkdir -p $(dir $@)
	$(JAVAC) -d $(BIN_DIR) $<

jar:
	@echo "Creating JAR..."
	$(JAR) cfe $(DIST_DIR)/$(JAR_FILE) $(MAIN_CLASS) -C $(BIN_DIR) .

run:
	$(JAVA) -cp $(DIST_DIR)/$(JAR_FILE) $(MAIN_CLASS)

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(DIST_DIR)/*

.PHONY: all setup compile jar run clean

