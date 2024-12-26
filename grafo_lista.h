#include <string>
#include "lista_encad.h"
#include "No.h"
#ifndef GRAFO_LISTA_H_INCLUDED
#define GRAFO_LISTA_H_INCLUDED

class grafo_lista
{
    private:
    int V;
    lista_encad* vetor;
public:
    grafo_lista(int V) : V(V) {
        vetor = new lista_encad[V];
        for (int i = 0; i < V; ++i)
            vetor[i].setPrimeiro(nullptr); //inicializa uma nova lista do grafo vazia
    }
    virtual ~grafo_lista() {
        delete[] vetor;
    }
    virtual bool eh_bipartido() const = 0;
    virtual int get_grau(int vertice) const = 0;
    virtual int get_ordem() const = 0;
};

#endif // GRAFO_LISTA_H_INCLUDED
