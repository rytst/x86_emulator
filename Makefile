TARGET := x86_emulator

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=%.o)

CC := gcc

.PHONY: all clean distclean

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(BUILD_DIR)/$(OBJS)
	echo $(BUILD_DIR)/$(OBJS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)/*.o

distclean:
	$(RM) $(BUILD_DIR)/$(TARGET)
