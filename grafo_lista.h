#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

class GrafoLista {
private:
    ListaEncadeada* vertices; 
    int ordem;
    bool direcionado;
    bool vertices_ponderados;
    bool arestas_ponderadas;
    static const int MAXIMO = 1000; // Número máximo de nós no grafo
    static const int MAX_ARESTAS = 2000; // Número máximo de arestas no grafo (ajustar se necessário)

    int contador_dfs = 0; // Numeração da DFS
    int lista_adj[MAX_ARESTAS]; // Lista de adjacências
    int prox_adj[MAX_ARESTAS]; // Próximo elemento na lista de adjacências
    int inicio_adj[MAXIMO]; // Índice inicial de cada nó na lista de adjacências
    int numero_dfs[MAXIMO]; // Estrutura numero_dfs
    int menor_dfs[MAXIMO]; // Estrutura menor_dfs
    int visitado[MAXIMO]; // Marca o estado do vértice
    int pai[MAXIMO]; // Marca quem são os pais de cada nó na árvore de DFS
    bool existe_ponte = false; // Indica se existe ao menos uma ponte
    int prox_disponivel = 0; // Próxima posição disponível na lista de adjacências
    int arestas[MAXIMO * 2]; // Lista de todas as arestas
    int inicio[MAXIMO];     // Ponteiro para a primeira aresta de cada vértice
    int prox_aresta[MAXIMO * 2]; // Ponteiro para a próxima aresta
    bool ponto_articulacao[MAXIMO]; // Marca quais vértices são pontos de articulação

    enum { NULO = -1, BRANCO = 0, CINZA = 1, PRETO = 2 };

    void inicializar_vertices();

public:
    GrafoLista();
    GrafoLista(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas);
    ~GrafoLista();

    void carrega_grafo(const std::string &arquivo);
    void salva_grafo(std::ofstream &saida) const;
    void novo_grafo(const std::string &descricao);

    int numero_componentes_conexas() const;
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
    bool eh_conexo() const;
    bool eh_arvore() const;
    bool possui_ponte(int quantidade_nos);
    void busca_em_profundidade(int no);

    bool possui_articulacao(int n);
    bool dfs_ponto_articulacao(int inicial);

    
    

    void imprime_grafoLista() const;
};

#endif
