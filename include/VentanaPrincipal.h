#ifndef VENTANA_PRINCIPAL_H
#define VENTANA_PRINCIPAL_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QFrame>
#include <QLabel>
#include <QSpacerItem>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QDialog>
#include <QFormLayout>
#include <QDir>
#include <vector>

// Incluir estructuras de datos del backend
#include "ListaEnlazada.h"
#include "ListaEnlazadaOrdenada.h"
#include "TablaHash.h"
#include "ArbolAVL.h"
#include "ArbolB.h"
#include "ArbolBPlus.h"
#include "CargadorCSV.h"
#include "MedidorRendimiento.h"

// =============================================================================
// VentanaPrincipal: Ventana principal de la aplicación con tema FC Barcelona
// Hereda de QMainWindow para proporcionar menús, barras de herramientas, etc.
// =============================================================================
class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

public:
    // Constructor que recibe referencias a todas las estructuras de datos
    explicit VentanaPrincipal(
        ListaEnlazada& listaNormal,
        ListaEnlazadaOrdenada& listaOrdenada,
        TablaHash& tablaHash,
        ArbolAVL& arbolAVL,
        ArbolB& arbolB,
        ArbolBPlus& arbolBPlus,
        QWidget* parent = nullptr
    );
    ~VentanaPrincipal();

private slots:
    // =========================================================================
    // Slots para manejar eventos de botones
    // =========================================================================
    void onCargarCSV();
    void onInsertarProducto();
    void onEliminarProducto();
    void onBuscarProducto();
    void onGenerarReportes();
    void onBenchmarking();
    void actualizarTabla();
    
    // Nuevos slots para búsqueda avanzada (Fase 7C)
    void ejecutarBusquedaAvanzada();
    void onCambioFiltroBusqueda(int indice);

private:
    // =========================================================================
    // Widgets de la interfaz
    // =========================================================================
    
    // Widget central y layout principal
    QWidget* widgetCentral;
    QHBoxLayout* layoutPrincipal;

    // Panel izquierdo (Sidebar)
    QFrame* panelSidebar;
    QVBoxLayout* layoutSidebar;
    QLabel* lblTitulo;

    // Botones del sidebar
    QPushButton* btnCargarCSV;
    QPushButton* btnInsertar;
    QPushButton* btnEliminar;
    QPushButton* btnBuscar;
    QPushButton* btnReportes;
    QPushButton* btnBenchmarking;

    // Panel derecho (Área de contenido)
    QWidget* panelContenido;
    QVBoxLayout* layoutContenido;
    
    // Barra de búsqueda avanzada
    QWidget* barraBusqueda;
    QHBoxLayout* layoutBarraBusqueda;
    QLabel* lblBuscarPor;
    QComboBox* comboFiltroBusqueda;
    QLineEdit* inputBusqueda;
    QLineEdit* inputBusquedaFin;  // Para rango de fechas
    QPushButton* btnEjecutarBusqueda;
    QPushButton* btnMostrarTodos;
    
    // Tabla de productos
    QTableWidget* tablaProductos;

    // =========================================================================
    // Referencias a estructuras de datos del backend
    // =========================================================================
    ListaEnlazada& refListaNormal;
    ListaEnlazadaOrdenada& refListaOrdenada;
    TablaHash& refTablaHash;
    ArbolAVL& refArbolAVL;
    ArbolB& refArbolB;
    ArbolBPlus& refArbolBPlus;
    
    // Cargador de CSV (propio de la ventana)
    CargadorCSV cargadorCSV;

    // =========================================================================
    // Métodos de inicialización
    // =========================================================================
    void configurarInterfaz();
    void configurarSidebar();
    void configurarBarraBusqueda();
    void configurarTabla();
    void aplicarEstilos();
    void conectarSenales();
    
    // Método auxiliar para mostrar productos en la tabla
    void mostrarProductosEnTabla(const std::vector<Producto*>& productos);
};

#endif // VENTANA_PRINCIPAL_H
