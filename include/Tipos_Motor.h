#ifndef TIPOS_MOTORES_H
#define TIPOS_MOTORES_H

#include "Motor.h"

/**
 * @brief Motor de combustao interna (aquece mais rapidamente em funcionamento).
 */
class MotorCombustao : public Motor {
public:
    MotorCombustao(int idIn, const string& marcaIn, double consumoHoraIn,
                   double tempAvisoIn, double tempParagemIn, double probAvariaIn, int xIn, int yIn);
    virtual ~MotorCombustao();

    virtual void RUN();
    virtual CLASSE_TEMPERATURA ESTOU_QUENTE();
};

/**
 * @brief Motor eletrico (aquecimento moderado).
 */
class MotorEletrico : public Motor {
public:
    MotorEletrico(int idIn, const string& marcaIn, double consumoHoraIn,
                  double tempAvisoIn, double tempParagemIn, double probAvariaIn, int xIn, int yIn);
    virtual ~MotorEletrico();

    virtual void RUN();
    virtual CLASSE_TEMPERATURA ESTOU_QUENTE();
};

/**
 * @brief Motor de inducao (elevada eficiencia termica).
 */
class MotorInducao : public Motor {
public:
    MotorInducao(int idIn, const string& marcaIn, double consumoHoraIn,
                 double tempAvisoIn, double tempParagemIn, double probAvariaIn, int xIn, int yIn);
    virtual ~MotorInducao();

    virtual void RUN();
    virtual CLASSE_TEMPERATURA ESTOU_QUENTE();
};

#endif
