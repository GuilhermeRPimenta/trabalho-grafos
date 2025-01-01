#include "grafo_matriz.h"

GrafoMatriz::GrafoMatriz() {
    matriz = nullptr;
    matriz_sem_direcao = nullptr;
    vertices = nullptr;
    n_vertices = 0;
    grafo_direcionado = false;
    arestas_ponderado = false;
    vertices_ponderado = false;
}

GrafoMatriz::~GrafoMatriz() {
    for (int i = 0; i < n_vertices; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
    delete[] matriz_sem_direcao;
    delete[] vertices;
}

void GrafoMatriz::imprimir_grafo() {
    /*
    Imprime grafo em formato de matriz de adjacência
    não importa se direcionado ou não mostrará a matriz inteira
    */
    for (int i = 0; i < n_vertices; i++) {
        for (int j = 0; j < n_vertices; j++) {
            cout << get_aresta(i,j) << " ";
        }
        cout << endl;
    }
}

void GrafoMatriz::inicia_grafo(int n, bool direcionado) {
    matriz = new int*[n];
    if (direcionado) {
        grafo_direcionado = true;
        for (int i = 0; i < n; i++) {
            matriz[i] = new int[n];
            for (int j = 0; j < n; j++) {
                matriz[i][j] = 0;
            }
        }
    } else {
        grafo_direcionado = false;
        int tam = ((n * (n-1))/2);
        matriz_sem_direcao = new int[tam];
        for (int i = 0; i < tam; i++) {
            matriz_sem_direcao[i] = 0;
        }
    }

    vertices = new int[n];
    n_vertices = n;
}

void GrafoMatriz::carrega_grafo(const std::string &arquivo) {
    ifstream arquivo_grafo(arquivo);
    if (arquivo_grafo.is_open()) {
        // Lendo o cabeçalho do arquivo
        arquivo_grafo >> n_vertices >> grafo_direcionado >> vertices_ponderado >> arestas_ponderado;
        
        // Define dados básicos do grafo
        inicia_grafo(n_vertices, grafo_direcionado);

        // Lê peso dos vértices apenas se for ponderado
        if (vertices_ponderado) {
            for (int i = 0; i < n_vertices; i++) {
                arquivo_grafo >> vertices[i];
            }
        }

        // Finalmente coloca os dados na matriz
        int v1, v2, peso;
        while (arquivo_grafo >> v1 && arquivo_grafo >> v2) {
            if (arestas_ponderado) {
                arquivo_grafo >> peso;
                set_aresta(v1-1, v2-1, peso);
            } else {
                set_aresta(v1-1, v2-1, 1);
            }
            
        }
    }
}

int GrafoMatriz::get_aresta(int i, int j) const {
    // Suporte a grafos direcionados e não direcionados
    if (grafo_direcionado) {
        return matriz[i][j];
    } else {
        int n = n_vertices;
        if (i < j) {
            int index = (n * (n - 1)) / 2 - ((n - i) * (n - i - 1)) / 2 + (j - i - 1);
            return matriz_sem_direcao[index];
        } else if (i==j) {
            return 0;
        }
        else {
            int index = (n * (n - 1)) / 2 - ((n - j) * (n - j - 1)) / 2 + (i - j - 1);
            return matriz_sem_direcao[index];
        }
    }
}

void GrafoMatriz::set_aresta(int i, int j, int val) {
    //Supote a grafos direcionados e não direcionados
    if (grafo_direcionado) {
        matriz[i][j] = val;
    } else {
        int n = n_vertices;
        if (i < j) {
            int index = (n * (n - 1)) / 2 - ((n - i) * (n - i - 1)) / 2 + (j - i - 1);
            matriz_sem_direcao[index] = val;
        } else {
            int index = (n * (n - 1)) / 2 - ((n - j) * (n - j - 1)) / 2 + (i - j - 1);
            matriz_sem_direcao[index] = val;
        }
    }
}


bool GrafoMatriz::eh_bipartido() const {
    return false;
} 

int GrafoMatriz::get_grau(int vertice) const {
    //Assumindo que o vértice é 1-indexado
    int grau = 0;
    for (int i = 0; i < n_vertices; i++) {
        if (get_aresta(vertice-1, i) != 0)
        grau++;
    }
    return grau;
}

int GrafoMatriz::get_ordem() const {
    return n_vertices;
}

bool GrafoMatriz::eh_direcionado() const {
    return grafo_direcionado;
}

bool GrafoMatriz::vertice_ponderado() const {
    return vertices_ponderado;
}

bool GrafoMatriz::aresta_ponderada() const {
    return arestas_ponderado;
}

bool GrafoMatriz::eh_completo() const {
    if(grafo_direcionado){
        for(int i = 0; i<n_vertices; i++){
            for(int j = 0; j<n_vertices; j++){
                if(i == j){
                    continue;
                }
                if(matriz[i][j] == 0){
                    return false;
                }
            }
        }
    }
    else{
        for(int i = 0; i < n_vertices;i++){
            for(int j = i + 1; j < n_vertices; j++){
                if(get_aresta(i,j) == 0){
                    return false;
                }
            }
        }
    }
    return true;
}

bool GrafoMatriz::eh_arvore() const {
    int numero_vertices_conexos = get_numero_vertices_conexos(0);
    if(numero_vertices_conexos != n_vertices){
        return false;
    }

    int numero_arestas = 0;
    if(grafo_direcionado){
        for(int i = 0; i < n_vertices; i++){
            for(int j = 0; j < n_vertices; j++){
                if(get_aresta(i,j) !=0){
                    numero_arestas++;
                }
            }
        }
    }
    else{
        for(int i = 0; i< n_vertices; i++){
            for(int j = i +1; j < n_vertices; j++){
                if(get_aresta(i,j) != 0){
                    numero_arestas++;
                }
            }
        }
    }
    
    return numero_arestas == n_vertices - 1;
}

bool GrafoMatriz::possui_articulacao() const {
    /*Tomando em base a definição de articulação, ser um vértice que quando
    removido resulta em um numero maior de componentes conexos: */
    if(n_vertices < 2){
        return false;
    }

    int num_conexo = n_conexo();
    for (int i = 0; i < n_vertices; i++) {
        GrafoMatriz *g = get_copia();
        for (int j = 0; j < n_vertices; j++) {
            g->set_aresta(i,j,0);
            g->set_aresta(j,i,0);
        }
        //-1 porque o vertice i foi removido, e sempre seria contado como mais um componente conexo
        if (g->n_conexo()-1 > num_conexo) {
            return true;
        }
    }
    return false;
}

bool GrafoMatriz::possui_ponte() const {
    /*Tomando em base a definição de ponte, ser uma aresta que quando
    removida resulta em um numero maior de componentes conexos: */
    if(n_vertices < 2){
        return false;
    }
    int num_conexo = n_conexo();
    GrafoMatriz *g = get_copia();
    for (int i = 0; i < n_vertices; i++) {
        for (int j = 0; j < n_vertices; j++) {
            if (i==j) {
                continue;
            }
            if (g->get_aresta(i,j) != 0) {
                int aux = g->get_aresta(i,j);
                g->set_aresta(i,j,0);
                if (g->n_conexo() > num_conexo) {
                    g->set_aresta(i,j,aux);
                    return true;
                }
                g->set_aresta(i,j,aux);
            }
        }
    }
    return false;
}

GrafoMatriz* GrafoMatriz::get_copia() const {
    GrafoMatriz* copia = new GrafoMatriz();
    copia->inicia_grafo(n_vertices, grafo_direcionado);
    for (int i = 0; i < n_vertices; i++) {
        if (vertices_ponderado) {
            copia->vertices[i] = vertices[i];
        }
        for (int j = 0; j < n_vertices; j++) {
            copia->set_aresta(i, j, get_aresta(i, j));
        }
    }
    return copia;
}

int GrafoMatriz::get_numero_vertices_conexos(int vertice) const{
    bool* vet_vertices_visitados = new bool[n_vertices];
    for(int i = 0; i < n_vertices; i++){
        vet_vertices_visitados[i] = false;
    }
    int numero_vertices_conexos = aux_get_numero_vertices_conexos(vertice, vet_vertices_visitados);
    delete[] vet_vertices_visitados;
    return numero_vertices_conexos;
}

int GrafoMatriz::aux_get_numero_vertices_conexos(int vertice, bool* vet_vertices_visitados) const{
    vet_vertices_visitados[vertice] = true;
    int numero_vertices_visitados = 1;
    for(int i = 0; i < n_vertices; i++){
        if(!vet_vertices_visitados[i] && get_aresta(vertice, i) !=0){
           numero_vertices_visitados += aux_get_numero_vertices_conexos(i, vet_vertices_visitados);
        }
    }
    return numero_vertices_visitados;
}

int GrafoMatriz::n_conexo() const {
    int numero_componentes_conexas = 0;
    bool* vet_vertices_visitados = new bool[n_vertices];
    for(int i = 0; i < n_vertices; i++){
        vet_vertices_visitados[i] = false;
    }
    for(int i = 0; i < n_vertices; i++){
        if(!vet_vertices_visitados[i]){
            aux_get_numero_vertices_conexos(i, vet_vertices_visitados);
            numero_componentes_conexas++;
        }
    }
    delete[] vet_vertices_visitados;
    return numero_componentes_conexas;
}

void GrafoMatriz::imprimir_grafo_formato_txt(int vertice) {
    cout<<"grafo.txt"<<endl;
    cout<<endl;
    cout<<"Grau: "<<get_grau(vertice)<<endl;
    cout<<"Ordem: "<<get_ordem()<<endl;
    cout<<"Direcionado: "<<eh_direcionado()<<endl;
    cout<<"Componentes conexas: "<<n_conexo()<<endl;
    cout<<"Vertices ponderados: "<<vertice_ponderado()<<endl;
    cout<<"Arestas ponderadas: "<<aresta_ponderada()<<endl;
    cout<<"Completo: "<<eh_completo()<<endl;
    cout<<"Bipartido: "<<eh_bipartido()<<endl;
    cout<<"Arvore: "<<eh_arvore()<<endl;
    cout<<"Aresta Ponte: "<<possui_ponte()<<endl;
    cout<<"Vertice Articulação: "<<possui_articulacao()<<endl;
}


/*
Para testes com o arquivo grafo.txt
*/

int main() {
    GrafoMatriz *g = new GrafoMatriz();
    string arquivo = "grafo2.txt";
    g->carrega_grafo(arquivo);
    g->imprimir_grafo();
    cout << g->possui_articulacao() << endl;
    return 0;
}
