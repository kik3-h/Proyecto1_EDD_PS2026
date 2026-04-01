#ifndef MEDIDOR_RENDIMIENTO_H
#define MEDIDOR_RENDIMIENTO_H

#include "ArbolAVL.h"
#include "ListaEnlazada.h"
#include "ListaEnlazadaOrdenada.h"
#include "TablaHash.h"

class MedidorRendimiento {
private:
    static const int CANTIDAD_CONSULTAS = 20;
    static const int CANTIDAD_REPETICIONES = 5;

    long long medirBusquedaNombresExitososListaNormal(ListaEnlazada& lista,
                                                      std::string nombres[],
                                                      int cantidad) const;
    long long medirBusquedaNombresFallidosListaNormal(ListaEnlazada& lista,
                                                      std::string nombres[],
                                                      int cantidad) const;
    long long medirBusquedaNombresExitososListaOrdenada(ListaEnlazadaOrdenada& lista,
                                                        std::string nombres[],
                                                        int cantidad) const;
    long long medirBusquedaNombresFallidosListaOrdenada(ListaEnlazadaOrdenada& lista,
                                                        std::string nombres[],
                                                        int cantidad) const;
    long long medirBusquedaNombresExitososAVL(ArbolAVL& arbol,
                                              std::string nombres[],
                                              int cantidad) const;
    long long medirBusquedaNombresFallidosAVL(ArbolAVL& arbol,
                                              std::string nombres[],
                                              int cantidad) const;
    long long medirBusquedaCodigosExitososHash(TablaHash& tabla,
                                               std::string codigos[],
                                               int cantidad) const;
    long long medirBusquedaCodigosFallidosHash(TablaHash& tabla,
                                               std::string codigos[],
                                               int cantidad) const;

    void imprimirEncabezadoTabla() const;
    void imprimirFilaTabla(const std::string& estructura,
                           const std::string& tipoBusqueda,
                           double tiempoPromedioMicrosegundos) const;

public:
    void ejecutarPruebasBusqueda(ListaEnlazada& listaNormal,
                                 ListaEnlazadaOrdenada& listaOrdenada,
                                 ArbolAVL& arbolAVL,
                                 TablaHash& tablaHash);
};

#endif
