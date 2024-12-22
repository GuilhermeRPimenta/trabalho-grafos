#include <iostream>
#include "lista_encad.h"

using namespace std;

lista_encad::lista_encad(){
    primeiro = NULL;
}

lista_encad::~lista_encad(){
    no *p = primeiro;
    while(p != NULL)
    {
        no *t = p->getProx();
        delete p;
        p = t;
    }
}

int lista_encad::get(int k){
    if(k < 0){
        cout << "Posicao invalida" << endl;
        exit(1);
    }
    int i = 0;
    for(no *p = primeiro; p != NULL; p = p->getProx(), i++)
        if(i == k)
            return p->getInfo();
    cout << "Posicao invalida" << endl;
    exit(1);
}

void lista_encad::insere_final(int val){
    no *ultimo;
    if(primeiro != NULL)
        for(ultimo = primeiro; ultimo->getProx() != NULL; ultimo = ultimo->getProx())
        {}
    no *p = new no();
    p->setInfo(val);
    p->setProx(NULL);
    if(primeiro == NULL)
        primeiro = p;
    else
        ultimo->setProx(p);
}