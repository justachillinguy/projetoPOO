#include "Tipos_Sensores.h"

// ==================== SENSOR LUZ ====================
SensorLuz::SensorLuz(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar)
    : Sensor(id, marca, valor, probAvaria, x, y), limiar(limiar) {}

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

// ==================== SENSOR HUMIDADE ====================
SensorHumidade::SensorHumidade(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar, int raio)
    : Sensor(id, marca, valor, probAvaria, x, y), limiar(limiar), raioVizinhanca(raio) {}

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

// ==================== SENSOR TEMPERATURA ====================
SensorTemperatura::SensorTemperatura(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar)
    : Sensor(id, marca, valor, probAvaria, x, y), limiar(limiar) {}

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

// ==================== SENSOR FUMO ====================
SensorFumo::SensorFumo(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar)
    : Sensor(id, marca, valor, probAvaria, x, y), limiar(limiar) {}

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

// ==================== SENSOR FOGO ====================
SensorFogo::SensorFogo(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar)
    : Sensor(id, marca, valor, probAvaria, x, y), limiar(limiar) {}

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

// ==================== SENSOR MISSEL ====================
SensorMissel::SensorMissel(int id, const string& marca, double valor, double probAvaria, int x, int y)
    : Sensor(id, marca, valor, probAvaria, x, y) {}

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
