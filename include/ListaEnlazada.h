#ifndef LISTA_ENLAZADA_H
#define LISTA_ENLAZADA_H

#include <string>

#include "Producto.h"

class Nodo {
public:
    Producto* producto;
    Nodo* siguiente;

    explicit Nodo(Producto* producto);
};

class ListaEnlazada {
private:
    Nodo* cabeza;

public:
    ListaEnlazada();
    ~ListaEnlazada();

    void insertar(Producto* producto);
    Producto* buscarPorCodigoBarras(const std::string& codigo) const;
    bool eliminarPorCodigoBarras(const std::string& codigo);
    void mostrarTodos() const;
    bool estaVacia() const;
};

#endif
