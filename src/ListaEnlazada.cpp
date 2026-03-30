#include "ListaEnlazada.h"

#include <iostream>

Nodo::Nodo(Producto* producto) : producto(producto), siguiente(nullptr) {}

ListaEnlazada::ListaEnlazada() : cabeza(nullptr) {}

ListaEnlazada::~ListaEnlazada() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cabeza = nullptr;
}

void ListaEnlazada::insertar(Producto* producto) {
    if (producto == nullptr) {
        return;
    }

    Nodo* nuevoNodo = new Nodo(producto);
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
}

Producto* ListaEnlazada::buscarPorCodigoBarras(const std::string& codigo) const {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->producto != nullptr && actual->producto->codigoBarras == codigo) {
            return actual->producto;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

bool ListaEnlazada::eliminarPorCodigoBarras(const std::string& codigo) {
    Nodo* actual = cabeza;
    Nodo* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->producto != nullptr && actual->producto->codigoBarras == codigo) {
            if (anterior == nullptr) {
                cabeza = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }

            delete actual;
            return true;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    return false;
}

void ListaEnlazada::mostrarTodos() const {
    if (estaVacia()) {
        std::cout << "La lista esta vacia.\n";
        return;
    }

    Nodo* actual = cabeza;
    int indice = 1;
    while (actual != nullptr) {
        if (actual->producto != nullptr) {
            std::cout << "Producto " << indice << ":\n";
            std::cout << *(actual->producto) << "\n\n";
            ++indice;
        }
        actual = actual->siguiente;
    }
}

bool ListaEnlazada::estaVacia() const {
    return cabeza == nullptr;
}
