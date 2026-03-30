#include "ArbolBPlus.h"

#include <cstdlib>
#include <iostream>

// ==================== ClaveCategoria ====================

ClaveCategoria::ClaveCategoria() : categoria(""), productos(nullptr) {}

ClaveCategoria::~ClaveCategoria() {
    // Solo libera la lista si existe (evita doble delete)
    if (productos != nullptr) {
        delete productos;
        productos = nullptr;
    }
}

// ==================== NodoBPlus ====================

NodoBPlus::NodoBPlus(int t, bool hoja)
    : claves(nullptr), hijos(nullptr), numClaves(0), esHoja(hoja),
      gradoMinimo(t), siguiente(nullptr) {
    int maxClaves = 2 * t - 1;
    int maxHijos = 2 * t;

    claves = new ClaveCategoria[maxClaves];
    hijos = new NodoBPlus*[maxHijos];

    for (int i = 0; i < maxHijos; ++i) {
        hijos[i] = nullptr;
    }
}

NodoBPlus::~NodoBPlus() {
    // En nodos internos, productos es nullptr (no hay doble delete)
    delete[] claves;
    claves = nullptr;

    delete[] hijos;
    hijos = nullptr;
}

// ==================== ArbolBPlus ====================

ArbolBPlus::ArbolBPlus(int t) : raiz(nullptr), gradoMinimo(t) {}

ArbolBPlus::~ArbolBPlus() {
    destruirRecursivo(raiz);
    raiz = nullptr;
}

void ArbolBPlus::destruirRecursivo(NodoBPlus* nodo) {
    if (nodo == nullptr) {
        return;
    }

    if (!nodo->esHoja) {
        for (int i = 0; i <= nodo->numClaves; ++i) {
            destruirRecursivo(nodo->hijos[i]);
        }
    }

    delete nodo;
}

bool ArbolBPlus::estaVacio() const {
    return raiz == nullptr;
}

NodoBPlus* ArbolBPlus::buscarHoja(NodoBPlus* nodo, const std::string& categoria) const {
    if (nodo == nullptr) {
        return nullptr;
    }

    if (nodo->esHoja) {
        return nodo;
    }

    // Buscar el hijo correcto
    int i = 0;
    while (i < nodo->numClaves && categoria >= nodo->claves[i].categoria) {
        ++i;
    }

    return buscarHoja(nodo->hijos[i], categoria);
}

ClaveCategoria* ArbolBPlus::buscarClaveEnHoja(NodoBPlus* hoja, const std::string& categoria) const {
    if (hoja == nullptr || !hoja->esHoja) {
        return nullptr;
    }

    for (int i = 0; i < hoja->numClaves; ++i) {
        if (hoja->claves[i].categoria == categoria) {
            return &(hoja->claves[i]);
        }
    }

    return nullptr;
}

void ArbolBPlus::insertar(Producto* producto) {
    if (producto == nullptr) {
        return;
    }

    const std::string& categoria = producto->categoria;

    // Si el árbol está vacío, crear raíz hoja
    if (raiz == nullptr) {
        raiz = new NodoBPlus(gradoMinimo, true);
        raiz->claves[0].categoria = categoria;
        raiz->claves[0].productos = new ListaEnlazada();
        raiz->claves[0].productos->insertar(producto);
        raiz->numClaves = 1;
        return;
    }

    // Buscar la hoja donde debería estar la categoría
    NodoBPlus* hoja = buscarHoja(raiz, categoria);
    ClaveCategoria* claveExistente = buscarClaveEnHoja(hoja, categoria);

    if (claveExistente != nullptr) {
        // La categoría ya existe: agregar producto a su lista
        claveExistente->productos->insertar(producto);
        return;
    }

    // La categoría no existe: insertar nueva clave
    if (raiz->numClaves == 2 * gradoMinimo - 1) {
        // La raíz está llena, dividirla
        NodoBPlus* nuevaRaiz = new NodoBPlus(gradoMinimo, false);
        nuevaRaiz->hijos[0] = raiz;
        dividirHijo(nuevaRaiz, 0, raiz);

        // Decidir en qué hijo insertar
        int i = 0;
        if (nuevaRaiz->claves[0].categoria < categoria) {
            ++i;
        }
        insertarEnNodoNoLleno(nuevaRaiz->hijos[i], producto);
        raiz = nuevaRaiz;
    } else {
        insertarEnNodoNoLleno(raiz, producto);
    }
}

