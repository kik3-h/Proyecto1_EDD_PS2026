#include "MedidorRendimiento.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

// ============================================================================
// Constructor: Inicializa los arreglos de datos de prueba inexistentes
// Los datos existentes se llenan dinámicamente desde la lista cargada
// ============================================================================
MedidorRendimiento::MedidorRendimiento() {
    // Inicializar nombres y códigos que NO existen en el catálogo
    // Estos se usan para medir búsquedas fallidas (peor caso)
    for (int i = 0; i < N; ++i) {
        nombresInexistentes[i] = "ProductoInexistente_XYZ_" + std::to_string(9000 + i);
        codigosInexistentes[i] = "CODIGO_INVALIDO_" + std::to_string(9000 + i);
    }
}

// ============================================================================
// Inicializar datos de prueba desde los productos realmente cargados
// Esto garantiza que los nombres y códigos SÍ existen en las estructuras
// ============================================================================
void MedidorRendimiento::inicializarDatosPrueba(ListaEnlazada& listaNormal) {
    Producto* muestras[N];
    for (int i = 0; i < N; ++i) {
        muestras[i] = nullptr;
    }

    int cantidadObtenida = listaNormal.obtenerPrimerosProductos(muestras, N);

    // Llenar arreglos con datos reales del catálogo
    for (int i = 0; i < N; ++i) {
        if (cantidadObtenida > 0) {
            Producto* producto = muestras[i % cantidadObtenida];
            nombresExistentes[i] = producto->nombre;
            codigosExistentes[i] = producto->codigoBarras;
        } else {
            // Fallback si no hay productos
            nombresExistentes[i] = "";
            codigosExistentes[i] = "";
        }
    }
}

// ============================================================================
// MEDICIÓN DE BÚSQUEDA EN LISTA NORMAL (NO ORDENADA)
// Complejidad teórica: O(n) - Búsqueda secuencial
//
// Algoritmo de medición:
// 1. Repetir M veces el experimento
// 2. En cada repetición: medir tiempo de ejecutar N búsquedas consecutivas
// 3. Acumular el tiempo total de todas las repeticiones
// 4. El promedio final se calcula dividiendo entre M
// ============================================================================
long long MedidorRendimiento::medirBusquedaListaNormal(ListaEnlazada& lista,
                                                       const std::string nombres[],
                                                       int cantidad) const {
    long long tiempoTotalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < M; ++repeticion) {
        // Iniciar cronómetro de alta resolución
        auto inicio = std::chrono::high_resolution_clock::now();

        // Ejecutar N búsquedas consecutivas
        for (int i = 0; i < cantidad; ++i) {
            // Suprimir resultado para evitar optimizaciones del compilador
            volatile Producto* resultado = lista.buscarPorNombre(nombres[i]);
            (void)resultado;
        }

        // Detener cronómetro y calcular duración
        auto fin = std::chrono::high_resolution_clock::now();
        tiempoTotalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return tiempoTotalMicrosegundos;
}

// ============================================================================
// MEDICIÓN DE BÚSQUEDA EN LISTA ORDENADA
// Complejidad teórica: O(n) - Búsqueda secuencial (aunque ordenada)
// Nota: La lista ordenada permite terminar antes si se pasa el elemento,
// pero en el peor caso sigue siendo O(n)
// ============================================================================
long long MedidorRendimiento::medirBusquedaListaOrdenada(ListaEnlazadaOrdenada& lista,
                                                         const std::string nombres[],
                                                         int cantidad) const {
    long long tiempoTotalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < M; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < cantidad; ++i) {
            volatile Producto* resultado = lista.buscarPorNombre(nombres[i]);
            (void)resultado;
        }

        auto fin = std::chrono::high_resolution_clock::now();
        tiempoTotalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return tiempoTotalMicrosegundos;
}

// ============================================================================
// MEDICIÓN DE BÚSQUEDA EN ÁRBOL AVL
// Complejidad teórica: O(log n) - Búsqueda binaria por estructura balanceada
// ============================================================================
long long MedidorRendimiento::medirBusquedaAVL(ArbolAVL& arbol,
                                               const std::string nombres[],
                                               int cantidad) const {
    long long tiempoTotalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < M; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < cantidad; ++i) {
            volatile Producto* resultado = arbol.buscarPorNombre(nombres[i]);
            (void)resultado;
        }

        auto fin = std::chrono::high_resolution_clock::now();
        tiempoTotalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return tiempoTotalMicrosegundos;
}

// ============================================================================
// MEDICIÓN DE BÚSQUEDA EN TABLA HASH
// Complejidad teórica: O(1) promedio, O(n) peor caso con colisiones
// ============================================================================
long long MedidorRendimiento::medirBusquedaHash(TablaHash& tabla,
                                                const std::string codigos[],
                                                int cantidad) const {
    long long tiempoTotalMicrosegundos = 0;

    for (int repeticion = 0; repeticion < M; ++repeticion) {
        auto inicio = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < cantidad; ++i) {
            volatile Producto* resultado = tabla.buscarPorCodigoBarras(codigos[i]);
            (void)resultado;
        }

        auto fin = std::chrono::high_resolution_clock::now();
        tiempoTotalMicrosegundos +=
            std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    }

    return tiempoTotalMicrosegundos;
}

