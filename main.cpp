#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

int main() {
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> consumo_dist(0.0, 2.0);

    uniform_real_distribution<> pressao_dist(1.0, 4.0);

    double total_litros = 0.0;

    cout << fixed << setprecision(2);
    cout << "simulador de hidrômetro iniciado...\n\n";

    while (true) {
        double consumo_inst = consumo_dist(gen);  // litros/s
        double pressao = pressao_dist(gen);       // bar

        total_litros += consumo_inst;

        int metros_cubicos = static_cast<int>(total_litros / 1000);
        int litros_restantes = static_cast<int>(total_litros) % 1000;

        // ponteiros auxiliares
        int centenas = (litros_restantes / 100) % 10;
        int dezenas  = (litros_restantes / 10) % 10;
        int litros   = litros_restantes % 10;
        int decimos  = static_cast<int>( (total_litros - static_cast<int>(total_litros)) * 10 );

        cout << "consumo instantâneo: " << consumo_inst << " L/s\n";
        cout << "total acumulado: " << metros_cubicos << " m³ "
             << litros_restantes << " L\n";
        cout << "ponteiros -> centenas: " << centenas
             << " | dezenas: " << dezenas
             << " | litros: " << litros
             << " | décimos: " << decimos << endl;
        cout << "pressão da água: " << pressao << " bar\n";
        cout << "-----------------------------\n";

        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}