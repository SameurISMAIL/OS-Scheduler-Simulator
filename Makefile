# Compilateur C
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c99 -I.

# Répertoires
SRC_DIR = algorithms/source
BUILD_DIR = algorithms/build
MAIN_SRC = main.c
MAIN_EXEC = game

# Lister les sources des algorithmes
ALGO_SOURCES = $(wildcard $(SRC_DIR)/*.c)
# Créer les noms des exécutables correspondants
ALGO_EXECS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%,$(ALGO_SOURCES))

# Règle par défaut
all: $(MAIN_EXEC) scheduling_policies_executables

# Règle pour créer le lanceur principal
$(MAIN_EXEC): $(MAIN_SRC) process.h
	$(CC) $(CFLAGS) $(MAIN_SRC) -o $(MAIN_EXEC)

# Règle pour créer tous les exécutables des algorithmes
# Nommée comme dans votre documentation
scheduling_policies_executables: $(ALGO_EXECS)

# Règle générique pour compiler un algorithme
$(BUILD_DIR)/%: $(SRC_DIR)/%.c process.h
	@mkdir -p $(BUILD_DIR) # Crée le répertoire build s'il n'existe pas
	$(CC) $(CFLAGS) $< -o $@

# Règle pour nettoyer le projet
clean:
	rm -f $(MAIN_EXEC)
	rm -rf $(BUILD_DIR)

.PHONY: all clean scheduling_policies_executables