void ArbolBPlus::insertarEnNodoNoLleno(NodoBPlus* nodo, Producto* producto) {
    const std::string& categoria = producto->categoria;
    int i = nodo->numClaves - 1;

    if (nodo->esHoja) {
        // Verificar si ya existe la categoría
        for (int j = 0; j < nodo->numClaves; ++j) {
            if (nodo->claves[j].categoria == categoria) {
                nodo->claves[j].productos->insertar(producto);
                return;
            }
        }

        // Mover claves mayores hacia la derecha
        while (i >= 0 && nodo->claves[i].categoria > categoria) {
            nodo->claves[i + 1].categoria = nodo->claves[i].categoria;
            nodo->claves[i + 1].productos = nodo->claves[i].productos;
            nodo->claves[i].categoria = "";
            nodo->claves[i].productos = nullptr;
            --i;
        }

        // Insertar nueva clave
        nodo->claves[i + 1].categoria = categoria;
        nodo->claves[i + 1].productos = new ListaEnlazada();
        nodo->claves[i + 1].productos->insertar(producto);
        ++(nodo->numClaves);
    } else {
        // Encontrar el hijo donde insertar
        while (i >= 0 && nodo->claves[i].categoria > categoria) {
            --i;
        }
        ++i;

        // Si el hijo está lleno, dividirlo
        if (nodo->hijos[i]->numClaves == 2 * gradoMinimo - 1) {
            dividirHijo(nodo, i, nodo->hijos[i]);

            // Después de dividir, decidir cuál hijo usar
            if (nodo->claves[i].categoria < categoria) {
                ++i;
            }
        }

        insertarEnNodoNoLleno(nodo->hijos[i], producto);
    }
}

void ArbolBPlus::dividirHijo(NodoBPlus* padre, int indice, NodoBPlus* hijo) {
    int t = gradoMinimo;

    NodoBPlus* nuevoNodo = new NodoBPlus(t, hijo->esHoja);
    nuevoNodo->numClaves = t - 1;

    if (hijo->esHoja) {
        // DIVISIÓN DE HOJA: copiar las últimas t claves al nuevo nodo
        for (int j = 0; j < t; ++j) {
            nuevoNodo->claves[j].categoria = hijo->claves[j + t - 1].categoria;
            nuevoNodo->claves[j].productos = hijo->claves[j + t - 1].productos;
            hijo->claves[j + t - 1].categoria = "";
            hijo->claves[j + t - 1].productos = nullptr;
        }
        nuevoNodo->numClaves = t;
        hijo->numClaves = t - 1;

        // Actualizar enlace entre hojas
        nuevoNodo->siguiente = hijo->siguiente;
        hijo->siguiente = nuevoNodo;

        // Mover hijos del padre para hacer espacio
        for (int j = padre->numClaves; j >= indice + 1; --j) {
            padre->hijos[j + 1] = padre->hijos[j];
        }
        padre->hijos[indice + 1] = nuevoNodo;

        // Mover claves del padre para hacer espacio
        for (int j = padre->numClaves - 1; j >= indice; --j) {
            padre->claves[j + 1].categoria = padre->claves[j].categoria;
            padre->claves[j + 1].productos = padre->claves[j].productos;
            padre->claves[j].categoria = "";
            padre->claves[j].productos = nullptr;
        }

        // Subir copia de la primera clave del nuevo nodo (B+ copia, no mueve)
        padre->claves[indice].categoria = nuevoNodo->claves[0].categoria;
        padre->claves[indice].productos = nullptr;  // Nodos internos no tienen datos
        ++(padre->numClaves);
    } else {
        // DIVISIÓN DE NODO INTERNO: igual que B-tree
        for (int j = 0; j < t - 1; ++j) {
            nuevoNodo->claves[j].categoria = hijo->claves[j + t].categoria;
            nuevoNodo->claves[j].productos = hijo->claves[j + t].productos;
            hijo->claves[j + t].categoria = "";
            hijo->claves[j + t].productos = nullptr;
        }

        if (!hijo->esHoja) {
            for (int j = 0; j < t; ++j) {
                nuevoNodo->hijos[j] = hijo->hijos[j + t];
                hijo->hijos[j + t] = nullptr;
            }
        }

        hijo->numClaves = t - 1;

        for (int j = padre->numClaves; j >= indice + 1; --j) {
            padre->hijos[j + 1] = padre->hijos[j];
        }
        padre->hijos[indice + 1] = nuevoNodo;

        for (int j = padre->numClaves - 1; j >= indice; --j) {
            padre->claves[j + 1].categoria = padre->claves[j].categoria;
            padre->claves[j + 1].productos = padre->claves[j].productos;
            padre->claves[j].categoria = "";
            padre->claves[j].productos = nullptr;
        }

        // Subir la clave mediana al padre
        padre->claves[indice].categoria = hijo->claves[t - 1].categoria;
        padre->claves[indice].productos = nullptr;
        hijo->claves[t - 1].categoria = "";
        hijo->claves[t - 1].productos = nullptr;

        ++(padre->numClaves);
    }
}

