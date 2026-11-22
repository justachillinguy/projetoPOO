#include "Tipos_Motor.h"

// ==================== MOTOR COMBUSTAO ====================
MotorCombustao::MotorCombustao(int id, const string& marca, double consumoHora,
                               double tempAviso, double tempParagem, double probAvaria, int x, int y)
    : Motor(id, "MCOMBUSTAO", marca, consumoHora, tempAviso, tempParagem, probAvaria, x, y) {}

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

// ==================== MOTOR ELETRICO ====================
MotorEletrico::MotorEletrico(int id, const string& marca, double consumoHora,
                             double tempAviso, double tempParagem, double probAvaria, int x, int y)
    : Motor(id, "MELETRICO", marca, consumoHora, tempAviso, tempParagem, probAvaria, x, y) {}

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

// ==================== MOTOR INDUCAO ====================
MotorInducao::MotorInducao(int id, const string& marca, double consumoHora,
                           double tempAviso, double tempParagem, double probAvaria, int x, int y)
    : Motor(id, "MINDUCAO", marca, consumoHora, tempAviso, tempParagem, probAvaria, x, y) {}

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
