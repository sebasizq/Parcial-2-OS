#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <climits>
#include <unistd.h>

using namespace std;


struct Bloque {
    string proceso; // "Libre" o nombre del proceso
    int tamano;     // tamaño del bloque físico
    int solicitado; 
    Bloque(string p, int t, int s) : proceso(p), tamano(t), solicitado(s) {}
};

class Memoria {
    vector<Bloque> bloques;
    int total;
public:
    Memoria(int sizeMem, int capacidad){
        for (int i = 0; i < sizeMem; i++) {
            bloques.push_back(Bloque("Libre", capacidad, 0));
        }
    }

    // Muestra el estado de la memoria en formato requerido
    void mostrar() const {
        for(int i = 0; i < bloques.size(); ++i) {
            const auto &b = bloques[i];
            if (b.proceso == "Libre") cout << "[Libre: " << b.tamano << "]";
            else cout << "[" << b.proceso << ": " << b.solicitado << "]";
        }
    }

    // Calcula y muestra fragmentación externa e interna
    void fragmentacion(){
        int FI = 0, FE = 0;
        for(int i = 0; i < bloques.size(); ++i) {
            const auto &b = bloques[i];
            if (b.proceso == "Libre") FE += b.tamano;
            else FI += (b.tamano - b.solicitado);
        }
        cout << "Fragmentacion externa total: " << FE << '\n';
        cout << "Fragmentacion interna total: " << FI << '\n';
    }
    // Verifica si ya existe el PID (incluso fragmentado)
    bool existeProceso(string &pid){
        bool ans = false;
        for(int i = 0; i < bloques.size(); ++i) {
            const auto &b = bloques[i];
            if (b.proceso == pid) ans = true;
        }
        return ans;
    }

    // Asignación con First/Best/Worst y fallback multi-bloque
    void asignar(string &pid, int tamanoSolicitado, string &algoritmo) {
        if (tamanoSolicitado <= 0){ cout << "Tamano invalido para " << pid << '\n'; return; }
        if (existeProceso(pid)) { cout << "El proceso " << pid << " ya existe." << '\n'; return; }
        // Buscar bloque adecuado según algoritmo
        int idx = -1;
        if (algoritmo == "FIRST") {
            for (int i = 0; i < (int)bloques.size(); ++i) {
                if (bloques[i].proceso == "Libre" && bloques[i].tamano >= tamanoSolicitado) {
                    idx = i;
                    break;
                }
            }
        } else if (algoritmo == "BEST") {
            int bestSize = INT_MAX;
            for (int i = 0; i < (int)bloques.size(); ++i) {
                if (bloques[i].proceso == "Libre" && bloques[i].tamano >= tamanoSolicitado && bloques[i].tamano < bestSize) {
                    bestSize = bloques[i].tamano;
                    idx = i;
                }
            }
        } else if (algoritmo == "WORST") {
            int worstSize = -1;
            for (int i = 0; i < (int)bloques.size(); ++i) {
                if (bloques[i].proceso == "Libre" && bloques[i].tamano >= tamanoSolicitado && bloques[i].tamano > worstSize) {
                    worstSize = bloques[i].tamano;
                    idx = i;
                }
            }
        } else {
            cout << "Algoritmo de asignacion desconocido: " << algoritmo << '\n';
            return;
        }
        if (idx != -1) {
            // Asignar en el bloque encontrado: conservar el tamano fisico del bloque
            if (bloques[idx].tamano >= tamanoSolicitado) {
                int tamF = bloques[idx].tamano;
                bloques[idx] = Bloque(pid, tamF, tamanoSolicitado);
            } else {
                cout << "No asignado: " << pid << '\n';
            }
        }
    
    }   

    // Liberar por PID (libera PID)
    void liberar(string &pid){
        bool encontrado = false;
        for (auto &b : bloques) {
            if (b.proceso == pid ) {
                b.proceso = "Libre";
                b.solicitado = 0;
                encontrado = true;
            }
        }
        if (!encontrado) { cout << "El proceso " << pid << " no existe." << '\n'; return; }   
        }
};

static string toUpper_s(string s) { transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return toupper(c); }); return s; }

int main() {
    string algoritmo;
    int tamBloques, sizeMem;
    cout << "Ingrese algoritmo de asignacion (First, Best, Worst): " << endl;
    cin >> algoritmo;
    cout << "Ingrese la capacidad de cada bloque: " << endl;
    cin >> tamBloques;
    cout << "Ingrese tamano total de memoria: " << endl;
    cin >> sizeMem;
    algoritmo = toUpper_s(algoritmo);
    Memoria memoria(sizeMem, tamBloques);
    cin.ignore();
    string line;
    bool running = true;
    while (running) {
        string line;
        getline(cin, line);
        istringstream iss(line);
        string command;
        iss >> command;
        if (command == "A") {
            string pid;
            int tamano;
            iss >> pid >> tamano;
            memoria.asignar(pid, tamano, algoritmo);
        } else if (command == "L") {
            string pid;
            iss >> pid;
            memoria.liberar(pid);
        } else if (command == "M") {
            memoria.mostrar();
            cout << '\n';
            memoria.fragmentacion();
            cout << '\n';
        } else if (command == "E") {
            running = false;
        } else {
            cout << "Comando desconocido: " << command << '\n';
        }
    }
    return 0;
}
