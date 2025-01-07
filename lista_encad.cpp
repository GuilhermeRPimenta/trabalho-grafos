#include "lista_encad.h"
#include <iostream>
#include <fstream>

using namespace std;

ListaEncadeada::ListaEncadeada() : primeiro(nullptr) {}


ListaEncadeada::~ListaEncadeada() {
    No* p = primeiro;
    while (p != nullptr) {
        No* t = p->getProx();
        delete p;
        p = t;
    }
}

int ListaEncadeada::get(int k) const {
    if (k < 0) {
        cout << "Posição inválida!" << endl;
        exit(1);
    }
    int i = 0;
    for (No* p = primeiro; p != nullptr; p = p->getProx(), i++) {
        if (i == k)
            return p->getInfo();
    }
    cout << "Posição inválida!" << endl;
    exit(1);
}

void ListaEncadeada::insereFinal(int val, float peso) {
    No* ultimo = nullptr;
    if (primeiro != nullptr) {
        for (ultimo = primeiro; ultimo->getProx() != nullptr; ultimo = ultimo->getProx()) {}
    }

    No* p = new No(val, peso, nullptr);

    if (primeiro == nullptr)
        primeiro = p;
    else
        ultimo->setProx(p);
}

void ListaEncadeada::imprimir() const {
    for (No* p = primeiro; p != nullptr; p = p->getProx()) {
        cout << p->getInfo() << " ";
    }
    cout << endl;
}

int ListaEncadeada::tamanho() const {
    int count = 0;
    No* temp = primeiro;
    while (temp != nullptr) {
        count++;
        temp = temp->getProx();
    }
    return count;
}

int* ListaEncadeada::obter_elementos() const {
    int qtd = tamanho(); 
    int* elementos = new int[qtd]; 
    int i = 0;

    No* p = primeiro;
    p->getProx();
    while (p != nullptr) {
        elementos[i++] = p->getInfo(); 
        p = p->getProx();
    }
    
    return elementos;  // array com os elementos
}

No* ListaEncadeada::getPrimeiro() {
        return primeiro;
}

void ListaEncadeada::setPesoV(float peso) {
    pesoV = peso;
}

float ListaEncadeada::getPesoV() {
    return pesoV;
}

void ListaEncadeada::escrever(std::ofstream &saida, int origem) const {
    No* atual = primeiro;
    while (atual != nullptr) {
        int destino = atual->getInfo();
        float peso = atual->getPeso();
        
        // Verifica se a aresta é ponderada ou não
        if (pesoV > 1.0) {  
            saida << origem+1 << " " << destino+1 << " " << peso << std::endl;
        } else {
            saida << origem+1 << " " << destino+1 << std::endl;
        }
        atual = atual->getProx();
    }
}

void ListaEncadeada::remove(int val) {
    // Caso a lista esteja vazia
    if (primeiro == nullptr) {
        cout << "Erro: A lista está vazia! Não é possível remover elementos." << endl;
        return;
    }

    // Caso o valor esteja no primeiro nó
    if (primeiro->getInfo() == val) {
        No* temp = primeiro;
        primeiro = primeiro->getProx();
        delete temp;
        return;
    }

    // Percorrer a lista para encontrar o nó a ser removido
    No* atual = primeiro;
    No* anterior = nullptr;

    while (atual != nullptr && atual->getInfo() != val) {
        anterior = atual;
        atual = atual->getProx();
    }

    // Caso o valor não seja encontrado
    if (atual == nullptr) {
        cout << "Erro: Elemento " << val << " não encontrado na lista." << endl;
        return;
    }

    // Remover o nó
    anterior->setProx(atual->getProx());
    delete atual;
}

void ListaEncadeada::limpar()
{
    No* p = primeiro;
    while (p != nullptr) {
        No* t = p->getProx();
        delete p;
        p = t;
    }
    primeiro = nullptr;
}
