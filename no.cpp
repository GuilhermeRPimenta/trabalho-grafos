#include "no.h"

//No::No() : info(0), peso(1.0f), prox(nullptr) {}

No::No(int val, float p, No* pNext) : info(val), peso(p), prox(pNext) {}

No::~No() {}

int No::getInfo() { return info; }

float No::getPeso() { return peso; }

No* No::getProx() { return prox; }

void No::setInfo(int val) { info = val; }

void No::setPeso(float p) { peso = p; }

void No::setProx(No* p) { prox = p; }
