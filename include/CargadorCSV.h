#ifndef CARGADOR_CSV_H
#define CARGADOR_CSV_H

#include <fstream>
#include <string>

#include "ArbolAVL.h"
#include "ArbolB.h"
#include "ArbolBPlus.h"
#include "ListaEnlazada.h"
#include "ListaEnlazadaOrdenada.h"
#include "TablaHash.h"

class CargadorCSV {
public:
    void cargar(std::string rutaArchivo,
                ListaEnlazada& listaNormal,
                ListaEnlazadaOrdenada& listaOrdenada,
                TablaHash& tabla,
                ArbolAVL& arbolAVL,
                ArbolB& arbolB,
                ArbolBPlus& arbolBPlus);

private:
    bool dividirLineaCSV(const std::string& linea, std::string campos[7]) const;
    std::string limpiarCampo(const std::string& campo) const;
    std::string normalizarTexto(const std::string& texto) const;
    bool esCabecera(const std::string campos[7]) const;
    void registrarError(std::ofstream& archivoErrores,
                        int numeroLinea,
                        const std::string& motivo,
                        const std::string& lineaOriginal) const;
};

#endif
