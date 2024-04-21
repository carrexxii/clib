BIN = clib

CC     = gcc
LINKER = gcc

SRC_DIR   = ./
BUILD_DIR = ./build

WARNINGS = -Wall -Wextra -Wno-missing-braces -Wno-unused-function -Wno-unused-parameter
CFLAGS = -std=c2x -Og -fstrict-aliasing -g2 -ggdb -pipe $(WARNINGS) -ftabstop=4   \
         -fstack-protector-strong -fstack-clash-protection -fno-omit-frame-pointer \
         -fsanitize=address -fsanitize=undefined -fsanitize-address-use-after-scope \
         -DDEBUG

LFLAGS   = -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fsanitize-address-use-after-scope
DEPFLAGS = -MT $@ -MMD -MF $(BUILD_DIR)/$*.dep

SRC := $(wildcard $(SRC_DIR)/tests/*.c) \
       $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEP := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.dep)

PRECOMPILE  = mkdir -p $(@D)
POSTCOMPILE =

$(BIN): $(OBJ)
	@$(LINKER) -o $@ $(LFLAGS) $(OBJ)
	@echo "Linking complete"

$(OBJ): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(PRECOMPILE)
	@$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
	@$(POSTCOMPILE)
	@echo "Compiled " $<

-include $(OBJ:.o=.dep)

all: $(BIN)

.PHONY: test
test: all
	@./$(BIN)

.PHONY: restore
restore: clean
	@bear -- make -j8

.PHONY: clean
clean:
	@rm -f $(OBJ)
	@echo "Executable removed"
	@rm -f $(DEP)
	@echo "Dependency files removed"
	@echo "Cleanup complete"

.PHONY: remove
remove: clean
	@echo "Tools removed"

.PHONY: cloc
cloc:
	cloc --vcs=git

