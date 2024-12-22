#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED

class no
{
  public:
    no();
    no(int val, no *p);
    ~no();
    int getInfo();
    no* getProx();
    void setInfo(int val);
    void setProx(no *p);
  private:
    int info;
    no *prox;
};

#endif
