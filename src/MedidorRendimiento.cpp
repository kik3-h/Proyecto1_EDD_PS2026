#include "MedidorRendimiento.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

long long MedidorRendimiento::medirBusquedaNombresExitososListaNormal(ListaEnlazada& lista,
                                                                      std::string nombres[],
                                                                      int cantidad) const {
    long long totalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < CANTIDAD_REPETICIONES; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < cantidad; ++i) {
            (void)lista.buscarPorNombre(nombres[i]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        totalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return totalMicrosegundos;
}

long long MedidorRendimiento::medirBusquedaNombresFallidosListaNormal(ListaEnlazada& lista,
                                                                      std::string nombres[],
                                                                      int cantidad) const {
    long long totalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < CANTIDAD_REPETICIONES; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < cantidad; ++i) {
            (void)lista.buscarPorNombre(nombres[i]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        totalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return totalMicrosegundos;
}

long long MedidorRendimiento::medirBusquedaNombresExitososListaOrdenada(ListaEnlazadaOrdenada& lista,
                                                                        std::string nombres[],
                                                                        int cantidad) const {
    long long totalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < CANTIDAD_REPETICIONES; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < cantidad; ++i) {
            (void)lista.buscarPorNombre(nombres[i]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        totalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return totalMicrosegundos;
}

long long MedidorRendimiento::medirBusquedaNombresFallidosListaOrdenada(ListaEnlazadaOrdenada& lista,
                                                                        std::string nombres[],
                                                                        int cantidad) const {
    long long totalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < CANTIDAD_REPETICIONES; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < cantidad; ++i) {
            (void)lista.buscarPorNombre(nombres[i]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        totalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return totalMicrosegundos;
}

long long MedidorRendimiento::medirBusquedaNombresExitososAVL(ArbolAVL& arbol,
                                                              std::string nombres[],
                                                              int cantidad) const {
    long long totalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < CANTIDAD_REPETICIONES; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < cantidad; ++i) {
            (void)arbol.buscarPorNombre(nombres[i]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        totalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return totalMicrosegundos;
}

long long MedidorRendimiento::medirBusquedaNombresFallidosAVL(ArbolAVL& arbol,
                                                              std::string nombres[],
                                                              int cantidad) const {
    long long totalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < CANTIDAD_REPETICIONES; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < cantidad; ++i) {
            (void)arbol.buscarPorNombre(nombres[i]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        totalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return totalMicrosegundos;
}

long long MedidorRendimiento::medirBusquedaCodigosExitososHash(TablaHash& tabla,
                                                               std::string codigos[],
                                                               int cantidad) const {
    long long totalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < CANTIDAD_REPETICIONES; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < cantidad; ++i) {
            (void)tabla.buscarPorCodigoBarras(codigos[i]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        totalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return totalMicrosegundos;
}

long long MedidorRendimiento::medirBusquedaCodigosFallidosHash(TablaHash& tabla,
                                                               std::string codigos[],
                                                               int cantidad) const {
    long long totalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < CANTIDAD_REPETICIONES; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < cantidad; ++i) {
            (void)tabla.buscarPorCodigoBarras(codigos[i]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        totalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return totalMicrosegundos;
}

void MedidorRendimiento::imprimirEncabezadoTabla() const {
    std::cout << "\n=== REPORTE DE BENCHMARKING DE BUSQUEDAS ===\n";
    std::cout << std::left << std::setw(24) << "Estructura"
              << std::setw(18) << "Tipo Busqueda"
              << std::setw(24) << "Tiempo Promedio (us)"
              << '\n';
    std::cout << std::string(66, '-') << '\n';
}

void MedidorRendimiento::imprimirFilaTabla(const std::string& estructura,
                                           const std::string& tipoBusqueda,
                                           double tiempoPromedioMicrosegundos) const {
    std::cout << std::left << std::setw(24) << estructura
              << std::setw(18) << tipoBusqueda
              << std::setw(24) << std::fixed << std::setprecision(2) << tiempoPromedioMicrosegundos
              << '\n';
}

void MedidorRendimiento::ejecutarPruebasBusqueda(ListaEnlazada& listaNormal,
                                                 ListaEnlazadaOrdenada& listaOrdenada,
                                                 ArbolAVL& arbolAVL,
                                                 TablaHash& tablaHash) {
    Producto* muestrasProductos[CANTIDAD_CONSULTAS];
    for (int i = 0; i < CANTIDAD_CONSULTAS; ++i) {
        muestrasProductos[i] = nullptr;
    }

    int cantidadMuestras = listaNormal.obtenerPrimerosProductos(muestrasProductos, CANTIDAD_CONSULTAS);
    if (cantidadMuestras <= 0) {
        std::cout << "No hay productos cargados para ejecutar el benchmarking.\n\n";
        return;
    }

    std::string nombresExitosos[CANTIDAD_CONSULTAS];
    std::string codigosExitosos[CANTIDAD_CONSULTAS];
    std::string nombresFallidos[CANTIDAD_CONSULTAS];
    std::string codigosFallidos[CANTIDAD_CONSULTAS];

    for (int i = 0; i < CANTIDAD_CONSULTAS; ++i) {
        Producto* producto = muestrasProductos[i % cantidadMuestras];
        nombresExitosos[i] = producto->nombre;
        codigosExitosos[i] = producto->codigoBarras;
        nombresFallidos[i] = "InexistenteNombre_" + std::to_string(i);
        codigosFallidos[i] = "InexistenteCodigo_" + std::to_string(i);
    }

    long long tiempoTotalListaNormalExitosa =
        medirBusquedaNombresExitososListaNormal(listaNormal, nombresExitosos, CANTIDAD_CONSULTAS);
    long long tiempoTotalListaNormalFallida =
        medirBusquedaNombresFallidosListaNormal(listaNormal, nombresFallidos, CANTIDAD_CONSULTAS);

    long long tiempoTotalListaOrdenadaExitosa =
        medirBusquedaNombresExitososListaOrdenada(listaOrdenada, nombresExitosos, CANTIDAD_CONSULTAS);
    long long tiempoTotalListaOrdenadaFallida =
        medirBusquedaNombresFallidosListaOrdenada(listaOrdenada, nombresFallidos, CANTIDAD_CONSULTAS);

    long long tiempoTotalAVLExitosa =
        medirBusquedaNombresExitososAVL(arbolAVL, nombresExitosos, CANTIDAD_CONSULTAS);
    long long tiempoTotalAVLFallida =
        medirBusquedaNombresFallidosAVL(arbolAVL, nombresFallidos, CANTIDAD_CONSULTAS);

    long long tiempoTotalHashExitosa =
        medirBusquedaCodigosExitososHash(tablaHash, codigosExitosos, CANTIDAD_CONSULTAS);
    long long tiempoTotalHashFallida =
        medirBusquedaCodigosFallidosHash(tablaHash, codigosFallidos, CANTIDAD_CONSULTAS);

    const double divisorPromedio = static_cast<double>(CANTIDAD_REPETICIONES);

    imprimirEncabezadoTabla();
    imprimirFilaTabla("Lista Normal", "Exitosa", tiempoTotalListaNormalExitosa / divisorPromedio);
    imprimirFilaTabla("Lista Normal", "Fallida", tiempoTotalListaNormalFallida / divisorPromedio);
    imprimirFilaTabla("Lista Ordenada", "Exitosa", tiempoTotalListaOrdenadaExitosa / divisorPromedio);
    imprimirFilaTabla("Lista Ordenada", "Fallida", tiempoTotalListaOrdenadaFallida / divisorPromedio);
    imprimirFilaTabla("Arbol AVL", "Exitosa", tiempoTotalAVLExitosa / divisorPromedio);
    imprimirFilaTabla("Arbol AVL", "Fallida", tiempoTotalAVLFallida / divisorPromedio);
    imprimirFilaTabla("Tabla Hash", "Exitosa", tiempoTotalHashExitosa / divisorPromedio);
    imprimirFilaTabla("Tabla Hash", "Fallida", tiempoTotalHashFallida / divisorPromedio);
    std::cout << '\n';
}
