#include "Tipos_Motor.h"

MotorCombustao::MotorCombustao(int idIn, const string& marcaIn, double consumoHoraIn,
                               double tempAvisoIn, double tempParagemIn, double probAvariaIn, int xIn, int yIn)
    : Motor(idIn, "MCOMBUSTAO", marcaIn, consumoHoraIn, tempAvisoIn, tempParagemIn, probAvariaIn, xIn, yIn) {}

MotorCombustao::~MotorCombustao() {}

void MotorCombustao::RUN() {
    if (!avariado && (estado == ESTADO_MOTOR::START || estado == ESTADO_MOTOR::RUN)) {
        estado = ESTADO_MOTOR::RUN;
        tempoTrabalho += 10;
        consumoDia += consumoHora * (10.0 / 60.0);
        temperaturaAtual += 3.5;

        if (ESTOU_QUENTE() == VERMELHO) {
            STOP();
        }
    }
}

CLASSE_TEMPERATURA MotorCombustao::ESTOU_QUENTE() {
    return Motor::ESTOU_QUENTE();
}

MotorEletrico::MotorEletrico(int idIn, const string& marcaIn, double consumoHoraIn,
                             double tempAvisoIn, double tempParagemIn, double probAvariaIn, int xIn, int yIn)
    : Motor(idIn, "MELETRICO", marcaIn, consumoHoraIn, tempAvisoIn, tempParagemIn, probAvariaIn, xIn, yIn) {}

MotorEletrico::~MotorEletrico() {}

void MotorEletrico::RUN() {
    if (!avariado && (estado == ESTADO_MOTOR::START || estado == ESTADO_MOTOR::RUN)) {
        estado = ESTADO_MOTOR::RUN;
        tempoTrabalho += 10;
        consumoDia += consumoHora * (10.0 / 60.0);
        temperaturaAtual += 2.0;

        if (ESTOU_QUENTE() == VERMELHO) {
            STOP();
        }
    }
}

CLASSE_TEMPERATURA MotorEletrico::ESTOU_QUENTE() {
    return Motor::ESTOU_QUENTE();
}

MotorInducao::MotorInducao(int idIn, const string& marcaIn, double consumoHoraIn,
                           double tempAvisoIn, double tempParagemIn, double probAvariaIn, int xIn, int yIn)
    : Motor(idIn, "MINDUCAO", marcaIn, consumoHoraIn, tempAvisoIn, tempParagemIn, probAvariaIn, xIn, yIn) {}

MotorInducao::~MotorInducao() {}

void MotorInducao::RUN() {
    if (!avariado && (estado == ESTADO_MOTOR::START || estado == ESTADO_MOTOR::RUN)) {
        estado = ESTADO_MOTOR::RUN;
        tempoTrabalho += 10;
        consumoDia += consumoHora * (10.0 / 60.0);
        temperaturaAtual += 1.5;

        if (ESTOU_QUENTE() == VERMELHO) {
            STOP();
        }
    }
}

CLASSE_TEMPERATURA MotorInducao::ESTOU_QUENTE() {
    return Motor::ESTOU_QUENTE();
}
