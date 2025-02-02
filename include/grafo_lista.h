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


    void setPesoV(float peso) override;
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

    /*
    bool bfs_bipartido(int inicio, int *cor) const;

    bool eh_bipartido() const override;
    bool eh_arvore() const override;
    bool possui_ponte() const override;
    bool possui_articulacao() const override;

    // METODO NOVO PARA SALVAR ALGORITMO SEM FORÇA BRUTA
    bool eh_bipartido_sem_bruta() const override;

    // AUXILIAR FORÇA BRUTA EH BIPARTIDO
    bool particao_valida(const bool conjunto1[], const bool conjunto2[]) const;

    void dfs_ignorando_vertice(int atual, int ignorado, bool *visitado) const;

    */

    void imprime_grafoLista() const;
};

#endif