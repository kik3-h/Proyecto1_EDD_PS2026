#ifndef MEDIDOR_RENDIMIENTO_H
#define MEDIDOR_RENDIMIENTO_H

#include "ArbolAVL.h"
#include "ListaEnlazada.h"
#include "ListaEnlazadaOrdenada.h"
#include "TablaHash.h"

// ============================================================================
// MedidorRendimiento: Clase para ejecutar pruebas de benchmarking
// según el enunciado del proyecto.
//
// Metodología de medición (según PDF):
// - N = 20 consultas consecutivas por cada prueba
// - M = 5 repeticiones del experimento para obtener el promedio
// - Tiempo final = (Suma de M mediciones) / M
// - Cada medición = tiempo de ejecutar N búsquedas consecutivas
// ============================================================================
class MedidorRendimiento {
private:
    // Constantes del experimento según enunciado
    static const int N = 20;  // Cantidad de consultas por prueba
    static const int M = 5;   // Cantidad de repeticiones del experimento

    // Arreglos estáticos de datos de prueba (20 elementos cada uno)
    // Nombres que SÍ existen en el CSV
    std::string nombresExistentes[N];
    // Códigos que SÍ existen en el CSV
    std::string codigosExistentes[N];
    // Nombres que NO existen (para búsquedas fallidas)
    std::string nombresInexistentes[N];
    // Códigos que NO existen (para búsquedas fallidas)
    std::string codigosInexistentes[N];

    // Métodos de medición para cada estructura y caso
    // Retornan el tiempo total acumulado de M repeticiones (en microsegundos)
    long long medirBusquedaListaNormal(ListaEnlazada& lista,
                                       const std::string nombres[],
                                       int cantidad) const;
    long long medirBusquedaListaOrdenada(ListaEnlazadaOrdenada& lista,
                                         const std::string nombres[],
                                         int cantidad) const;
    long long medirBusquedaAVL(ArbolAVL& arbol,
                               const std::string nombres[],
                               int cantidad) const;
    long long medirBusquedaHash(TablaHash& tabla,
                                const std::string codigos[],
                                int cantidad) const;

    // Métodos de presentación de resultados
    void imprimirEncabezadoTabla() const;
    void imprimirSeparador() const;
    void imprimirFilaTabla(const std::string& estructura,
                           const std::string& tipoBusqueda,
                           const std::string& complejidad,
                           double tiempoPromedioMicrosegundos) const;
    void imprimirResumenMetodologia() const;

    // Inicializar datos de prueba desde la lista cargada
    void inicializarDatosPrueba(ListaEnlazada& listaNormal);

public:
    MedidorRendimiento();

    // Método principal de ejecución del benchmarking
    void ejecutarPruebasBusqueda(ListaEnlazada& listaNormal,
                                 ListaEnlazadaOrdenada& listaOrdenada,
                                 ArbolAVL& arbolAVL,
                                 TablaHash& tablaHash);
};

#endif
