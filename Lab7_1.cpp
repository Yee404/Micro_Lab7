//hilo 1 - ventas de producto (multi)
//hilo 2 - utilidad producto (multi)
//hilo 3 - utilidad producto (resta)
//hilo 4 - total venta
//hilo 5 - total utilidad producto
//hilo 6 - ventas de producto 2 (multi)
//hilo 7 - utilidad producto 2 (multi)

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <unistd.h>

const int NumProductos = 8;
using namespace std;
mutex mtx;

// Productos
string productos[NumProductos] = {
    "Porcion pastel de chocolate", "White mocha", "Café americano 8onz", "Latte 8onz", "Toffee coffee", "Cappuccino 8onz", "S'mores Latte", "Café tostado molido"
};

// Arrays de datos
// Precio unitario productos
double precioUnitario[NumProductos] = {60.00, 32.00, 22.00, 24.00, 28.00, 24.00, 32.00, 60.00};

// Costo fijo por producto
double costoFijo[NumProductos] = {20.00, 19.20, 13.20, 17.20, 20.10, 17.20, 23.00, 20.00};

// Unidades vendidas en julio
int unidadesVendidasJ[NumProductos] = {300, 400, 1590, 200, 390, 1455, 800, 60};

// Unidades vendidas en agosto
int unidadesVendidasA[NumProductos] = {250, 380, 800, 250, 600, 1200, 1540, 15};

// Arrays para resultados de cálculo
double ventasProducto[NumProductos];
double utilidadProducto[NumProductos];

double totalVentas = 0;
double totalUtilidad = 0;

// CÁLCULOS
void calcular(int id) {
    {
        lock_guard<mutex> lock(mtx);
        cout << "Cálculo hilo " << id << " iniciado" << endl;
    }
    
    sleep(1);

    // Ventas y Utilidad por producto (Julio)
    ventasProducto[id] = unidadesVendidasJ[id] * precioUnitario[id];
    utilidadProducto[id] = ventasProducto[id] - (unidadesVendidasJ[id] * costoFijo[id]);

    // Actualizacion Totales
    {
        lock_guard<mutex> lock(mtx);  // Protege las variables compartidas
        totalVentas += ventasProducto[id];
        totalUtilidad += utilidadProducto[id];
    }

    {
        lock_guard<mutex> lock(mtx);
        cout << "Cálculo hilo " << id << " terminado" << endl;
    }
}

int main() {
    vector<thread> hilos;

    for (int i = 0; i < NumProductos; i++) {
        hilos.push_back(thread(calcular, i));
    }

    // Espera a que todos los hilos terminen
    for (auto& hilo : hilos) {
        hilo.join();
    }

    cout << "REPORTE DEL MES DE JULIO" << endl;
    cout << "__Monto Ventas por Producto________" << endl;
    for (int i = 0; i < NumProductos; i++) {
        cout << productos[i] << ": Q" << ventasProducto[i] << endl;
    }
    cout << "__Utilidad por Producto____________" << endl;
    for (int i = 0; i < NumProductos; i++) {
        cout << productos[i] << ": Q" << utilidadProducto[i] << endl;
    }
    cout << "__Monto Total Ventas del Mes:\nQ " << totalVentas << endl;
    cout << "__Utilidad del mes:\nQ" << totalUtilidad << endl;

    return 0;
}
