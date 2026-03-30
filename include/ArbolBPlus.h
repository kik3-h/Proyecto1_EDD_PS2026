#ifndef ARBOL_BPLUS_H
#define ARBOL_BPLUS_H

#include <fstream>
#include <string>

#include "ListaEnlazada.h"
#include "Producto.h"

// Estructura para manejar claves con productos de la misma categoría
struct ClaveCategoria {
    std::string categoria;
    ListaEnlazada* productos;  // Solo se usa en hojas

    ClaveCategoria();
    ~ClaveCategoria();
};

// Nodo del Árbol B+
class NodoBPlus {
public:
    ClaveCategoria* claves;
    NodoBPlus** hijos;
    int numClaves;
    bool esHoja;
    int gradoMinimo;
    NodoBPlus* siguiente;  // Enlace entre hojas (solo para nodos hoja)

    explicit NodoBPlus(int t, bool hoja);
    ~NodoBPlus();
};

// Árbol B+ con grado mínimo t = 3
class ArbolBPlus {
private:
    NodoBPlus* raiz;
    int gradoMinimo;

    void insertarEnNodoNoLleno(NodoBPlus* nodo, Producto* producto);
    void dividirHijo(NodoBPlus* padre, int indice, NodoBPlus* hijo);

    NodoBPlus* buscarHoja(NodoBPlus* nodo, const std::string& categoria) const;
    ClaveCategoria* buscarClaveEnHoja(NodoBPlus* hoja, const std::string& categoria) const;

    void destruirRecursivo(NodoBPlus* nodo);

    void generarDotRecursivo(NodoBPlus* nodo, std::ofstream& archivo, int& contadorNodo) const;
    void generarEnlacesHojas(std::ofstream& archivo) const;
    std::string escaparTexto(const std::string& texto) const;

public:
    ArbolBPlus(int t = 3);
    ~ArbolBPlus();

    void insertar(Producto* producto);
    void buscarPorCategoria(const std::string& categoria) const;

    void generarDot(const std::string& rutaArchivo) const;
    void generarImagen() const;

    bool estaVacio() const;
};

#endif
