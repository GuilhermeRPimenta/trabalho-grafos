#ifndef GRAFO_MATRIZ_H_INCLUDED
#define GRAFO_MATRIZ_H_INCLUDED
#include "grafo.h"
#include "iostream"
#include "fstream"
#include "string"

using namespace std;

class GrafoMatriz: public Grafo {
    public:
        float **matriz;
        float *matriz_sem_direcao;
        int *vertices;

        GrafoMatriz();
        GrafoMatriz(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas);
        ~GrafoMatriz();
        void setPesoV(float peso, int vertice) override;
        int getGrauV(int vertice) override;
        void salva_grafo(std::ofstream &saida) const;
        void novo_grafo(const std::string &arquivo);

        void novo_no(int peso = 0);
        void deleta_no(int no);
        //void imprime_vertices();

    private:
        int dim_matriz;

        int get_aresta(int i, int j) const;
        void setAresta(int i,float valor, int j ) override;
        void inicializar_vertices(int ordem) override;
        void dfs(int vertice, bool* visitado) override;
        void dfs_ordem(int vertice, bool *visitado, int *pilha, int &topo) override;
        int conta_transposto(bool *visitado, int *pilha, int &topo) override;
};

#endif