#include "TiposSensor.h"

#include <cstdlib>

using namespace std;

SensorLuz::SensorLuz(int id, const string& marca, double valor, double probabilidadeAvaria,
                     int posX, int posY, double limiar)
    : Sensor(id, marca, valor, probabilidadeAvaria, posX, posY), limiar(limiar)
{
}

SensorLuz::~SensorLuz()
{
}

double SensorLuz::GetLimiar() const
{
    return limiar;
}

string SensorLuz::GetTipo() const
{
    return "LUZ";
}

bool SensorLuz::EmAviso() const
{
    return valor < limiar;
}

void SensorLuz::Actualizar(bool, int horaAtual, int horaAbertura, int horaEncerramento, double)
{
    if (horaAtual == horaAbertura)
    {
        valor = limiar + 40.0;
    }

    if (horaEncerramento > 0 && horaAtual >= horaEncerramento - 1 && horaAtual < horaEncerramento)
    {
        valor = limiar - 1.0;
    }
}

void SensorLuz::DefinirValorSeguro()
{
    valor = limiar + 40.0;
}

SensorHumidade::SensorHumidade(int id, const string& marca, double valor, double probabilidadeAvaria,
                               int posX, int posY, double limiar, int raioVizinhanca)
    : Sensor(id, marca, valor, probabilidadeAvaria, posX, posY), limiar(limiar), raioVizinhanca(0)
{
    if (raioVizinhanca > 0)
    {
        this->raioVizinhanca = raioVizinhanca;
    }
}

SensorHumidade::~SensorHumidade()
{
}

double SensorHumidade::GetLimiar() const
{
    return limiar;
}

int SensorHumidade::GetRaioVizinhanca() const
{
    return raioVizinhanca;
}

string SensorHumidade::GetTipo() const
{
    return "HUMIDADE";
}

bool SensorHumidade::EmAviso() const
{
    return valor >= limiar;
}

void SensorHumidade::Actualizar(bool motoresEmMarcha, int, int, int, double)
{
    if (motoresEmMarcha)
    {
        if (valor < 100.0)
        {
            valor = valor + 0.5;
        }
    }
    else if (valor > 0.0)
    {
        valor = valor - 0.3;
        if (valor < 0.0)
        {
            valor = 0.0;
        }
    }
}

void SensorHumidade::DefinirValorSeguro()
{
    if (limiar > 1.0)
    {
        valor = limiar - 1.0;
    }
    else
    {
        valor = 0.0;
    }
}

SensorFumo::SensorFumo(int id, const string& marca, double valor, double probabilidadeAvaria,
                       int posX, int posY, double limiar, double probEvento)
    : Sensor(id, marca, valor, probabilidadeAvaria, posX, posY), limiar(limiar),
      probEvento(limitarProbabilidade(probEvento))
{
}

SensorFumo::~SensorFumo()
{
}

double SensorFumo::GetLimiar() const
{
    return limiar;
}

double SensorFumo::GetProbabilidadeEvento() const
{
    return probEvento;
}

string SensorFumo::GetTipo() const
{
    return "FUMO";
}

bool SensorFumo::EmAviso() const
{
    return valor >= limiar;
}

void SensorFumo::Actualizar(bool motoresEmMarcha, int, int, int, double fracaoHora)
{
    // O fumo e um evento aleatorio (probEvento por hora), so possivel com motores em marcha.
    if (valor >= limiar || EstaAvariado())
    {
        return;
    }

    if (motoresEmMarcha)
    {
        double sorteio = (rand() % 10000) / 10000.0;
        double limite  = probEvento * fracaoHora;
        if (sorteio < limite)
        {
            valor = limiar;
        }
    }
}

void SensorFumo::DefinirValorSeguro()
{
    valor = 0.0;
}

SensorMissel::SensorMissel(int id, const string& marca, double valor, double probabilidadeAvaria,
                           int posX, int posY, double probEvento)
    : Sensor(id, marca, valor, probabilidadeAvaria, posX, posY),
      probEvento(limitarProbabilidade(probEvento))
{
}

SensorMissel::~SensorMissel()
{
}

double SensorMissel::GetProbabilidadeEvento() const
{
    return probEvento;
}

string SensorMissel::GetTipo() const
{
    return "MISSEL";
}

bool SensorMissel::EmAviso() const
{
    return valor >= 1.0;
}

void SensorMissel::Actualizar(bool, int, int, int, double fracaoHora)
{
    if (valor >= 1.0 || EstaAvariado())
    {
        return;
    }

    double sorteio = (rand() % 10000) / 10000.0;
    double limite = probEvento * fracaoHora;
    if (sorteio < limite)
    {
        valor = 1.0;
    }
}

void SensorMissel::DefinirValorSeguro()
{
    valor = 0.0;
}

SensorTemperatura::SensorTemperatura(int id, const string& marca, double valor,
                                     double probabilidadeAvaria, int posX, int posY, double limiar)
    : Sensor(id, marca, valor, probabilidadeAvaria, posX, posY), limiar(limiar)
{
}

SensorTemperatura::~SensorTemperatura()
{
}

double SensorTemperatura::GetLimiar() const
{
    return limiar;
}

string SensorTemperatura::GetTipo() const
{
    return "TEMPERATURA";
}

bool SensorTemperatura::EmAviso() const
{
    return valor >= limiar;
}

void SensorTemperatura::Actualizar(bool motoresEmMarcha, int, int, int, double)
{
    if (motoresEmMarcha && valor < limiar + 10.0)
    {
        valor = valor + 0.5;
    }
    else if (!motoresEmMarcha && valor > 0.0)
    {
        valor = valor - 0.3;
        if (valor < 0.0)
        {
            valor = 0.0;
        }
    }
}

void SensorTemperatura::DefinirValorSeguro()
{
    if (limiar > 1.0)
    {
        valor = limiar - 1.0;
    }
    else
    {
        valor = 0.0;
    }
}

SensorFogo::SensorFogo(int id, const string& marca, double valor,
                       double probabilidadeAvaria, int posX, int posY, double limiar,
                       double probEvento)
    : Sensor(id, marca, valor, probabilidadeAvaria, posX, posY), limiar(limiar),
      probEvento(limitarProbabilidade(probEvento))
{
}

SensorFogo::~SensorFogo()
{
}

double SensorFogo::GetLimiar() const
{
    return limiar;
}

double SensorFogo::GetProbabilidadeEvento() const
{
    return probEvento;
}

string SensorFogo::GetTipo() const
{
    return "FOGO";
}

bool SensorFogo::EmAviso() const
{
    return valor >= limiar;
}

void SensorFogo::Actualizar(bool motoresEmMarcha, int, int, int, double fracaoHora)
{
    if (valor >= limiar || EstaAvariado())
    {
        return;
    }

    if (motoresEmMarcha)
    {
        double sorteio = (rand() % 10000) / 10000.0;
        double limite  = probEvento * fracaoHora;
        if (sorteio < limite)
        {
            valor = limiar;
        }
    }
}

void SensorFogo::DefinirValorSeguro()
{
    valor = 0.0;
}
