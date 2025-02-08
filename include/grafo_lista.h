#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H
#include "grafo.h"
#include "lista_encad.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

class GrafoLista : public Grafo
{
private:
    ListaEncadeada *vertices;

    void inicializar_vertices(int tam) override;


    void setPesoV(float peso, int vertice) override;
    void setAresta(int origem, float pesoAresta, int destino) override;
    int getGrauV(int indice) override;

public:
    GrafoLista();
    GrafoLista(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas);
    ~GrafoLista();

    void salva_grafo(std::ofstream &saida) const;
    void novo_grafo(const std::string &descricao);

    void dfs(int vertice, bool *visitado) override;
    void dfs_ordem(int vertice, bool *visitado, int *pilha, int &topo) override;
    int  conta_transposto(bool *visitado, int *pilha, int &topo) override;

    void nova_aresta(int origem, float pesoAresta, int destino);

    void deleta_aresta(int origem, int destino);

};

#endif