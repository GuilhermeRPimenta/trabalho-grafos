#include "grafo_lista.h"
#include "lista_encad.h"
#include <iostream>
#include <fstream>
#include <sstream>

GrafoLista::GrafoLista()
    : vertices(nullptr), ordem(0), direcionado(false), vertices_ponderados(false), arestas_ponderadas(false) {}

GrafoLista::GrafoLista(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas)
    : ordem(ordem), direcionado(direcionado), vertices_ponderados(vertices_ponderados), arestas_ponderadas(arestas_ponderadas) {
    inicializar_vertices();
}

GrafoLista::~GrafoLista() {
    if (vertices) {
        delete[] vertices;
    }
}

void GrafoLista::inicializar_vertices() {
    vertices = new ListaEncadeada[ordem]; // N numeros de LL (mesmo nr de nos)
}

void GrafoLista::carrega_grafo(const std::string &arquivo) {
    std::ifstream entrada(arquivo);
    if (!entrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << arquivo << std::endl;
        return;
    }

    // Lê ordem, direcionado, vertices ponderados e arestas ponderadas
    entrada >> ordem >> direcionado >> vertices_ponderados >> arestas_ponderadas;
    inicializar_vertices();

    // Lê os pesos dos vértices (se vértices forem ponderados) 
    if (vertices_ponderados) {
        for (int i = 0; i < ordem; ++i) {
            float peso;
            entrada >> peso;
            vertices[i].setPesoV(peso); // IDs começam em 1
        }
    }

    //Lê as arestas
    int origem, destino;
    float pesoAresta = 1.0; // Peso padrão
    while (entrada >> origem >> destino) {
        if (arestas_ponderadas) {
            entrada >> pesoAresta;
        }
        
        // Adiciona a aresta (-1 para ajustar com info)
        vertices[origem-1].insereFinal(destino-1, pesoAresta);

        // Adiciona a aresta inversa (-1 para ajustar com info)
        if (!direcionado) {
            vertices[destino-1].insereFinal(origem-1, pesoAresta);
        }
    }

    entrada.close();

    std::cout << "Grafo carregado com sucesso!" << std::endl;
    std::cout << "Ordem: " << ordem << " | Direcionado: " << (direcionado ? "Sim" : "Não") <<std::endl<< std::endl;
}


void GrafoLista::salva_grafo(std::ofstream &saida) const {
    
}

void GrafoLista::novo_grafo(const std::string &descricao) {
    std::istringstream stream(descricao);
    stream >> ordem >> direcionado >> vertices_ponderados >> arestas_ponderadas;
    inicializar_vertices();

    int origem, destino;
    float peso = 1.0;
    while (stream >> origem >> destino) {
        if (arestas_ponderadas) {
            stream >> peso;
        }
        vertices[origem].insereFinal(destino, peso);
        if (!direcionado) {
            vertices[destino].insereFinal(origem, peso);
        }
    }
}

int GrafoLista::get_grau() const {
    int grau = 0;
    for (int i = 0; i < ordem; ++i) {
        grau += vertices[i].tamanho();
    }
    return grau;
}

int GrafoLista::get_ordem() const {
    return ordem;
}

bool GrafoLista::eh_direcionado() const {
    return direcionado;
}

bool GrafoLista::vertice_ponderado() const {
    return vertices_ponderados;
}

bool GrafoLista::aresta_ponderada() const {
    return arestas_ponderadas;
}

bool GrafoLista::eh_completo() const {
    int max_arestas = ordem * (ordem - 1) / 2;
    if (direcionado) {
        max_arestas *= 2;
    }
    return get_grau() == max_arestas;
}

bool GrafoLista::eh_conexo() const {
    bool* visitado = new bool[ordem];
    for (int i = 0; i < ordem; ++i) {
        visitado[i] = false;
    }

    int* fila = new int[ordem];
    int inicio = 0, fim = 0;

    fila[fim++] = 0; 
    visitado[0] = true;
    int visitados = 1;

    while (inicio < fim) {
        int atual = fila[inicio++]; 
        auto* elementos = vertices[atual].obter_elementos(); 
        int num_elementos = vertices[atual].tamanho();

        
        for (int i = 0; i < num_elementos; ++i) { 
            int vizinho = elementos[i];
            if (!visitado[i]) { // Se o vizinho não foi visitado
                visitado[i] = true;
                fila[fim++] = vizinho; 
                visitados++; 
            }
        }
    }

    delete[] visitado;
    delete[] fila; 
    return visitados == ordem; // Verifica se todos os vértices foram visitados
}