void ArbolBPlus::buscarPorCategoria(const std::string& categoria) const {
    if (raiz == nullptr) {
        std::cout << "El arbol B+ esta vacio.\n";
        return;
    }

    NodoBPlus* hoja = buscarHoja(raiz, categoria);
    ClaveCategoria* clave = buscarClaveEnHoja(hoja, categoria);

    if (clave == nullptr) {
        std::cout << "No se encontraron productos en la categoria '" << categoria << "'.\n";
        return;
    }

    std::cout << "=== Productos en categoria: " << categoria << " ===\n\n";
    if (clave->productos != nullptr) {
        clave->productos->mostrarTodos();
    }
}

std::string ArbolBPlus::escaparTexto(const std::string& texto) const {
    std::string resultado;
    for (char c : texto) {
        if (c == '"' || c == '\\' || c == '<' || c == '>' || c == '|') {
            resultado += '\\';
        }
        resultado += c;
    }
    return resultado;
}

void ArbolBPlus::generarDotRecursivo(NodoBPlus* nodo, std::ofstream& archivo, int& contadorNodo) const {
    if (nodo == nullptr) {
        return;
    }

    int idActual = contadorNodo;

    // Generar etiqueta del nodo
    if (nodo->esHoja) {
        archivo << "    nodo" << idActual << " [label=\"";
        for (int i = 0; i < nodo->numClaves; ++i) {
            if (i > 0) archivo << "|";
            archivo << "<f" << i << "> " << escaparTexto(nodo->claves[i].categoria);
        }
        archivo << "\", fillcolor=lightgreen];\n";
    } else {
        archivo << "    nodo" << idActual << " [label=\"";
        for (int i = 0; i < nodo->numClaves; ++i) {
            if (i > 0) archivo << "|";
            archivo << "<f" << i << "> " << escaparTexto(nodo->claves[i].categoria);
        }
        archivo << "\", fillcolor=lightyellow];\n";
    }

    // Generar conexiones a hijos
    if (!nodo->esHoja) {
        for (int i = 0; i <= nodo->numClaves; ++i) {
            if (nodo->hijos[i] != nullptr) {
                ++contadorNodo;
                int idHijo = contadorNodo;
                archivo << "    nodo" << idActual << " -> nodo" << idHijo << ";\n";
                generarDotRecursivo(nodo->hijos[i], archivo, contadorNodo);
            }
        }
    }
}

