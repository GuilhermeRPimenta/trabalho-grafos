#ifndef No_H_INCLUDED
#define No_H_INCLUDED

class No
{
  public:
    //No();
    No(int val, float peso, No* p = nullptr);
    No(int val, No *p);
    ~No();

    int getInfo();
    No* getProx();
    float getPeso();

    void setInfo(int val);
    void setProx(No *p);
    void setPeso(float peso);

    
  private:
    int info;
    float peso;
    No *prox;
};

#endif