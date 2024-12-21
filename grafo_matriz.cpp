#include "grafo_matriz.h"

GrafoMatriz::GrafoMatriz() {
    /*
    Apenas para motivos de teste:
    */
    string arquivo = "grafo.txt";
}

GrafoMatriz::~GrafoMatriz() {
    for (int i = 0; i < n_vertices; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
    delete[] matriz_sem_direcao;
    delete[] vertices;
}

void GrafoMatriz::carrega_grafo(std::string *arquivo) {
    ifstream arquivo_grafo(*arquivo);
    if (arquivo_grafo.is_open()) {
        arquivo_grafo >> n_vertices >> grafo_direcionado >> vertices_ponderado >> arestas_ponderado;
        novo_grafo(n_vertices, false);
        if (vertices_ponderado) {
            for (int i = 0; i < n_vertices; i++) {
                arquivo_grafo >> vertices[i];
            }
        }
        int v1, v2, peso;
        while (arquivo_grafo >> v1 && arquivo_grafo >> v2) {
            if (arestas_ponderado) {
                arquivo_grafo >> peso;
            }
            // Process the read values (v1, v2, peso)
        }
        cout << endl;
    }
}

bool GrafoMatriz::eh_bipartido() {
    return false;
}

int GrafoMatriz::get_grau(int vertice) {
    return 0;
}

int GrafoMatriz::get_ordem() {
    return 0;
}

bool GrafoMatriz::eh_direcionado() {
    return false;
}

bool GrafoMatriz::vertice_ponderado() {
    return false;
}

bool GrafoMatriz::aresta_ponderada() {
    return false;
}