// ============================================================================
// MÉTODOS DE PRESENTACIÓN DE RESULTADOS
// ============================================================================

void MedidorRendimiento::imprimirResumenMetodologia() const {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║            BENCHMARKING DE BUSQUEDAS - CATALOGO DE PRODUCTOS                 ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  Metodologia segun enunciado:                                                ║\n";
    std::cout << "║  - N = " << std::setw(2) << N << " consultas por prueba                                             ║\n";
    std::cout << "║  - M = " << std::setw(2) << M << " repeticiones del experimento                                     ║\n";
    std::cout << "║  - Tiempo promedio = (Suma de M mediciones) / M                              ║\n";
    std::cout << "║  - Medicion = tiempo de ejecutar N busquedas consecutivas                    ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";
}

void MedidorRendimiento::imprimirEncabezadoTabla() const {
    std::cout << "\n";
    std::cout << "┌────────────────────┬────────────────┬─────────────┬──────────────────────┐\n";
    std::cout << "│";
    std::cout << std::left << std::setw(20) << " Estructura";
    std::cout << "│";
    std::cout << std::setw(16) << " Tipo Busqueda";
    std::cout << "│";
    std::cout << std::setw(13) << " Complejidad";
    std::cout << "│";
    std::cout << std::setw(22) << " Tiempo Prom. (us)";
    std::cout << "│\n";
    std::cout << "├────────────────────┼────────────────┼─────────────┼──────────────────────┤\n";
}

void MedidorRendimiento::imprimirSeparador() const {
    std::cout << "├────────────────────┼────────────────┼─────────────┼──────────────────────┤\n";
}

void MedidorRendimiento::imprimirFilaTabla(const std::string& estructura,
                                           const std::string& tipoBusqueda,
                                           const std::string& complejidad,
                                           double tiempoPromedioMicrosegundos) const {
    std::cout << "│ ";
    std::cout << std::left << std::setw(19) << estructura;
    std::cout << "│ ";
    std::cout << std::setw(15) << tipoBusqueda;
    std::cout << "│ ";
    std::cout << std::setw(12) << complejidad;
    std::cout << "│ ";
    std::cout << std::right << std::setw(18) << std::fixed << std::setprecision(2) 
              << tiempoPromedioMicrosegundos << " us";
    std::cout << " │\n";
}

