#include <string>
#ifndef GRAFO_LISTA_H_INCLUDED
#define GRAFO_LISTA_H_INCLUDED

class grafo_lista
{
public:
    grafo_lista() {}
    virtual ~grafo_lista() {}
    virtual bool eh_bipartido() const = 0;
    virtual int get_grau(int vertice) const = 0;
    virtual int get_ordem() const = 0;
    virtual bool eh_direcionado() const = 0;
    virtual bool vertice_ponderado() const = 0;
    virtual bool aresta_ponderada() const = 0;
    virtual bool eh_completo() const = 0;
    virtual bool eh_arvore() const = 0;
    virtual bool possui_articulacao() const = 0;
    virtual bool possui_ponte() const = 0;
    virtual void carrega_grafo(const std::string& arquivo) = 0;
};


#endif // GRAFO_LISTA_H_INCLUDED
