# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -g

# Nome do executável final
TARGET = out/out.exe

# Diretório de origem
SRC_DIR = src

# Diretório de saída para arquivos objeto
OBJ_DIR = out

# Lista de arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Arquivos objeto correspondentes
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Regra padrão (default)
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para compilar arquivos .c em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Criação do diretório de saída
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Regra para compilar e executar o programa
run: all
	./$(TARGET)

# Limpeza dos arquivos compilados
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# PHONY targets
.PHONY: all clean run
