#ifndef GRAFO_MATRIZ_H_INCLUDED
#define GRAFO_MATRIZ_H_INCLUDED
#include "grafo.h"
#include "iostream"
#include "fstream"
#include "string"

using namespace std;

class GrafoMatriz: public Grafo {
    public:
        int **matriz;
        int *matriz_sem_direcao;
        int *vertices;
        int n_vertices;
        bool grafo_direcionado;
        bool arestas_ponderado;
        bool vertices_ponderado;

        GrafoMatriz();
        ~GrafoMatriz();
        void carrega_grafo(std::string *arquivo);
        bool eh_bipartido();
        int get_grau(int vertice);
        int get_ordem();
        bool eh_direcionado();
        bool vertice_ponderado();
        bool aresta_ponderada();

        void imprimir_grafo();

        bool eh_bipartido() {
            return false;
        }
        int get_grau(int vertice) {
            return 0;
        }
        int get_ordem() {
            return 0;
        }
        bool eh_direcionado() {
            return false;
        }
        bool vertice_ponderado() {
            return false;
        }
        bool aresta_ponderada() {
            return false;
        }
        bool eh_completo() {
            return false;
        }
        bool eh_arvore() {
            return false;
        }
        bool possui_articulacao() {
            return false;
        }
        bool possui_ponte() {
            return false;
        }
    private:
        int get_aresta(int i, int j);
        void set_aresta(int i, int j, int valor);
        void novo_grafo(int n_vertices, bool direcionado);
};

#endif