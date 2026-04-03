#include "CargadorCSV.h"

#include <iostream>
#include <sstream> //esta libreria se encarga de manejar strings y flujos de datos

#include "Producto.h"
//esta clase realiza la carga de datos desde un archivo CSV

void CargadorCSV::cargar(std::string rutaArchivo,
                         ListaEnlazada& listaNormal,
                         ListaEnlazadaOrdenada& listaOrdenada,
                         TablaHash& tabla, //esto en teoria pero no jalo xd 
                         ArbolAVL& arbolAVL,
                         ArbolB& arbolB,
                         ArbolBPlus& arbolBPlus) {
    std::ifstream archivoCSV(rutaArchivo);
    if (!archivoCSV.is_open()) {
        std::cout << "Error: no se pudo abrir el archivo '" << rutaArchivo << "'.\n";
        return;
    }

    std::ofstream archivoErrores("errors.log", std::ios::app);
    if (!archivoErrores.is_open()) {
        std::cout << "Error: no se pudo abrir 'errors.log' para registrar errores.\n";
        return;
    }

    std::string linea;
    int numeroLinea = 0;
    int totalInsertados = 0;
    int totalErrores = 0;

    while (std::getline(archivoCSV, linea)) {
        ++numeroLinea;

        if (linea.empty()) {
            registrarError(archivoErrores, numeroLinea, "Linea vacia", linea);
            ++totalErrores;
            continue;
        }

        std::string campos[7];
        if (!dividirLineaCSV(linea, campos)) {
            registrarError(archivoErrores, numeroLinea, "Linea malformada", linea);
            ++totalErrores;
            continue;
        }

        if (esCabecera(campos)) {
            continue;
        }

        if (tabla.buscarPorCodigoBarras(campos[1]) != nullptr) {
            registrarError(archivoErrores, numeroLinea, "Codigo de barras duplicado", linea);
            ++totalErrores;
            continue;
        }

        try {
            std::size_t caracteresConvertidosPrecio = 0;
            std::size_t caracteresConvertidosStock = 0;

            double precio = std::stod(campos[5], &caracteresConvertidosPrecio);
            int stock = std::stoi(campos[6], &caracteresConvertidosStock);

            if (caracteresConvertidosPrecio != campos[5].size() ||
                caracteresConvertidosStock != campos[6].size()) {
                throw std::invalid_argument("Valor numerico invalido");
            }

            Producto* nuevoProducto = new Producto(campos[0],
                                                   campos[1],
                                                   campos[2],
                                                   campos[3],
                                                   campos[4],
                                                   precio,
                                                   stock);

            listaNormal.insertar(nuevoProducto);
            listaOrdenada.insertarOrdenado(nuevoProducto);

            if (!tabla.insertar(nuevoProducto)) {
                listaNormal.eliminarPorCodigoBarras(nuevoProducto->codigoBarras);
                listaOrdenada.eliminarPorCodigoBarras(nuevoProducto->codigoBarras);
                registrarError(archivoErrores, numeroLinea, "Fallo insercion en tabla hash", linea);
                delete nuevoProducto;
                ++totalErrores;
                continue;
            }

            arbolAVL.insertar(nuevoProducto);
            arbolB.insertar(nuevoProducto);
            arbolBPlus.insertar(nuevoProducto);

            ++totalInsertados;
        } catch (const std::exception&) {
            registrarError(archivoErrores,
                           numeroLinea,
                           "Precio o stock no numerico",
                           linea);
            ++totalErrores;
            continue;
        }
    }

    std::cout << "Carga finalizada. Insertados: " << totalInsertados
              << ", errores: " << totalErrores << ".\n";
}

bool CargadorCSV::dividirLineaCSV(const std::string& linea, std::string campos[7]) const {
    // Parseo CSV con stringstream + getline(','): si un fragmento abre comillas
    // y no las cierra, se concatena con el siguiente fragmento hasta cerrarlas.
    std::stringstream flujo(linea);
    std::string fragmento;
    std::string campoAcumulado;
    bool comillasAbiertas = false;
    int indiceCampo = 0;

    while (std::getline(flujo, fragmento, ',')) {
        if (!campoAcumulado.empty()) {
            campoAcumulado += ",";
        }
        campoAcumulado += fragmento;

        int comillasEnFragmento = 0;
        for (char caracter : fragmento) {
            if (caracter == '"') {
                ++comillasEnFragmento;
            }
        }

        if (comillasEnFragmento % 2 != 0) {
            comillasAbiertas = !comillasAbiertas;
        }

        if (!comillasAbiertas) {
            if (indiceCampo >= 7) {
                return false;
            }
            campos[indiceCampo] = limpiarCampo(campoAcumulado);
            campoAcumulado.clear();
            ++indiceCampo;
        }
    }

    if (comillasAbiertas || !campoAcumulado.empty()) {
        return false;
    }

    return indiceCampo == 7;
}

std::string CargadorCSV::limpiarCampo(const std::string& campo) const {
    if (campo.empty()) {
        return campo;
    }

    std::size_t inicio = 0;
    std::size_t fin = campo.size();

    while (inicio < fin &&
           (campo[inicio] == ' ' || campo[inicio] == '\t' || campo[inicio] == '\r')) {
        ++inicio;
    }

    while (fin > inicio &&
           (campo[fin - 1] == ' ' || campo[fin - 1] == '\t' || campo[fin - 1] == '\r')) {
        --fin;
    }

    if (inicio >= fin) {
        return "";
    }

    std::string limpio = campo.substr(inicio, fin - inicio);

    if (limpio.size() >= 2 && limpio.front() == '"' && limpio.back() == '"') {
        limpio = limpio.substr(1, limpio.size() - 2);
    }

    return limpio;
}

std::string CargadorCSV::normalizarTexto(const std::string& texto) const {
    std::string resultado = texto;
    for (char& caracter : resultado) {
        if (caracter >= 'A' && caracter <= 'Z') {
            caracter = static_cast<char>(caracter + ('a' - 'A'));
        } else if (caracter == ' ') {
            caracter = '_';
        }
    }
    return resultado;
}

bool CargadorCSV::esCabecera(const std::string campos[7]) const {
    const std::string nombre = normalizarTexto(campos[0]);
    const std::string codigo = normalizarTexto(campos[1]);
    const std::string categoria = normalizarTexto(campos[2]);
    const std::string fecha = normalizarTexto(campos[3]);
    const std::string marca = normalizarTexto(campos[4]);
    const std::string precio = normalizarTexto(campos[5]);
    const std::string stock = normalizarTexto(campos[6]);

    return (nombre == "nombre" &&
            codigo == "codigobarra") &&
           (categoria == "categoria" &&
            (fecha == "fecha_caducidad" || fecha == "fechacaducidad")) &&
           (marca == "marca" && precio == "precio" && stock == "stock");
}

void CargadorCSV::registrarError(std::ofstream& archivoErrores,
                                 int numeroLinea,
                                 const std::string& motivo,
                                 const std::string& lineaOriginal) const {
    archivoErrores << "Linea " << numeroLinea << " | " << motivo
                   << " | Contenido: " << lineaOriginal << '\n';
}
