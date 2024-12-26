#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

#include <iostream>
#include "no.h"

class ListaEncadeada {
private:
    No* primeiro;
    float pesoV;

public:
    ListaEncadeada();
    ~ListaEncadeada();

    void insereFinal(int val, float peso = 1.0); 
    int get(int k) const;
    No* getPrimeiro();
    int tamanho() const;
    void imprimir() const;

    void setPesoV(float peso);
    int* obter_elementos() const; // elementos como  array

    void escrever(std::ofstream &saida, int origem) const;

};

#endif 
