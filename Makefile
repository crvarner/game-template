# -*- Makefile -*-

SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include
TEST_DIR := test
TARGET := bin/game
TEST_TARGET := $(TARGET)_test
SRC_EXT := cpp

CC := g++
LIB := -L/usr/local/lib -framework OpenGL -lglfw
CFLAGS := -Wall -Werror -I$(INCLUDE_DIR)
DEPFLAGS := -MMD -MP
.DEFAULT_GOAL := all

SOURCES := $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
OBJECTS := $(SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(BUILD_DIR)/%.o)
DEPS := $(OBJECTS:%.o=%.d)
-include $(DEPS)

TEST_DIR_SOURCES := $(shell find $(TEST_DIR) -type f -name *.$(SRC_EXT))
TEST_DIR_OBJECTS := $(TEST_DIR_SOURCES:$(TEST_DIR)/%.$(SRC_EXT)=$(BUILD_DIR)/%.o)
TEST_DIR_DEPS := $(TEST_DIR_OBJECTS:%.o=%.d)
-include $(TEST_DIR_DEPS)

MAIN_OBJECT := $(BUILD_DIR)/main.o
TEST_OBJECTS := $(filter-out $(MAIN_OBJECT),$(OBJECTS)) $(TEST_DIR_OBJECTS)

.PHONY: all clean test

all: $(TARGET) $(TEST_TARGET)

run: $(TARGET)
	@$(TARGET)

test: $(TEST_TARGET)
	@$(TEST_TARGET)

# main executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LIB) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# test executable
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) $(LIB) $^ -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.$(SRC_EXT)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	-rm $(MAIN_OBJECT) $(TEST_OBJECTS) $(DEPS) $(TEST_DIR_DEPS) $(TARGET) $(TEST_TARGET)