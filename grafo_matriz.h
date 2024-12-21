#ifndef GRAFO_MATRIZ_H_INCLUDED
#define GRAFO_MATRIZ_H_INCLUDED
#include "grafo.h"
#include "iostream"

using namespace std;

class GrafoMatriz: public Grafo {
    public:
        int **matriz;
        int *vertices;
        GrafoMatriz() {
            /*
            Apenas para motivos de teste:
            */
           novo_grafo(5, true);
        }
        ~GrafoMatriz() {
            for (int i = 0; i < 5; i++) {
                delete[] matriz[i];
            }
            delete[] matriz;
            delete[] vertices;
        }

        
        void carrega_grafo() {

        }

        void imprimir_grafo() {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    cout << matriz[i][j] << " ";
                }
                cout << endl;
            }
        }
    private:
        void novo_grafo(int n_vertices, bool direcionado) {
            matriz = new int*[n_vertices];
            for (int i = 0; i < n_vertices; i++) {
                matriz[i] = new int[n_vertices];
                for (int j = 0; j < n_vertices; j++) {
                    matriz[i][j] = 0;
                }
            }

            vertices = new int[n_vertices];
        }
};

#endif