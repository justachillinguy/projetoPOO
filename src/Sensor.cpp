#include "Sensor.h"

using namespace std;

Sensor::Sensor(int id, const string& marca, double valor, double probabilidadeAvaria, int posX, int posY)
    : Dispositivo(id, marca, probabilidadeAvaria, posX, posY), valor(valor), avisoAnterior(false)
{
}

Sensor::~Sensor()
{
}

double Sensor::GetValor() const
{
    return valor;
}

void Sensor::SetValor(double novoValor)
{
    valor = novoValor;
}

int Sensor::GetRaioVizinhanca() const
{
    return 0;
}

bool Sensor::Disparou()
{
    bool agora = EmAviso();
    bool novo = agora && !avisoAnterior;
    avisoAnterior = agora;
    return novo;
}

void Sensor::Actualizar(bool, int, int, int, double)
{
}

void Sensor::Repor()
{
    DefinirValorSeguro();
    RepararAvaria();
    avisoAnterior = false;
}
