#include "MedidorRendimiento.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

// Constructor: Inicializa los arreglos de datos de prueba inexistentes
MedidorRendimiento::MedidorRendimiento() {
    for (int i = 0; i < N; ++i) {
        nombresInexistentes[i] = "ProductoInexistente_XYZ_" + std::to_string(9000 + i);
        codigosInexistentes[i] = "CODIGO_INVALIDO_" + std::to_string(9000 + i);
    }
}

// Inicializar datos de prueba desde los productos realmente cargados

void MedidorRendimiento::inicializarDatosPrueba(ListaEnlazada& listaNormal) {
    Producto* muestras[N];
    for (int i = 0; i < N; ++i) {
        muestras[i] = nullptr;
    }

    int cantidadObtenida = listaNormal.obtenerPrimerosProductos(muestras, N);

    for (int i = 0; i < N; ++i) {
        if (cantidadObtenida > 0) {
            Producto* producto = muestras[i % cantidadObtenida];
            nombresExistentes[i] = producto->nombre;
            codigosExistentes[i] = producto->codigoBarras;
        } else {
            nombresExistentes[i] = "";
            codigosExistentes[i] = "";
        }
    }
}

// MEDICIÓN EN LISTA NORMAL - O(n)
// Retorna tiempo total en milisegundos (double)

double MedidorRendimiento::medirBusquedaListaNormal(ListaEnlazada& lista,
                                                    const std::string nombres[],
                                                    int cantidad) const {
    double tiempoTotalMs = 0.0;

    for (int repeticion = 0; repeticion < M; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < cantidad; ++i) {
            volatile Producto* resultado = lista.buscarPorNombre(nombres[i]);
            (void)resultado;
        }

        auto fin = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duracion = fin - inicio;
        tiempoTotalMs += duracion.count();
    }

    return tiempoTotalMs;
}

// MEDICIÓN EN LISTA ORDENADA - O(n)

double MedidorRendimiento::medirBusquedaListaOrdenada(ListaEnlazadaOrdenada& lista,
                                                      const std::string nombres[],
                                                      int cantidad) const {
    double tiempoTotalMs = 0.0;

    for (int repeticion = 0; repeticion < M; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < cantidad; ++i) {
            volatile Producto* resultado = lista.buscarPorNombre(nombres[i]);
            (void)resultado;
        }

        auto fin = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duracion = fin - inicio;
        tiempoTotalMs += duracion.count();
    }

    return tiempoTotalMs;
}


// MEDICIÓN EN ÁRBOL AVL - O(log n)

double MedidorRendimiento::medirBusquedaAVL(ArbolAVL& arbol,
                                            const std::string nombres[],
                                            int cantidad) const {
    double tiempoTotalMs = 0.0;

    for (int repeticion = 0; repeticion < M; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < cantidad; ++i) {
            volatile Producto* resultado = arbol.buscarPorNombre(nombres[i]);
            (void)resultado;
        }

        auto fin = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duracion = fin - inicio;
        tiempoTotalMs += duracion.count();
    }

    return tiempoTotalMs;
}

// MÉTODOS DE GENERACIÓN DE STRINGS

std::string MedidorRendimiento::generarResumenMetodologia() const {
    std::ostringstream ss;
    ss << "\n";
    ss << "+==============================================================================+\n";
    ss << "|            BENCHMARKING DE BUSQUEDAS - CATALOGO DE PRODUCTOS                |\n";
    ss << "+==============================================================================+\n";
    ss << "|  Metodologia segun enunciado:                                               |\n";
    ss << "|  - N = " << std::setw(2) << N << " consultas por prueba                                            |\n";
    ss << "|  - M = " << std::setw(2) << M << " repeticiones del experimento                                    |\n";
    ss << "|  - Tiempo promedio = (Suma de M mediciones) / M                             |\n";
    ss << "|  - Medicion = tiempo de ejecutar N busquedas consecutivas                   |\n";
    ss << "+==============================================================================+\n";
    return ss.str();
}

std::string MedidorRendimiento::generarEncabezadoTabla() const {
    std::ostringstream ss;
    ss << "\n";
    ss << "+--------------------+----------------+-------------+----------------------+\n";
    ss << "|";
    ss << std::left << std::setw(20) << " Estructura";
    ss << "|";
    ss << std::setw(16) << " Tipo Busqueda";
    ss << "|";
    ss << std::setw(13) << " Complejidad";
    ss << "|";
    ss << std::setw(22) << " Tiempo Prom. (ms)";
    ss << "|\n";
    ss << "+--------------------+----------------+-------------+----------------------+\n";
    return ss.str();
}

std::string MedidorRendimiento::generarSeparador() const {
    return "+--------------------+----------------+-------------+----------------------+\n";
}

std::string MedidorRendimiento::generarFilaTabla(const std::string& estructura,
                                                  const std::string& tipoBusqueda,
                                                  const std::string& complejidad,
                                                  double tiempoPromedioMs) const {
    std::ostringstream ss;
    ss << "| ";
    ss << std::left << std::setw(19) << estructura;
    ss << "| ";
    ss << std::setw(15) << tipoBusqueda;
    ss << "| ";
    ss << std::setw(12) << complejidad;
    ss << "| ";
    ss << std::right << std::setw(17) << std::fixed << std::setprecision(4) 
       << tiempoPromedioMs << " ms";
    ss << " |\n";
    return ss.str();
}


