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
    void setPrimeiro(no *p) { primeiro = p; }
    no* getPrimeiro() { return primeiro; }

  private:
    no *primeiro;
};

#endif
