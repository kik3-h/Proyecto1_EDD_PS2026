#ifndef TABLA_HASH_H
#define TABLA_HASH_H

#include <string>

#include "Producto.h"

class NodoHash {
public:
    Producto* producto;
    NodoHash* siguiente;

    explicit NodoHash(Producto* producto);
};

class TablaHash {
private:
    NodoHash** tabla;
    int capacidad;

public:
    explicit TablaHash(int capacidadInicial = 1009);
    ~TablaHash();

    int funcionHash(const std::string& clave) const;
    bool insertar(Producto* producto);
    Producto* buscarPorCodigoBarras(const std::string& codigo) const;
    bool eliminarPorCodigoBarras(const std::string& codigo);
};

#endif
