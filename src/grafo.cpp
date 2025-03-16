#include "../include/grafo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

void Grafo::carrega_grafo(const std::string &arquivo)
{
    std::string caminho_completo = "./entradas/" + arquivo;
    std::ifstream entrada(caminho_completo);

    if (!entrada.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << arquivo << std::endl;
        return;
    }

    // Lê ordem, direcionado, vertices ponderados e arestas ponderadas
    entrada >> ordem >> direcionado >> vertices_ponderados >> arestas_ponderadas;
    inicializar_vertices(ordem);

    // Lê os pesos dos vértices (se vértices forem ponderados
    if (vertices_ponderados)
    {
        for (int i = 0; i < ordem; ++i)
        {
            float peso;
            entrada >> peso;
            setPesoV(peso, i); // IDs começam em 1
        }
    }

    // Lê as arestas
    int origem, destino;
    float pesoAresta = 1.0; // Peso padrão
    while (entrada >> origem >> destino)
    {
        if (arestas_ponderadas)
        {
            entrada >> pesoAresta;
        }
        // Adiciona a aresta (-1 para ajustar com info)
        setAresta(origem, pesoAresta, destino);
    }

    entrada.close();

    std::cout << "Grafo carregado com sucesso!" << std::endl;
    std::cout << "Ordem: " << ordem << " | Direcionado: " << (direcionado ? "Sim" : "Não") << std::endl
              << std::endl;
}

int Grafo::get_grau()
{
    int max_grau = 0;
    for (int i = 0; i < ordem; i++)
    {
        int grau_saida = getGrauV(i);

        if (grau_saida > max_grau)
        {
            max_grau = grau_saida;
        }
    }
    return max_grau;
}

int Grafo::get_ordem()
{
    return ordem;
}

bool Grafo::eh_direcionado()
{
    return direcionado;
}

bool Grafo::vertice_ponderado()
{
    return vertices_ponderados;
}

bool Grafo::aresta_ponderada()
{
    return arestas_ponderadas;
}

bool Grafo::eh_completo()
{
    int max_arestas = ordem * (ordem - 1) / 2;
    if (direcionado)
    {
        max_arestas *= 2;
    }
    return get_grau() == max_arestas;
}

int Grafo::n_conexo()
{
    if (direcionado)
    {
        // Algoritmo de Kosaraju para grafos direcionados
        int *pilha = new int[ordem];
        bool *visitado = new bool[ordem];
        int topo = -1;

        // Preenche a pilha na ordem inversa
        for (int i = 0; i < ordem; i++)
            visitado[i] = false;

        for (int i = 0; i < ordem; i++)
        {
            if (!visitado[i])
                dfs_ordem(i, visitado, pilha, topo);
        }

        int numComp = conta_transposto(visitado, pilha, topo);

        delete[] pilha;
        delete[] visitado;

        return numComp;
    }

    else
    {
        // Algoritmo grafos não direcionados
        bool *visitado = new bool[ordem];
        for (int i = 0; i < ordem; ++i)
            visitado[i] = false;

        int numComp = 0;
        for (int i = 0; i < ordem; ++i)
        {
            if (!visitado[i])
            {
                dfs(i, visitado);
                ++numComp;
            }
        }

        delete[] visitado;
        return numComp;
    }
}

float Grafo::menor_caminho(int origem, int destino, int ordem) {
    int dist[ordem + 1];  // Armazena as menores distâncias
    bool processado[ordem + 1];  // Marca se um nó já foi processado

    // Inicializa as distâncias como infinito e não processado
    for (int i = 1; i <= ordem; i++) {
        dist[i] = 1000000000;  // Usando um valor grande como "infinito"
        processado[i] = false;
    }

    // A distância para a origem é 0
    dist[origem] = 0;

    // Executa o algoritmo de Bellman-Ford para relaxar as arestas
    for (int i = 1; i < ordem; i++) {  // Relaxamento de todas as arestas (ordem-1 iterações)
        for (int u = 1; u <= ordem; u++) {
            for (int v = 1; v <= ordem; v++) {
                int peso = get_Pesoaresta(u, v);  // Obtem o peso da aresta entre u e v

                // Debug: Mostrar o peso da aresta
                if (peso != 1000000000) {
//                    std::cout << "Aresta: " << u << " -> " << v << " com peso: " << peso << std::endl;
                }

                if (peso != 1000000000) {  // Se existe aresta entre u e v
                    if (dist[u] + peso < dist[v]) {  // Relaxamento da aresta
                        dist[v] = dist[u] + peso;
                    }
                }
            }
        }
    }

    // Verificação de ciclos negativos após o relaxamento
    for (int u = 1; u <= ordem; u++) {
        for (int v = 1; v <= ordem; v++) {
            int peso = get_Pesoaresta(u, v);
            if (peso != 1000000000) {
                if (dist[u] + peso < dist[v]) {  // Se for possível melhorar a distância
                    std::cout << "Erro: Ciclo negativo detectado!" << std::endl;
                    return -1;  // Retorna -1 se houver um ciclo negativo
                }
            }
        }
    }

    // Imprime a mensagem antes de retornar o valor
    if (dist[destino] == 1000000000) {
 //       std::cout << "MENOR CAMINHO entre " << origem << " e " << destino << ": -1 (sem caminho)";
        return -1;  // Se não houver caminho, retorna -1
    } else {
 //       std::cout << "MENOR CAMINHO entre " << origem << " e " << destino << ": ";
        return (dist[destino] > 900000000) ? std::cout << "Nao existe caminho" << std::endl, -1 : dist[destino];

    }
}

