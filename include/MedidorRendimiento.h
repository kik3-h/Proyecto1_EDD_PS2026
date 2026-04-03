#ifndef MEDIDOR_RENDIMIENTO_H
#define MEDIDOR_RENDIMIENTO_H

#include <string>
#include <sstream>

#include "ArbolAVL.h"
#include "ListaEnlazada.h"
#include "ListaEnlazadaOrdenada.h"
#include "TablaHash.h"

// MedidorRendimiento: Clase para ejecutar pruebas de benchmarking
// Metodología de medición 
// - N = 20 consultas consecutivas por cada prueba
// - M = 5 repeticiones del experimento para obtener el promedio
// - Tiempo final = (Suma de M mediciones) / M
// - Cada medición = tiempo de ejecutar N búsquedas consecutivas
class MedidorRendimiento {
private:
    // Constantes del experimento según enunciado
    static const int N = 20;  // Cantidad de consultas por prueba
    static const int M = 5;   // Cantidad de repeticiones del experimento

    // Arreglos estáticos de datos de prueba (20 elementos cada uno)
    std::string nombresExistentes[N];
    std::string codigosExistentes[N];
    std::string nombresInexistentes[N];
    std::string codigosInexistentes[N];

    // Métodos de medición para cada estructura
    // Retornan el tiempo total acumulado de M repeticiones (en nanosegundos)
    double medirBusquedaListaNormal(ListaEnlazada& lista,
                                    const std::string nombres[],
                                    int cantidad) const;
    double medirBusquedaListaOrdenada(ListaEnlazadaOrdenada& lista,
                                      const std::string nombres[],
                                      int cantidad) const;
    double medirBusquedaAVL(ArbolAVL& arbol,
                            const std::string nombres[],
                            int cantidad) const;

    // Métodos de presentación (generan strings en lugar de imprimir)
    std::string generarEncabezadoTabla() const;
    std::string generarSeparador() const;
    std::string generarFilaTabla(const std::string& estructura,
                                  const std::string& tipoBusqueda,
                                  const std::string& complejidad,
                                  double tiempoPromedioMs) const;
    std::string generarResumenMetodologia() const;

    // Inicializar datos de prueba desde la lista cargada
    void inicializarDatosPrueba(ListaEnlazada& listaNormal);

public:
    MedidorRendimiento();

    // Método principal - retorna string con toda la tabla ASCII
    std::string ejecutarPruebasBusqueda(ListaEnlazada& listaNormal,
                                         ListaEnlazadaOrdenada& listaOrdenada,
                                         ArbolAVL& arbolAVL,
                                         TablaHash& tablaHash);
};

#endif
