#ifndef LISTAENCAD_H_INCLUDED
#define LISTAENCAD_H_INCLUDED
#include "No.h"

class lista_encad
{
  public:
    lista_encad();
    ~lista_encad();
    int get(int k);
    void insere_final(int val);
    no *getPrimeiro(){return primeiro;}
    void setPrimeiro(no* novoPrimeiro) {this->primeiro = novoPrimeiro;}

  private:
    no *primeiro;
};

#endif
