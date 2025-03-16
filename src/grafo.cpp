#include "../include/grafo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>

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

float Grafo::menor_caminho(int origem, int destino, int ordem)
{
    int dist[ordem + 1];        // Armazena as menores distâncias

    // Inicializa as distâncias como infinito e não processado
    for (int i = 1; i <= ordem; i++)
    {
        dist[i] = 1000000000; // Usando um valor grande como "infinito"
    }

    // A distância para a origem é 0
    dist[origem] = 0;

    // Executa o algoritmo de Bellman-Ford para relaxar as arestas
    for (int i = 1; i < ordem; i++)
    { // Relaxamento de todas as arestas (ordem-1 iterações)
        for (int u = 1; u <= ordem; u++)
        {
            for (int v = 1; v <= ordem; v++)
            {
                int peso = get_Pesoaresta(u, v); // Obtem o peso da aresta entre u e v

                // Debug: Mostrar o peso da aresta
                if (peso != 1000000000)
                {
                    //                    std::cout << "Aresta: " << u << " -> " << v << " com peso: " << peso << std::endl;
                }

                if (peso != 1000000000)
                { // Se existe aresta entre u e v
                    if (dist[u] + peso < dist[v])
                    { // Relaxamento da aresta
                        dist[v] = dist[u] + peso;
                    }
                }
            }
        }
    }

    // Verificação de ciclos negativos após o relaxamento
    for (int u = 1; u <= ordem; u++)
    {
        for (int v = 1; v <= ordem; v++)
        {
            int peso = get_Pesoaresta(u, v);
            if (peso != 1000000000)
            {
                if (dist[u] + peso < dist[v])
                { // Se for possível melhorar a distância
                    std::cout << "Erro: Ciclo negativo detectado!" << std::endl;
                    return -1; // Retorna -1 se houver um ciclo negativo
                }
            }
        }
    }

    // Imprime a mensagem antes de retornar o valor
    if (dist[destino] == 1000000000)
    {
        //       std::cout << "MENOR CAMINHO entre " << origem << " e " << destino << ": -1 (sem caminho)";
        return -1; // Se não houver caminho, retorna -1
    }
    else
    {
        //       std::cout << "MENOR CAMINHO entre " << origem << " e " << destino << ": ";
        return (dist[destino] > 900000000) ? std::cout << "Nao existe caminho" << std::endl, -1 : dist[destino];
    }
}

float Grafo::maior_menor_caminho(Grafo &grafo, int ordem)
{
    float maior_dist = -1;

    for (int origem = 1; origem <= ordem; origem++)
    {
        for (int destino = 1; destino <= ordem; destino++)
        {
            if (origem != destino)
            {
                float caminho = grafo.menor_caminho(origem, destino, ordem);
                if (caminho > maior_dist && caminho != -1)
                {
                    maior_dist = caminho;
                }
            }
        }
    }

    // std::cout << maior_dist << std::endl;
    return maior_dist;
}

/*
Alterações para testes usando com-Amazon...
*/

void Grafo::carrega_grafo_clusters(const std::string &arquivo){
    std::string caminho_completo = "./entradas/" + arquivo;
    std::ifstream entrada(caminho_completo);

    if (!entrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << arquivo << std::endl;
        return;
    }
    int num_arestas;
    entrada >> ordem >> num_arestas;  // Leitura da ordem e número de arestas

    nClusters = 400;

    inicializar_vertices(ordem);
    inicializar_clusters(nClusters, ordem);

    clusters_visitados = new bool[nClusters](); 

    int indexNo, destino, indexCluster;
    while (entrada >> indexNo >> destino >> indexCluster) {
        clusters[indexNo][indexCluster] = 1;
        setAresta(indexNo, 1, destino);
    }


    direcionado = true;
    arestas_ponderadas = false;
    vertices_ponderados = false;

    clusters_visitados = new bool[nClusters+1];
    for (int i = 0; i < nClusters+1; i++){
        clusters_visitados[i] = false;
    }

    inicializar_vertices(ordem);

    relacao_id_cluster = new int[ordem+1];

    for (int i = 0; i < num_arestas; i++){
        int origem, destino, cluster;
        entrada >> origem >> destino >> cluster;
        setAresta(origem, 1, destino);
        setAresta(destino, 1, origem);
        relacao_id_cluster[origem] = cluster;
    }

    entrada.close();

    std::cout << "Grafo carregado com sucesso!" << std::endl;
    std::cout << "Ordem: " << ordem << " | Direcionado: " << (direcionado ? "Sim" : "Não") << std::endl
              << std::endl;

    /*
    inicializar_clusters(nClusters, ordem);

    clusters_visitados = new bool[nClusters](); 

    int indexNo, destino, indexCluster;
    while (entrada >> indexNo >> destino >> indexCluster) {
        clusters[indexNo][indexCluster] = 1;
        setAresta(indexNo, 1, destino);
    }
        */
    
}

