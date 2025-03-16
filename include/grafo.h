#ifndef GRAFO_H
#define GRAFO_H
#include <string>

class Grafo {
public:
    virtual ~Grafo() {}
    virtual int get_grau();
    virtual int get_ordem();
    virtual bool eh_direcionado();
    virtual bool vertice_ponderado();
    virtual bool aresta_ponderada();
    virtual bool eh_completo();
    virtual int n_conexo();
    
    virtual void carrega_grafo(const std::string& arquivo);
    virtual void carrega_grafo_clusters(const std::string& arquivo);

    /*
    virtual bool eh_bipartido() const = 0;
    virtual bool eh_arvore() const = 0;
    virtual bool possui_articulacao() const = 0;
    virtual bool possui_ponte() const = 0;
    virtual bool eh_bipartido_sem_bruta() const = 0;
    */

    virtual void inicializar_vertices(int ordem) = 0;
    void inicializar_clusters(int nClusters, int ordem);
    

    virtual void setPesoV(float peso, int vertice) = 0;
    virtual void setAresta(int origem, float pesoAresta, int destino) = 0;

    virtual int  getGrauV(int indice) = 0;
    virtual float  get_Pesoaresta(int origem, int destino) = 0;

    virtual void nova_aresta(int origem, float peso, int destino) = 0;
    virtual void deleta_aresta(int origem, int destino) = 0;

    virtual void novo_no(int indice, float peso) = 0;
    virtual void deleta_no(int indice) = 0;


    float menor_caminho(int origem, int destino, int ordem);
    float maior_menor_caminho(Grafo &grafo, int ordem);

    int getVerticeVizinhoRand(int origem);
    void AGMG_randomizada(Grafo &grafo, int ordem);
    void AGMG_guloso(Grafo &grafo, int ordem);
    bool aux_AGMG_guloso(Grafo &grafo, int atual, int ordem, bool* nosVisitados);
    bool todosClustersVisitados(Grafo &grafo);

    int find_cluster(int node);
    bool explore_cluster(int start_node, bool *visited, int target_cluster, int origem);
    void agmg_reativo(int origem);
    virtual bool  existe_Aresta(int node, int i, int origem) = 0;


protected:
    int ordem;
    bool direcionado;
    bool vertices_ponderados;
    bool arestas_ponderadas;
    int nClusters;
    int **clusters;
    bool *clusters_visitados;

    int* relacao_id_cluster;

    virtual void dfs_ordem(int vertice, bool *visitado, int *pilha, int &topo) = 0;
    virtual void dfs(int vertice, bool *visitado) = 0;

    virtual int conta_transposto(bool *visitado, int *pilha, int &topo) = 0;

    bool no_valido(int no);
    int get_vizinhos(int no, int* vizinhos);

};


#endif // GRAFO_H