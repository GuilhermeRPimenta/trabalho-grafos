#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H
#include "grafo.h"
#include "lista_encad.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

class GrafoLista : public Grafo {
private:
    ListaEncadeada* vertices; 
    int ordem;
    bool direcionado;
    bool vertices_ponderados;
    bool arestas_ponderadas;

    void inicializar_vertices(int tam);

    void criar_grafo_completo();
    void criar_grafo_bipartido();
    void criar_aresta_ponte();
    void criar_vertice_articulado();
    void criar_arvore();
    void criar_arestas_restantes(int grau, int componentesConexas);

public:
    GrafoLista();
    GrafoLista(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas);
    ~GrafoLista();

    void carrega_grafo(const std::string &arquivo);
    void salva_grafo(std::ofstream &saida) const;
    void novo_grafo(const std::string &descricao);

    int n_conexo() const;
    void dfs(int vertice, bool* visitado) const;
    void dfs_ordem(int vertice, bool* visitado, int* pilha, int& topo) const;
    GrafoLista transpor() const;

    bool bfs_bipartido(int inicio, int* cor) const;

    int get_grau() const;
    int get_ordem() const;
    bool eh_direcionado() const;
    bool vertice_ponderado() const;
    bool aresta_ponderada() const;
    bool eh_completo() const;
    bool eh_bipartido() const;
    bool eh_arvore() const;
    bool possui_ponte() const;
    bool possui_articulacao() const;
    void dfs_ignorando_vertice(int atual, int ignorado, bool* visitado) const;


    void imprime_grafoLista() const;
};

#endif