void GrafoLista::dfs_ordem(int vertice, bool* visitado, int* pilha, int& topo) const {
    visitado[vertice] = true;
    No* noAtual = vertices[vertice].getPrimeiro();
    while (noAtual) {
        int adj = noAtual->getInfo();
        if (!visitado[adj]) {
            dfs_ordem(adj, visitado, pilha, topo);
        }
        noAtual = noAtual->getProx();
    }
    pilha[++topo] = vertice;
}

void GrafoLista::dfs(int vertice, bool* visitado) const {
    visitado[vertice] = true;
    No* noAtual = vertices[vertice].getPrimeiro();
    while (noAtual) {
        int adj = noAtual->getInfo();
        if (!visitado[adj]) {
            dfs(adj, visitado);
        }
        noAtual = noAtual->getProx();
    }
}


GrafoLista GrafoLista::transpor() const {
    GrafoLista transposto(ordem, true, vertices_ponderados, arestas_ponderadas);
    for (int i = 0; i < ordem; ++i) {
        No* noAtual = vertices[i].getPrimeiro();
        while (noAtual) {
            int adj = noAtual->getInfo();
            float peso = noAtual->getPeso();
            transposto.vertices[adj].insereFinal(i, peso);
            noAtual = noAtual->getProx();
        }
    }
    return transposto;
}


int GrafoLista::numero_componentes_conexas() const {
    if (eh_direcionado()) {
        // Algoritmo de Kosaraju para grafos direcionados
        int* pilha = new int[ordem];
        bool* visitado = new bool[ordem];
        int topo = -1;

        // Preenche a pilha na ordem inversa
        for (int i = 0; i < ordem; ++i)
            visitado[i] = false;

        for (int i = 0; i < ordem; ++i) {
            if (!visitado[i])
                dfs_ordem(i, visitado, pilha, topo);
        }

        GrafoLista grafoTransposto = transpor();

        for (int i = 0; i < ordem; ++i)
            visitado[i] = false;

        int numComponentes = 0;
        while (topo >= 0) {
            int v = pilha[topo--];
            if (!visitado[v]) {
                grafoTransposto.dfs(v, visitado);
                ++numComponentes;
            }
        }

        delete[] pilha;
        delete[] visitado;

        return numComponentes;
    } else {
        // Algoritmo grafos não direcionados
        bool* visitado = new bool[ordem];
        for (int i = 0; i < ordem; ++i)
            visitado[i] = false;

        int numComponentes = 0;
        for (int i = 0; i < ordem; ++i) {
            if (!visitado[i]) {
                dfs(i, visitado);
                ++numComponentes;
            }
        }

        delete[] visitado;
        return numComponentes;
    }
}

// Método auxiliar para BFS
bool GrafoLista::bfs_bipartido(int inicio, int* cor) const {
    int* fila = new int[ordem];
    int inicioFila = 0, fimFila = 0;

    // vértice inicial adicionado a uma fila e colore com 0
    fila[fimFila++] = inicio;
    cor[inicio] = 0;

    // processa os vértices da fila
    while (inicioFila < fimFila) {
        int atual = fila[inicioFila++];

        // percorre todos os adjacentes ao vértice atual
        No* noAtual = vertices[atual].getPrimeiro();
        while (noAtual) {
            int adj = noAtual->getInfo();
            if (cor[adj] == -1) {
                // adjacente ainda não foi visitado, colore com a cor oposta
                cor[adj] = 1 - cor[atual];
                fila[fimFila++] = adj;
            } else if (cor[adj] == cor[atual]) {
                // vértice adjacente tem a mesma cor
                delete[] fila;
                return false;
            }
            noAtual = noAtual->getProx();
        }
    }

    delete[] fila;
    return true;
}

bool GrafoLista::eh_bipartido() const {
    // -1: não visitado, 0: cor 0, 1: cor 1
    int* cor = new int[ordem];
    for (int i = 0; i < ordem; ++i) {
        cor[i] = -1; // todos os vértices estão sem cor
    }

    // percorre todos os vértices (para lidar com grafos desconexos)
    for (int i = 0; i < ordem; ++i) {
        if (cor[i] == -1) { // o vértice ainda não foi visitado
            if (!bfs_bipartido(i, cor)) {
                delete[] cor;
                return false; 
            }
        }
    }

    delete[] cor;
    return true; 
}




bool GrafoLista::eh_arvore() const {
    return (get_grau() == ordem - 1 && !direcionado && eh_conexo());
}



