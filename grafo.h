#ifndef GRAFO_H
#define GRAFO_H
#include <string>

class Grafo {
public:
    virtual ~Grafo() {}
    virtual bool eh_bipartido() const = 0;
    virtual int get_grau() const = 0;
    virtual int get_ordem() const = 0;
    virtual bool eh_direcionado() const = 0;
    virtual bool vertice_ponderado() const = 0;
    virtual bool aresta_ponderada() const = 0;
    virtual bool eh_completo() const = 0;
    virtual bool eh_arvore() const = 0;
    virtual int n_conexo() const = 0;
    virtual bool possui_articulacao() const = 0;
    virtual bool possui_ponte() const = 0;
    virtual void carrega_grafo(const std::string& arquivo) = 0;
};


#endif // GRAFO_H