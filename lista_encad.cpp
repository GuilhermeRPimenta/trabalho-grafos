#include "lista_encad.h"
#include <iostream>

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