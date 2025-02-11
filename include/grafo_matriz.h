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

        void novo_no(int indice, float peso) override;
        void deleta_no(int indice) override;
        void nova_aresta(int origem, float peso, int destino) override;
        void deleta_aresta(int origem, int destino) override;
        void imprime_vertices();

    private:
        int dim_matriz;

        void aumentar_matriz();
        float get_Pesoaresta(int i, int j) override;
        float get_aresta(int i, int j) const;
        void setAresta(int i,float valor, int j ) override;
        void inicializar_vertices(int ordem) override;
        void dfs(int vertice, bool* visitado) override;
        void dfs_ordem(int vertice, bool *visitado, int *pilha, int &topo) override;
        int conta_transposto(bool *visitado, int *pilha, int &topo) override;
};

#endif