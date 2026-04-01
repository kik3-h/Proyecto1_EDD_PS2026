CXX = g++
FLAGS_COMPILACION = -Wall -Wextra -std=c++17 -O2
RUTA_FUENTES = src
RUTA_CABECERAS = include
EJECUTABLE = catalogoEdp1kike

ARCHIVOS_CPP = $(wildcard $(RUTA_FUENTES)/*.cpp)
ARCHIVOS_OBJ = $(ARCHIVOS_CPP:.cpp=.o)

.PHONY: all limpiar

all: $(EJECUTABLE)

$(EJECUTABLE): $(ARCHIVOS_OBJ)
	$(CXX) $(FLAGS_COMPILACION) -I$(RUTA_CABECERAS) -o $@ $^

$(RUTA_FUENTES)/%.o: $(RUTA_FUENTES)/%.cpp
	$(CXX) $(FLAGS_COMPILACION) -I$(RUTA_CABECERAS) -c $< -o $@

run: $(EJECUTABLE)
	@echo "Ejecutando el proyecto uno de kike, llamado: $(EJECUTABLE)..."
	./$(EJECUTABLE)

limpiar:
	rm -f $(RUTA_FUENTES)/*.o $(EJECUTABLE)
