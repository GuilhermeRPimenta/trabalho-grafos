#include "no.h"

no::no(){}

no::no(int val, no *p){
    info = val;
    prox = p;
}

no::~no(){}

int no::getInfo(){
    return info;
}

no* no::getProx(){
    return prox;
}

void no::setInfo(int val){
    info = val;
}

void no::setProx(no *p){
    prox = p;
}