// MÉTODO PRINCIPAL: ejecutarPruebasBusqueda
// Retorna un string con toda la tabla ASCII (sin Tabla Hash)

std::string MedidorRendimiento::ejecutarPruebasBusqueda(ListaEnlazada& listaNormal,
                                                         ListaEnlazadaOrdenada& listaOrdenada,
                                                         ArbolAVL& arbolAVL,
                                                         TablaHash& tablaHash) {
    std::ostringstream resultado;
    
    // Verificar que hay datos cargados
    if (listaNormal.estaVacia()) {
        resultado << "\n[ERROR] No hay productos cargados para ejecutar el benchmarking.\n";
        resultado << "Por favor, cargue primero un archivo CSV con productos.\n\n";
        return resultado.str();
    }

    // Suprimir warning de parámetro no usado (TablaHash se mantiene en firma pero no se usa)
    (void)tablaHash;

    // Inicializar datos de prueba desde el catálogo real
    inicializarDatosPrueba(listaNormal);

    // Generar metodología
    resultado << generarResumenMetodologia();

    // MEDICIONES

    // Lista Normal
    double tiempoListaNormalExitosa = medirBusquedaListaNormal(listaNormal, nombresExistentes, N);
    double tiempoListaNormalFallida = medirBusquedaListaNormal(listaNormal, nombresInexistentes, N);

    // Lista Ordenada
    double tiempoListaOrdenadaExitosa = medirBusquedaListaOrdenada(listaOrdenada, nombresExistentes, N);
    double tiempoListaOrdenadaFallida = medirBusquedaListaOrdenada(listaOrdenada, nombresInexistentes, N);

    // Árbol AVL
    double tiempoAVLExitosa = medirBusquedaAVL(arbolAVL, nombresExistentes, N);
    double tiempoAVLFallida = medirBusquedaAVL(arbolAVL, nombresInexistentes, N);

    // CÁLCULO DE PROMEDIOS (dividir entre M)
    const double divisorM = static_cast<double>(M);

    double promedioListaNormalExitosa = tiempoListaNormalExitosa / divisorM;
    double promedioListaNormalFallida = tiempoListaNormalFallida / divisorM;
    double promedioListaOrdenadaExitosa = tiempoListaOrdenadaExitosa / divisorM;
    double promedioListaOrdenadaFallida = tiempoListaOrdenadaFallida / divisorM;
    double promedioAVLExitosa = tiempoAVLExitosa / divisorM;
    double promedioAVLFallida = tiempoAVLFallida / divisorM;


    // GENERAR TABLA

    resultado << generarEncabezadoTabla();

    // Lista Normal
    resultado << generarFilaTabla("Lista Normal", "Exitosa", "O(n)", promedioListaNormalExitosa);
    resultado << generarFilaTabla("Lista Normal", "Fallida", "O(n)", promedioListaNormalFallida);
    resultado << generarSeparador();

    // Lista Ordenada
    resultado << generarFilaTabla("Lista Ordenada", "Exitosa", "O(n)", promedioListaOrdenadaExitosa);
    resultado << generarFilaTabla("Lista Ordenada", "Fallida", "O(n)", promedioListaOrdenadaFallida);
    resultado << generarSeparador();

    // Árbol AVL
    resultado << generarFilaTabla("Arbol AVL", "Exitosa", "O(log n)", promedioAVLExitosa);
    resultado << generarFilaTabla("Arbol AVL", "Fallida", "O(log n)", promedioAVLFallida);

    resultado << "+--------------------+----------------+-------------+----------------------+\n";

    // Notas
    resultado << "\n";
    resultado << "  Notas:\n";
    resultado << "  - Tiempos en milisegundos (ms).\n";
    resultado << "  - Cada medicion ejecuta " << N << " busquedas consecutivas.\n";
    resultado << "  - El promedio se calcula sobre " << M << " repeticiones del experimento.\n";
    resultado << "\n";

 
    // ANÁLISIS COMPARATIVO
  
    resultado << "+------------------------------------------------------------------------------+\n";
    resultado << "|                           ANALISIS COMPARATIVO                              |\n";
    resultado << "+------------------------------------------------------------------------------+\n";

    // Encontrar el más rápido
    double tiempos[3] = {promedioListaNormalExitosa, promedioListaOrdenadaExitosa, promedioAVLExitosa};
    const char* nombres[3] = {"Lista Normal", "Lista Ordenada", "Arbol AVL"};
    
    int indiceMinimo = 0;
    for (int i = 1; i < 3; ++i) {
        if (tiempos[i] < tiempos[indiceMinimo]) {
            indiceMinimo = i;
        }
    }

    resultado << "|  Estructura mas rapida (busqueda exitosa): " 
              << std::left << std::setw(31) << nombres[indiceMinimo] << "|\n";

    // Speedup AVL vs Lista Normal
    if (promedioListaNormalExitosa > 0 && promedioAVLExitosa > 0) {
        double speedupAVL = promedioListaNormalExitosa / promedioAVLExitosa;
        
        std::ostringstream ssSpeedup;
        ssSpeedup << std::fixed << std::setprecision(2) << speedupAVL << "x";
        
        resultado << "|  Speedup AVL vs Lista Normal: " 
                  << std::left << std::setw(44) << ssSpeedup.str() << "|\n";
    }

    resultado << "+------------------------------------------------------------------------------+\n\n";

    return resultado.str();
}
