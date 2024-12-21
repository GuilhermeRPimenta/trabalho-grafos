#include "grafo_matriz.h"

GrafoMatriz::GrafoMatriz() {
    /*
    Apenas para motivos de teste:
    */
    string arquivo = "grafo.txt";
    carrega_grafo(arquivo);
    imprimir_grafo();
}

GrafoMatriz::~GrafoMatriz() {
    for (int i = 0; i < n_vertices; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
    delete[] matriz_sem_direcao;
    delete[] vertices;
}

void GrafoMatriz::imprimir_grafo() {
    for (int i = 0; i < n_vertices; i++) {
        for (int j = 0; j < n_vertices; j++) {
            cout << get_aresta(i,j) << " ";
        }
        cout << endl;
    }
}

void GrafoMatriz::novo_grafo(int n, bool direcionado) {
    matriz = new int*[n];
    if (direcionado) {
        grafo_direcionado = true;
        for (int i = 0; i < n; i++) {
            matriz[i] = new int[n];
            for (int j = 0; j < n; j++) {
                matriz[i][j] = 0;
            }
        }
    } else {
        grafo_direcionado = false;
        for (int i = 0; i < n; i++) {
            matriz_sem_direcao = new int[(n * (n-1))/2];
        }
    }

    vertices = new int[n];
    n_vertices = n;
}

void GrafoMatriz::carrega_grafo(const std::string &arquivo) {
    ifstream arquivo_grafo(arquivo);
    if (arquivo_grafo.is_open()) {
        arquivo_grafo >> n_vertices >> grafo_direcionado >> vertices_ponderado >> arestas_ponderado;
        novo_grafo(n_vertices, grafo_direcionado);
        if (vertices_ponderado) {
            for (int i = 0; i < n_vertices; i++) {
                arquivo_grafo >> vertices[i];
            }
        }
        int v1, v2, peso;
        while (arquivo_grafo >> v1 && arquivo_grafo >> v2) {
            if (arestas_ponderado) {
                arquivo_grafo >> peso;
                set_aresta(v1-1, v2-1, peso);
            } else {
                set_aresta(v1-1, v2-1, 1);
            }
            
        }
    }
}

int GrafoMatriz::get_aresta(int i, int j) {
    if (grafo_direcionado) {
        return matriz[i][j];
    } else {
        if (i < j) {
            return matriz_sem_direcao[(i * (i-1))/2 + j];
        } else {
            return matriz_sem_direcao[(j * (j-1))/2 + i];
        }
    }
}

void GrafoMatriz::set_aresta(int i, int j, int val) {
    if (grafo_direcionado) {
        matriz[i][j] = val;
    } else {
        if (i < j) {
            matriz_sem_direcao[(i * (i-1))/2 + j] = val;
        } else {
            matriz_sem_direcao[(j * (j-1))/2 + i] = val;
        }
    }
}


bool GrafoMatriz::eh_bipartido() const {
    return false;
} 

int GrafoMatriz::get_grau(int vertice) const {
    return vertice;
}

int GrafoMatriz::get_ordem() const {
    return 0;
}

bool GrafoMatriz::eh_direcionado() const {
    return false;
}

bool GrafoMatriz::vertice_ponderado() const {
    return false;
}

bool GrafoMatriz::aresta_ponderada() const {
    return false;
}

bool GrafoMatriz::eh_completo() const {
    return false;
}

bool GrafoMatriz::eh_arvore() const {
    return false;
}

bool GrafoMatriz::possui_articulacao() const {
    return false;
}

bool GrafoMatriz::possui_ponte() const {
    return false;
}




/*
Para testes com o arquivo grafo.txt
*/

int main() {
    GrafoMatriz *g = new GrafoMatriz();
    return 0;
}