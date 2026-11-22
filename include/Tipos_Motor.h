#ifndef TIPOS_MOTORES_H
#define TIPOS_MOTORES_H

#include "Motor.h"

/**
 * @brief Motor de combustao interna (aquece mais rapidamente em funcionamento).
 */
class MotorCombustao : public Motor {
public:
    MotorCombustao(int id, const string& marca, double consumoHora,
                   double tempAviso, double tempParagem, double probAvaria, int x, int y);
    virtual ~MotorCombustao();

    virtual void RUN();
    virtual CLASSE_TEMPERATURA ESTOU_QUENTE();
};

/**
 * @brief Motor eletrico (aquecimento moderado).
 */
class MotorEletrico : public Motor {
public:
    MotorEletrico(int id, const string& marca, double consumoHora,
                  double tempAviso, double tempParagem, double probAvaria, int x, int y);
    virtual ~MotorEletrico();

    virtual void RUN();
    virtual CLASSE_TEMPERATURA ESTOU_QUENTE();
};

/**
 * @brief Motor de inducao (elevada eficiencia termica).
 */
class MotorInducao : public Motor {
public:
    MotorInducao(int id, const string& marca, double consumoHora,
                 double tempAviso, double tempParagem, double probAvaria, int x, int y);
    virtual ~MotorInducao();

    virtual void RUN();
    virtual CLASSE_TEMPERATURA ESTOU_QUENTE();
};

#endif // TIPOS_MOTORES_H
