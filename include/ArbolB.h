#ifndef ARBOL_B_H
#define ARBOL_B_H

#include <fstream>
#include <string>

#include "ListaEnlazada.h"
#include "Producto.h"

// Estructura para manejar claves con productos duplicados por fecha 
struct ClaveFecha {
    std::string fecha;
    ListaEnlazada* productos;

    ClaveFecha();
    ~ClaveFecha();
};

// Nodo del Árbol B
class NodoB {
public:
    ClaveFecha* claves;
    NodoB** hijos;
    int numClaves;
    bool esHoja;
    int gradoMinimo;

    explicit NodoB(int t, bool hoja);
    ~NodoB();
};

// Árbol B con grado mínimo t = 3 (max 5 claves, 6 hijos)
class ArbolB {
private:
    NodoB* raiz;
    int gradoMinimo;

    void insertarEnNodoNoLleno(NodoB* nodo, Producto* producto);
    void dividirHijo(NodoB* padre, int indice, NodoB* hijo);

    ClaveFecha* buscarClaveRecursivo(NodoB* nodo, const std::string& fecha) const;
    void buscarRangoRecursivo(NodoB* nodo,
                              const std::string& fechaInicio,
                              const std::string& fechaFin,
                              int& contador) const;

    void destruirRecursivo(NodoB* nodo);

    void generarDotRecursivo(NodoB* nodo, std::ofstream& archivo, int& contadorNodo) const;
    std::string escaparTexto(const std::string& texto) const;

public:
    ArbolB(int t = 3);
    ~ArbolB();

    void insertar(Producto* producto);
    void buscarPorRango(const std::string& fechaInicio, const std::string& fechaFin) const;

    void generarDot(const std::string& rutaArchivo) const;
    void generarImagen() const;

    bool estaVacio() const;
};

#endif