// ============================================================================
// MÉTODO PRINCIPAL: ejecutarPruebasBusqueda
//
// Ejecuta el benchmarking completo según el enunciado:
// 1. Inicializa los datos de prueba desde el catálogo cargado
// 2. Mide tiempos para cada estructura (Lista Normal, Lista Ordenada, AVL, Hash)
// 3. Mide tanto búsquedas exitosas como fallidas
// 4. Calcula promedios y presenta resultados en tabla ASCII
// ============================================================================
void MedidorRendimiento::ejecutarPruebasBusqueda(ListaEnlazada& listaNormal,
                                                 ListaEnlazadaOrdenada& listaOrdenada,
                                                 ArbolAVL& arbolAVL,
                                                 TablaHash& tablaHash) {
    // Verificar que hay datos cargados
    if (listaNormal.estaVacia()) {
        std::cout << "\n[ERROR] No hay productos cargados para ejecutar el benchmarking.\n";
        std::cout << "Por favor, cargue primero un archivo CSV con productos.\n\n";
        return;
    }

    // Inicializar datos de prueba desde el catálogo real
    inicializarDatosPrueba(listaNormal);

    // Imprimir metodología
    imprimirResumenMetodologia();

    // ========================================================================
    // MEDICIONES: Ejecutar M repeticiones de N búsquedas cada una
    // ========================================================================

    // Lista Normal (No Ordenada) - Búsqueda secuencial O(n)
    long long tiempoListaNormalExitosa = medirBusquedaListaNormal(
        listaNormal, nombresExistentes, N);
    long long tiempoListaNormalFallida = medirBusquedaListaNormal(
        listaNormal, nombresInexistentes, N);

    // Lista Ordenada - Búsqueda secuencial mejorada O(n)
    long long tiempoListaOrdenadaExitosa = medirBusquedaListaOrdenada(
        listaOrdenada, nombresExistentes, N);
    long long tiempoListaOrdenadaFallida = medirBusquedaListaOrdenada(
        listaOrdenada, nombresInexistentes, N);

    // Árbol AVL - Búsqueda binaria O(log n)
    long long tiempoAVLExitosa = medirBusquedaAVL(arbolAVL, nombresExistentes, N);
    long long tiempoAVLFallida = medirBusquedaAVL(arbolAVL, nombresInexistentes, N);

    // Tabla Hash - Búsqueda directa O(1) promedio
    long long tiempoHashExitosa = medirBusquedaHash(tablaHash, codigosExistentes, N);
    long long tiempoHashFallida = medirBusquedaHash(tablaHash, codigosInexistentes, N);

    // ========================================================================
    // CÁLCULO DE PROMEDIOS
    // Promedio = Tiempo total acumulado / M repeticiones
    // ========================================================================
    const double divisorM = static_cast<double>(M);

    double promedioListaNormalExitosa = tiempoListaNormalExitosa / divisorM;
    double promedioListaNormalFallida = tiempoListaNormalFallida / divisorM;
    double promedioListaOrdenadaExitosa = tiempoListaOrdenadaExitosa / divisorM;
    double promedioListaOrdenadaFallida = tiempoListaOrdenadaFallida / divisorM;
    double promedioAVLExitosa = tiempoAVLExitosa / divisorM;
    double promedioAVLFallida = tiempoAVLFallida / divisorM;
    double promedioHashExitosa = tiempoHashExitosa / divisorM;
    double promedioHashFallida = tiempoHashFallida / divisorM;

    // ========================================================================
    // PRESENTACIÓN DE RESULTADOS
    // ========================================================================
    imprimirEncabezadoTabla();

    // Lista Normal (No Ordenada)
    imprimirFilaTabla("Lista Normal", "Exitosa", "O(n)", promedioListaNormalExitosa);
    imprimirFilaTabla("Lista Normal", "Fallida", "O(n)", promedioListaNormalFallida);
    imprimirSeparador();

    // Lista Ordenada
    imprimirFilaTabla("Lista Ordenada", "Exitosa", "O(n)", promedioListaOrdenadaExitosa);
    imprimirFilaTabla("Lista Ordenada", "Fallida", "O(n)", promedioListaOrdenadaFallida);
    imprimirSeparador();

    // Árbol AVL
    imprimirFilaTabla("Arbol AVL", "Exitosa", "O(log n)", promedioAVLExitosa);
    imprimirFilaTabla("Arbol AVL", "Fallida", "O(log n)", promedioAVLFallida);
    imprimirSeparador();

    // Tabla Hash
    imprimirFilaTabla("Tabla Hash", "Exitosa", "O(1)*", promedioHashExitosa);
    imprimirFilaTabla("Tabla Hash", "Fallida", "O(1)*", promedioHashFallida);

    std::cout << "└────────────────────┴────────────────┴─────────────┴──────────────────────┘\n";

    // Notas al pie
    std::cout << "\n";
    std::cout << "  Notas:\n";
    std::cout << "  - *O(1) es el caso promedio para Tabla Hash; peor caso es O(n) con colisiones.\n";
    std::cout << "  - Tiempos en microsegundos (us). 1 ms = 1000 us.\n";
    std::cout << "  - Cada medicion ejecuta " << N << " busquedas consecutivas.\n";
    std::cout << "  - El promedio se calcula sobre " << M << " repeticiones del experimento.\n";
    std::cout << "\n";

    // ========================================================================
    // ANÁLISIS COMPARATIVO
    // ========================================================================
    std::cout << "┌──────────────────────────────────────────────────────────────────────────────┐\n";
    std::cout << "│                           ANALISIS COMPARATIVO                               │\n";
    std::cout << "├──────────────────────────────────────────────────────────────────────────────┤\n";

    // Encontrar el más rápido para búsqueda exitosa
    double tiempos[4] = {promedioListaNormalExitosa, promedioListaOrdenadaExitosa, 
                         promedioAVLExitosa, promedioHashExitosa};
    const char* nombres[4] = {"Lista Normal", "Lista Ordenada", "Arbol AVL", "Tabla Hash"};
    
    int indiceMinimo = 0;
    for (int i = 1; i < 4; ++i) {
        if (tiempos[i] < tiempos[indiceMinimo]) {
            indiceMinimo = i;
        }
    }

    std::cout << "│  Estructura mas rapida (busqueda exitosa): " 
              << std::left << std::setw(32) << nombres[indiceMinimo] << "│\n";

    // Calcular speedup vs Lista Normal
    if (promedioListaNormalExitosa > 0) {
        double speedupAVL = promedioListaNormalExitosa / promedioAVLExitosa;
        double speedupHash = promedioListaNormalExitosa / promedioHashExitosa;
        
        std::cout << "│  Speedup AVL vs Lista Normal: " 
                  << std::right << std::setw(8) << std::fixed << std::setprecision(2) 
                  << speedupAVL << "x" << std::setw(36) << " " << "│\n";
        std::cout << "│  Speedup Hash vs Lista Normal: " 
                  << std::right << std::setw(7) << std::fixed << std::setprecision(2) 
                  << speedupHash << "x" << std::setw(36) << " " << "│\n";
    }

    std::cout << "└──────────────────────────────────────────────────────────────────────────────┘\n\n";
}
