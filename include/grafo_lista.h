#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H
#include "grafo.h"
#include "lista_encad.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>


struct NoLL {
    int indice;                // Índice do nó na lista
    ListaEncadeada lista;      // Lista encadeada de adjacências do nó
    NoLL* proximo;             // Ponteiro para o próximo nó

    NoLL(int i) : indice(i), proximo(nullptr) {}

    ~NoLL() {
        // Limpar a lista de adjacências
        No* p = lista.getPrimeiro();
        while (p != nullptr) {
            No* t = p->getProx();
            delete p;  // Libera o nó atual
            p = t;     // Avança para o próximo nó
        }
    }

    // Método para adicionar um vértice à lista de adjacências
    void adicionarAdjacente(int destino, float peso) {
        lista.insereFinal(destino, peso);
    }

    // Método para remover adjacente
    void removerAdjacente(int destino) {
        lista.remove(destino);
    }
};

struct LinkedList {
    NoLL* primeiro;  
    LinkedList() : primeiro(nullptr) {}

    ~LinkedList() {
        NoLL* p = primeiro;
        while (p != nullptr) {
            NoLL* t = p->proximo;
            delete p;  // Libera o nó atual
            p = t;     // Avança para o próximo nó
        }
    }

    // Método para adicionar um vértice (noLL)
    void adicionarVertice(int indice, float peso) {
        NoLL* novo = new NoLL(indice);
        novo->lista.setPesoV(peso);
        novo->proximo = primeiro;
        primeiro = novo;
    }

    NoLL* buscarIndice(int indiceBusca) {
        NoLL* atual = primeiro;
        while (atual != nullptr) {
            if (atual->indice == indiceBusca) {
                return atual;  
            }
            atual = atual->proximo;
        }
        return nullptr;  
    }

    // Método para remover um vértice pela sua lista de adjacências
    void removerVertice(int indiceRemover) {
        NoLL* atual = primeiro;
        NoLL* anterior = nullptr;

        while (atual != nullptr) {
            if (atual->indice == indiceRemover) {
                if (anterior == nullptr) {
                    // Está removendo o primeiro nó
                    primeiro = atual->proximo;
                } else {
                    // Está removendo um nó intermediário
                    anterior->proximo = atual->proximo;
                }
                delete atual;  // Libera o nó encontrado
                return;
            }
            anterior = atual;
            atual = atual->proximo;
        }
    }
};


class GrafoLista : public Grafo
{
private:
    LinkedList *vertices;

    void inicializar_vertices(int tam) override;

    void aux_dfs_ordem(NoLL* atual,int vertice, bool *visitado, int *pilha, int &topo);
    void aux_dfs(NoLL* atual, int vertice, bool *visitado);


    void setPesoV(float peso, int vertice) override;
    void setAresta(int origem, float pesoAresta, int destino) override;
    int getGrauV(int indice) override;

public:
    GrafoLista();
    GrafoLista(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas);
    ~GrafoLista();

    void salva_grafo(std::ofstream &saida) const;
    
    //void novo_grafo(const std::string &descricao);

    void dfs(int vertice, bool *visitado) override;
    void dfs_ordem(int vertice, bool *visitado, int *pilha, int &topo) override;
    int  conta_transposto(bool *visitado, int *pilha, int &topo) override;

    void nova_aresta(int origem, float peso, int destino);
    void deleta_aresta(int origem, int destino);

    void novo_no(int indice, float peso);
    void deleta_no(int indice);

    void imprimir();
};

#endif