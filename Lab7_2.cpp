#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;
int silo = 0;
int bodega = 0;
const int LIMITE_SILO = 5;
const int TOTAL_BOLSAS = 400;

void tostadora1() {
    for (int i = 0; i < TOTAL_BOLSAS / 2; ++i) {
        this_thread::sleep_for(chrono::seconds(1));  // Simula el tiempo de producción
        unique_lock<mutex> lock(mtx);
        silo++;
        cout << "Tostadora 1 produjo: 1 lb de cafe tostado\n";
        cv.notify_all(); 
    }
}

void tostadora2() {
    for (int i = 0; i < TOTAL_BOLSAS / 2; ++i) {
        this_thread::sleep_for(chrono::seconds(1));  // Simula el tiempo de producción
        unique_lock<mutex> lock(mtx);
        silo++;
        cout << "Tostadora 2 produjo: 1 lb de cafe tostado\n";
        cv.notify_all();  
    }
}

void empacadora() {
    while (bodega < TOTAL_BOLSAS) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return silo >= LIMITE_SILO || bodega >= TOTAL_BOLSAS; });

        if (bodega >= TOTAL_BOLSAS) break;

        for (int i = 0; i < LIMITE_SILO && bodega < TOTAL_BOLSAS; ++i) {
            this_thread::sleep_for(chrono::seconds(1));  // Simula el tiempo de empaquetado
            silo--;
            bodega++;
            cout << "Empacadora produjo: 1 bolsa de 1 lb de cafe\n";
            cout << "Lbs de cafe en silo: " << silo << "\n";
            cout << "Lbs de cafe en bodega: " << bodega << "\n";
        }
    }
}

int main() {
    thread t1(tostadora1);
    thread t2(tostadora2);
    thread t3(empacadora);

    t1.join();
    t2.join();
    t3.join();

    cout << "Produccion completada. Lbs de cafe en bodega: " << bodega << "\n";
    return 0;
}
