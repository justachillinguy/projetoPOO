#ifndef TIPOS_SENSORES_H
#define TIPOS_SENSORES_H

#include "Sensores.h"

/**
 * @brief Sensor de luminosidade (dispara aviso quando a luz desce abaixo do limiar).
 */
class SensorLuz : public Sensor {
private:
    double limiar;
public:
    SensorLuz(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar);
    double GetLimiar() const { return limiar; }
    virtual ~SensorLuz();

    virtual bool EmAviso() const;
    virtual void DefinirValorSeguro();
    virtual string GetTipo() const;
};

/**
 * @brief Sensor de humidade (desliga motores num dado raio de vizinhanca).
 */
class SensorHumidade : public Sensor {
private:
    double limiar;
    int raioVizinhanca;
public:
    SensorHumidade(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar, int raio);
    double GetLimiar() const { return limiar; }
    virtual ~SensorHumidade();

    virtual bool EmAviso() const;
    virtual void DefinirValorSeguro();
    virtual int GetRaioVizinhanca() const;
    virtual string GetTipo() const;
};

/**
 * @brief Sensor de temperatura ambiente.
 */
class SensorTemperatura : public Sensor {
private:
    double limiar;
public:
    SensorTemperatura(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar);
    double GetLimiar() const { return limiar; }
    virtual ~SensorTemperatura();

    virtual bool EmAviso() const;
    virtual void DefinirValorSeguro();
    virtual string GetTipo() const;
};

/**
 * @brief Sensor de detecao de fumo.
 */
class SensorFumo : public Sensor {
private:
    double limiar;
public:
    SensorFumo(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar);
    double GetLimiar() const { return limiar; }
    virtual ~SensorFumo();

    virtual bool EmAviso() const;
    virtual void DefinirValorSeguro();
    virtual string GetTipo() const;
};

/**
 * @brief Sensor de detecao de fogo / chamas.
 */
class SensorFogo : public Sensor {
private:
    double limiar;
public:
    SensorFogo(int id, const string& marca, double valor, double probAvaria, int x, int y, double limiar);
    double GetLimiar() const { return limiar; }
    virtual ~SensorFogo();

    virtual bool EmAviso() const;
    virtual void DefinirValorSeguro();
    virtual string GetTipo() const;
};

/**
 * @brief Sensor de detecao de misseis (dispara aviso quando valor == 1).
 */
class SensorMissel : public Sensor {
public:
    SensorMissel(int idIn, const string& marcaIn, double valorIn, double probAvariaIn, int xIn, int yIn);
    double GetLimiar() const { return 1.0; }
    virtual ~SensorMissel();

    virtual bool EmAviso() const;
    virtual void DefinirValorSeguro();
    virtual string GetTipo() const;
};

#endif
