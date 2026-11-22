#include "Tipos_Sensores.h"

SensorLuz::SensorLuz(int idIn, const string& marcaIn, double valorIn, double probAvariaIn, int xIn, int yIn, double limiarIn)
    : Sensor(idIn, marcaIn, valorIn, probAvariaIn, xIn, yIn), limiar(limiarIn) {}

SensorLuz::~SensorLuz() {}

bool SensorLuz::EmAviso() const {
    return !avariado && (valor < limiar);
}

void SensorLuz::DefinirValorSeguro() {
    valor = limiar + 20.0;
}

string SensorLuz::GetTipo() const {
    return "LUZ";
}

SensorHumidade::SensorHumidade(int idIn, const string& marcaIn, double valorIn, double probAvariaIn, int xIn, int yIn, double limiarIn, int raioIn)
    : Sensor(idIn, marcaIn, valorIn, probAvariaIn, xIn, yIn), limiar(limiarIn), raioVizinhanca(raioIn) {}

SensorHumidade::~SensorHumidade() {}

bool SensorHumidade::EmAviso() const {
    return !avariado && (valor >= limiar);
}

void SensorHumidade::DefinirValorSeguro() {
    valor = limiar - 15.0;
    if (valor < 0.0) valor = 0.0;
}

int SensorHumidade::GetRaioVizinhanca() const {
    return raioVizinhanca;
}

string SensorHumidade::GetTipo() const {
    return "HUMIDADE";
}

SensorTemperatura::SensorTemperatura(int idIn, const string& marcaIn, double valorIn, double probAvariaIn, int xIn, int yIn, double limiarIn)
    : Sensor(idIn, marcaIn, valorIn, probAvariaIn, xIn, yIn), limiar(limiarIn) {}

SensorTemperatura::~SensorTemperatura() {}

bool SensorTemperatura::EmAviso() const {
    return !avariado && (valor >= limiar);
}

void SensorTemperatura::DefinirValorSeguro() {
    valor = 22.0;
}

string SensorTemperatura::GetTipo() const {
    return "TEMPERATURA";
}

SensorFumo::SensorFumo(int idIn, const string& marcaIn, double valorIn, double probAvariaIn, int xIn, int yIn, double limiarIn)
    : Sensor(idIn, marcaIn, valorIn, probAvariaIn, xIn, yIn), limiar(limiarIn) {}

SensorFumo::~SensorFumo() {}

bool SensorFumo::EmAviso() const {
    return !avariado && (valor >= limiar);
}

void SensorFumo::DefinirValorSeguro() {
    valor = 0.0;
}

string SensorFumo::GetTipo() const {
    return "FUMO";
}

SensorFogo::SensorFogo(int idIn, const string& marcaIn, double valorIn, double probAvariaIn, int xIn, int yIn, double limiarIn)
    : Sensor(idIn, marcaIn, valorIn, probAvariaIn, xIn, yIn), limiar(limiarIn) {}

SensorFogo::~SensorFogo() {}

bool SensorFogo::EmAviso() const {
    return !avariado && (valor >= limiar);
}

void SensorFogo::DefinirValorSeguro() {
    valor = 0.0;
}

string SensorFogo::GetTipo() const {
    return "FOGO";
}

SensorMissel::SensorMissel(int idIn, const string& marcaIn, double valorIn, double probAvariaIn, int xIn, int yIn)
    : Sensor(idIn, marcaIn, valorIn, probAvariaIn, xIn, yIn) {}

SensorMissel::~SensorMissel() {}

bool SensorMissel::EmAviso() const {
    return !avariado && (valor >= 1.0);
}

void SensorMissel::DefinirValorSeguro() {
    valor = 0.0;
}

string SensorMissel::GetTipo() const {
    return "MISSEL";
}