void ArbolBPlus::generarEnlacesHojas(std::ofstream& archivo) const {
    if (raiz == nullptr) {
        return;
    }

    // Encontrar la primera hoja siguiendo siempre el hijo izquierdo
    NodoBPlus* primeraHoja = raiz;
    while (primeraHoja != nullptr && !primeraHoja->esHoja) {
        primeraHoja = primeraHoja->hijos[0];
    }

    if (primeraHoja == nullptr || primeraHoja->siguiente == nullptr) {
        return;
    }

    // Mapear cada nodo a su ID mediante recorrido en preorden (igual que generarDotRecursivo)
    NodoBPlus* mapaNodos[1000];
    int numNodos = 0;

    NodoBPlus* pila[1000];
    int tope = 0;
    pila[tope++] = raiz;

    while (tope > 0) {
        NodoBPlus* nodo = pila[--tope];
        mapaNodos[numNodos++] = nodo;

        if (!nodo->esHoja) {
            for (int i = nodo->numClaves; i >= 0; --i) {
                if (nodo->hijos[i] != nullptr) {
                    pila[tope++] = nodo->hijos[i];
                }
            }
        }
    }

    // Generar subgrafo para alinear hojas y mostrar enlaces
    archivo << "\n    // Subgrafo para alinear hojas horizontalmente\n";
    archivo << "    { rank=same; ";
    NodoBPlus* hoja = primeraHoja;
    while (hoja != nullptr) {
        for (int i = 0; i < numNodos; ++i) {
            if (mapaNodos[i] == hoja) {
                archivo << "nodo" << i << "; ";
                break;
            }
        }
        hoja = hoja->siguiente;
    }
    archivo << "}\n";

    // Generar enlaces entre hojas con estilo invisible para ordenamiento
    archivo << "\n    // Enlaces entre hojas (ordenamiento)\n";
    archivo << "    edge [style=invis, weight=10];\n";

    hoja = primeraHoja;
    while (hoja != nullptr && hoja->siguiente != nullptr) {
        int idActual = -1, idSiguiente = -1;
        for (int i = 0; i < numNodos; ++i) {
            if (mapaNodos[i] == hoja) idActual = i;
            if (mapaNodos[i] == hoja->siguiente) idSiguiente = i;
        }
        if (idActual >= 0 && idSiguiente >= 0) {
            archivo << "    nodo" << idActual << " -> nodo" << idSiguiente << ";\n";
        }
        hoja = hoja->siguiente;
    }

    // Generar enlaces visibles (punteados azules)
    archivo << "\n    // Enlaces visibles entre hojas\n";
    archivo << "    edge [style=dashed, color=blue, constraint=false, weight=0];\n";

    hoja = primeraHoja;
    while (hoja != nullptr && hoja->siguiente != nullptr) {
        int idActual = -1, idSiguiente = -1;
        for (int i = 0; i < numNodos; ++i) {
            if (mapaNodos[i] == hoja) idActual = i;
            if (mapaNodos[i] == hoja->siguiente) idSiguiente = i;
        }
        if (idActual >= 0 && idSiguiente >= 0) {
            archivo << "    nodo" << idActual << ":e -> nodo" << idSiguiente << ":w [label=\"sig\"];\n";
        }
        hoja = hoja->siguiente;
    }
}

void ArbolBPlus::generarDot(const std::string& rutaArchivo) const {
    std::ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cout << "Error: no se pudo crear el archivo DOT.\n";
        return;
    }

    archivo << "digraph ArbolBPlus {\n";
    archivo << "    node [shape=record, style=filled];\n";
    archivo << "    rankdir=TB;\n";

    if (raiz != nullptr) {
        int contador = 0;
        generarDotRecursivo(raiz, archivo, contador);
        generarEnlacesHojas(archivo);
    } else {
        archivo << "    vacio [label=\"Arbol B+ vacio\", shape=plaintext];\n";
    }

    archivo << "}\n";
    archivo.close();

    std::cout << "Archivo DOT generado: " << rutaArchivo << '\n';
}

void ArbolBPlus::generarImagen() const {
    generarDot("data/arbol_bplus.dot");
    std::system("dot -Tpng data/arbol_bplus.dot -o data/arbol_bplus.png 2>/dev/null");
    
    // Verificar si el archivo se creó
    std::ifstream archivo("data/arbol_bplus.png");
    if (archivo.good()) {
        std::cout << "Imagen PNG generada: data/arbol_bplus.png\n";
    } else {
        std::cout << "Error al generar imagen. Asegurese de tener Graphviz instalado.\n";
    }
}