float Grafo::maior_menor_caminho(Grafo &grafo, int ordem) {
    float maior_dist = -1;

    for (int origem = 1; origem <= ordem; origem++) {
        for (int destino = 1; destino <= ordem; destino++) {
            std::cout << "Origem: " << origem << " | Destino: " << destino << std::endl;
            if (origem != destino) {
                float caminho = grafo.menor_caminho(origem, destino, ordem);
                if (caminho > maior_dist && caminho != -1) {
                    maior_dist = caminho;
                }
            }
        }
    }

    //std::cout << maior_dist << std::endl;
    return maior_dist;
}

/*
Alterações para testes usando com-Amazon...
*/

void Grafo::carrega_grafo_clusters(const std::string &arquivo){
    //std::string caminho_completo = "./entradas/" + arquivo;
    std::string caminho_completo = "./grafo-teste-cluster.txt";
    std::ifstream entrada(caminho_completo);

    if (!entrada.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << arquivo << std::endl;
        return;
    }

    int nArestas = 0;
    direcionado = true;
    arestas_ponderadas = false;
    vertices_ponderados = false;
    entrada >> ordem >> nArestas;
    // nClusters = 400;
    nClusters = 3;

    clusters_visitados = new bool[nClusters];
    for (int i = 0; i < nClusters; i++){
        clusters_visitados[i] = false;
    }

    inicializar_vertices(ordem);

    relacao_id_cluster = new int[ordem+1];

    for (int i = 0; i < nArestas; i++){
        int origem, destino, cluster;
        entrada >> origem >> destino >> cluster;
        setAresta(origem, 1, destino);
        relacao_id_cluster[origem] = cluster;
    }

    entrada.close();

    std::cout << "Grafo carregado com sucesso!" << std::endl;
    std::cout << "Ordem: " << ordem << " | Direcionado: " << (direcionado ? "Sim" : "Não") << std::endl
              << std::endl;

    /*
    inicializar_clusters(nClusters, ordem);

    clusters_visitados = new bool[nClusters];

    for(int i = 0; i < nClusters; i++){
        clusters_visitados[i] = false;
    }

    int indexNo;
    int indexCluster;
    while(entrada >> indexNo >> indexCluster){
        clusters[indexNo][indexCluster] = 1;
    }
        */
    
}

/*
void Grafo::inicializar_clusters(int nClusters, int ordem){
    clusters = new int *[ordem];
    for (int i = 0; i < ordem; i++){
        clusters[i] = new int[nClusters];
    }
}
*/

// bool Grafo::no_valido(int no) {
//     for (int i = 0; i < ordem; i++) {
//         if (i==no) {
//             continue;
//         }

//         if (get_Pesoaresta(no, i) == 1) {
//             return true;
//         }
//     }
//     return false;
// }

int Grafo::get_vizinhos(int no, int* vizinhos) {
    int index = 0;
    for (int i = 1; i <= ordem; i++) {
        std::cout<<"No: "<<no<<" | i: "<<i<<" | Peso: "<<get_Pesoaresta(no, i)<<std::endl;
        if (get_Pesoaresta(no, i) == 1) {           
            vizinhos[index] = i;
            index++;
        }
    }
    return index;
}

int Grafo::getVerticeVizinhoRand(int origem) {
    int vizinhos[ordem];
    int nVizinhos = get_vizinhos(origem, vizinhos);

    std::cout << "vizinhos: " ;
    for (int i = 0; i < nVizinhos; i++) {
        std::cout << "[" << i << "]: " << vizinhos[i] << " ";
    }
    std::cout << std::endl;

    int vizinhos_cluster_naovisitados[ordem];
    int index = 0;

    for (int i = 0; i < nVizinhos; i++) {
        std::cout << vizinhos[i] << " " << relacao_id_cluster[vizinhos[i]] << std::endl;
        if (!clusters_visitados[relacao_id_cluster[vizinhos[i]]]) {
            vizinhos_cluster_naovisitados[index] = vizinhos[i];
            index++;
        }
    }

    if (index == 0) {
        return -1;
    }

    // index = novo n de vizinhos
    int vizinhoRand = rand() % index;
    return vizinhos_cluster_naovisitados[vizinhoRand];
}


void Grafo::AGMG_randomizada(Grafo &grafo, int ordem){
    srand(time(0));
    int nClusters = grafo.nClusters;
    int noRand = 1 + rand() % (ordem-1);
    // std::cout << "No rand: " << noRand << std::endl;
    // std::cout << "Cluster: " << grafo.relacao_id_cluster[noRand] << std::endl;
    
    std::cout << "No rand: " << noRand << std::endl;
    std::cout << "Cluster: " << grafo.relacao_id_cluster[noRand] << std::endl;

    
    inicializar_vertices(0);
    std::cout << "Inicializado " << std::endl;
    novo_no(noRand, 0);
    std::cout << "Novo no: " << noRand << std::endl;

    grafo.clusters_visitados[grafo.relacao_id_cluster[noRand]] = true;

    std::cout << "Definido cluster visitado" << std::endl;
    int vizinho = grafo.getVerticeVizinhoRand(noRand);
    novo_no(vizinho, 0);
    setAresta(noRand, 1, vizinho);
    

    
    /*
    int noRand = rand() % ordem;
    
    for(int i = 0; i < nClusters; i++){
        if(clusters[noRand][i] == 1){
            clusters_visitados[i] = true;
            setAresta(noRand, 1, i);
        }
    }

    bool todosClustersVisitados = false;
    while(!todosClustersVisitados){
        for(int i = 0; i < nClusters; i++){
            if(!clusters_visitados[i]){
                todosClustersVisitados = false;
                break;
            }
            todosClustersVisitados = true;
        }

        getVerticeVizinhoRand()
    }*/
}