void Grafo::inicializar_clusters(int nClusters, int ordem)
{
    clusters = new int*[ordem];
    for (int i = 0; i < ordem; i++) {
        clusters[i] = new int[nClusters]();  // Inicializa todos os valores como 0
    }
}

int Grafo::find_cluster(int node)
{
    // Ajuste o índice para acessar corretamente os clusters (como os índices começam em 1)
    for (int i = 0; i < nClusters; i++) { 
        if (clusters[node - 1][i] == 1) {  // Ajuste para node - 1
            return i; // Encontrou o cluster
        }
    }
    return -1; // Caso não encontre
}

bool Grafo::explore_cluster(int start_node, int &novo_cluster, bool *clusters_visitados)
{
    visited[start_node - 1] = true;  // Ajuste para start_node - 1

    // Tenta explorar os vizinhos do nó
    for (int i = 0; i < ordem; i++) {
        if (!visited[i] && existe_Aresta(start_node - 1, i)) {  // Ajuste para start_node - 1
            novo_cluster = find_cluster(i + 1);  // Passa i + 1 porque o índice começa em 1

            if (!clusters_visitados[novo_cluster]) {
                clusters_visitados[novo_cluster] = true; 
                return true;  
            }
        }
    }

    return false;
}
/*
void Grafo::inicializar_clusters(int nClusters, int ordem){
    clusters = new int *[ordem];
    for (int i = 0; i < ordem; i++)
    {
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
        if (get_Pesoaresta(no, i) == 1) {           
            vizinhos[index] = i;
            index++;
        }
    }

    // std::cout << "Vizinhos: " << std::endl;
    // for (int i = 0; i<index; i++) {
    //     std::cout << vizinhos[i] << " ";
    // }
    // std::cout << std::endl;

    return index;
}

int Grafo::getVerticeVizinhoRand(int origem) {
    int vizinhos[ordem];
    int nVizinhos = get_vizinhos(origem, vizinhos);

    int vizinhos_cluster_naovisitados[ordem];
    int index = 0;

    for (int i = 0; i < nVizinhos; i++) {
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
    int* vertices = new int[ordem];
    int verticeNum = 0;
    
    std::cout << "No rand: " << noRand << std::endl;
    std::cout << "Cluster: " << grafo.relacao_id_cluster[noRand] << std::endl;

    inicializar_vertices(0);
    std::cout << "Inicializado " << std::endl;

    novo_no(noRand, 0);
    vertices[verticeNum] = noRand;
    verticeNum++;

    std::cout << "Novo no: " << noRand << std::endl;
    grafo.clusters_visitados[grafo.relacao_id_cluster[noRand]] = true;

    std::cout << "Definido cluster visitado" << std::endl;

    bool todosVisitados = false;

    while (!todosVisitados)
    {
        for (int i = verticeNum-1; i >= 0; i--) {
            int vizinho = grafo.getVerticeVizinhoRand(vertices[i]);
            // std::cout << "Vizinho rand encontrado: " << vizinho << std::endl;
            if (vizinho == -1 && i == 0) {
                //Não encontrado nenhum vizinho aleatório possível de outro cluster não visitado:
                for (int j = verticeNum-1; j >= 0; j--) {
                    int viz[ordem];
                    int nViz = grafo.get_vizinhos(vertices[j], viz);
                    int vizNaoVisitado[nViz];
                    int nVizNaoVisitado = 0;
                    // encontramos todos os vizinhos de J não visitados ainda e escolhemos um
                    for (int k = 0; k<nViz; k++) {
                        bool visitado = false;
                        for (int l = 0; l < verticeNum; l++) {
                            // std::cout << vertices[l] << std::endl;
                            if (viz[k] == vertices[l]) {
                                visitado = true;
                            }
                        }
                        if (!visitado) {
                            vizNaoVisitado[nVizNaoVisitado] = viz[k];
                            nVizNaoVisitado++;
                        }
                    }

                    if (nVizNaoVisitado > 0) {
                        int vizRand;
                        if (nVizNaoVisitado == 1)
                            vizRand = 0;
                        else vizRand = rand() % nVizNaoVisitado;
                        novo_no(vizNaoVisitado[vizRand], 0);
                        std::cout << "Novo no: " << vizNaoVisitado[vizRand] << std::endl;
                        setAresta(vertices[j], 1, vizNaoVisitado[vizRand]);
                        setAresta(vizNaoVisitado[vizRand], 1, vertices[j]);

                        std::cout << "Set aresta: " << "[" << vertices[j]  << "] " << vizNaoVisitado[vizRand] << std::endl;

                        vertices[verticeNum] = vizNaoVisitado[vizRand];
                        verticeNum++;
                        break;
                    }
                }
            } else if (vizinho == -1) {
                continue;
            }
            else {
                novo_no(vizinho, 0);
                std::cout << "Novo no: " << vizinho << std::endl;
                setAresta(vertices[i], 1, vizinho);
                setAresta(vizinho, 1, vertices[i]);

                std::cout << "Set aresta: " << "[" << vertices[i]  << "] " << vizinho << std::endl;

                grafo.clusters_visitados[grafo.relacao_id_cluster[vizinho]] = true;
                vertices[verticeNum] = vizinho;
                verticeNum++;


                for (int i = 1; i < grafo.nClusters + 1; i++) {
                    if (grafo.clusters_visitados[i]) {
                        todosVisitados = true;
                    }
                    else {
                        todosVisitados = false;
                        break;
                    }    
                }

                break;
            }
        }
    }

    std::cout << "clusters visitados : " << std::endl;
            
    for (int i = 0; i< nClusters+1; i++) {
        std::cout << grafo.clusters_visitados[i] << std::endl;
    }


    
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


int Grafo::find_cluster(int node)
{
    for (int i = 0; i < nClusters; i++) { 
        if (clusters[node][i] == 1) {
            return i; // Encontrou o cluster
        }
    }
    return -1; // Caso não encontre
}

bool Grafo::explore_cluster(int start_node, int &novo_cluster, bool *clusters_visitados)
{
    visited[start_node] = true;

    // Tenta explorar os vizinhos do nó
    for (int i = 0; i < ordem; i++) {
        if (!visited[i] && existe_Aresta(start_node, i)) {
            novo_cluster = find_cluster(i);  // Encontra o cluster do vizinho

            if (!clusters_visitados[novo_cluster]) {
                clusters_visitados[novo_cluster] = true; 
                return true;  
            }
        }
    }

    return false;
}

void Grafo::agmg_reativo()
{
    srand(time(0));

    visited = new bool[ordem]();
    bool* clusters_visitados = new bool[nClusters]();

    int total_clusters_explored = 0;
    int novo_cluster;

    int *expansao = new int[ordem]; // Lista para simular uma fila
    int frente = 0, tras = 0;

    // Começa de um nó aleatório
    int start_node = rand() % ordem + 1;  // Ajuste para garantir que o nó começa em 1
    expansao[tras++] = start_node;
    visited[start_node - 1] = true;  // Ajuste para start_node - 1
    clusters_visitados[find_cluster(start_node) - 1] = true; // Ajuste para start_node - 1
    total_clusters_explored++;

    while (total_clusters_explored < nClusters && frente < tras) {
        int node_atual = expansao[frente++];

        for (int i = 0; i < ordem; i++) {
            if (!visited[i] && existe_Aresta(node_atual - 1, i)) {  // Ajuste para node_atual - 1
                novo_cluster = find_cluster(i + 1);  // Passa i + 1 porque o índice começa em 1

                if (!clusters_visitados[novo_cluster]) {
                    clusters_visitados[novo_cluster] = true; 
                    total_clusters_explored++;
                    std::cout << "Cluster " << novo_cluster << " visitado!" << std::endl;
                }

                // Marca o nó e o adiciona à expansão
                visited[i] = true;
                expansao[tras++] = i + 1; // Armazena no expansao com índice ajustado
            }
        }
    }

    if (total_clusters_explored == nClusters) {
        std::cout << "Todos os clusters foram explorados!" << std::endl;
    } else {
        std::cout << "Erro: Apenas " << total_clusters_explored << " clusters foram explorados." << std::endl;
    }

    delete[] visited;
    delete[] clusters_visitados;
    delete[] expansao;
}







