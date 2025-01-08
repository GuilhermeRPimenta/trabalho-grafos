#ifndef GRAFO_MATRIZ_H_INCLUDED
#define GRAFO_MATRIZ_H_INCLUDED
#include "grafo.h"
#include "iostream"
#include "fstream"
#include "string"

using namespace std;

class GrafoMatriz: public Grafo {
    public:
        int **matriz;
        int *matriz_sem_direcao;
        int *vertices;
        int n_vertices;
        bool grafo_direcionado;
        bool arestas_ponderado;
        bool vertices_ponderado;

        GrafoMatriz();
        ~GrafoMatriz();
        void carrega_grafo(const std::string &arquivo);
        void imprimir_grafo() const;
        void imprimir_grafo_formato_txt(int vertice);

        bool eh_bipartido() const;
        int get_grau_vertice(int vertice) const;
        int get_grau() const;
        int get_ordem() const;
        int n_conexo() const;
        bool eh_direcionado() const;
        bool vertice_ponderado() const;
        bool aresta_ponderada() const;
        bool eh_completo() const;
        bool eh_arvore() const;
        bool possui_articulacao() const;
        bool possui_ponte() const;
        void novo_grafo(const std::string &arquivo);
        void salva_grafo(std::ofstream &saida) const;
        GrafoMatriz* get_copia() const;
    private:
        int get_aresta(int i, int j) const;
        void set_aresta(int i, int j, int valor);
        void inicia_grafo(int n_vertices, bool direcionado);
        int get_numero_vertices_conexos(int vertice) const;
        int aux_get_numero_vertices_conexos(int vertice, bool* vet_vertices_visitados) const;
        void criar_arvore(int i, int tam);
};

#endif