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
        
        // Adiciona a aresta
        vertices[origem-1].insereFinal(destino-1, pesoAresta);

        // Adiciona a aresta inversa
        if (!direcionado) {
            vertices[destino-1].insereFinal(origem-1, pesoAresta);
        }
    }

    entrada.close();

    std::cout << "Grafo carregado com sucesso!" << std::endl;
    std::cout << "Ordem: " << ordem << " | Direcionado: " << (direcionado ? "Sim" : "Não") << std::endl;
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
                fila[fim++] = vizinho; // Adiciona o vizinho à fila
                visitados++; 
            }
        }
    }

    delete[] visitado;
    delete[] fila; 
    return visitados == ordem; // Verifica se todos os vértices foram visitados
}

void GrafoLista::dfs(int vertice, bool* visitado) const {
    visitado[vertice] = true; // Marca o vértice como visitado

    // Percorre todos os vizinhos do vértice
    No* atual = vertices[vertice].getPrimeiro();
    while (atual != nullptr) {
        int vizinho = atual->getInfo();
        if (!visitado[vizinho]) {
            dfs(vizinho, visitado);
        }
        atual = atual->getProx();
    }
}

int GrafoLista::get_numero_componentes_conexas() const {
    if (ordem == 0) {
        return 0; // Sem vértices, sem componentes
    }

    bool* visitado = new bool[ordem];
    for (int i = 0; i < ordem; ++i) {
        visitado[i] = false; 
    }

    int num_componentes = 0;

    for (int i = 0; i < ordem; ++i) {
        if (!visitado[i]) {
            ++num_componentes;
            dfs(i, visitado); // Explora o componente conexo
        }
    }

    delete[] visitado; // Libera a memória alocada
    return num_componentes;
}

bool GrafoLista::eh_bipartido() const {
   

}



bool GrafoLista::eh_arvore() const {
    return (get_grau() == ordem - 1 && !direcionado && eh_conexo());
}



