/**
 * @file Relogio.cpp
 * @brief Implementacao dos metodos da classe Relogio.
 */

#include "Relogio.h"

/**
 * @brief Construtor da classe Relogio.
 */
Relogio::Relogio(int _velocidade) {
    tInicio = time(NULL);
    segundosAcumulados = 0;
    if (_velocidade >= 1) {
        velocidade = _velocidade;
    } else {
        velocidade = 1;
    }
}

/**
 * @brief Destrutor da classe Relogio.
 */
Relogio::~Relogio() {}

/**
 * @brief Obtem o total de segundos simulados decorridos desde o arranque do relogio.
 * @return Segundos simulados decorridos.
 */
long Relogio::GetSegundosSimulados() const {
    time_t agora = time(NULL);
    return segundosAcumulados + (long)(velocidade * (agora - tInicio));
}

/**
 * @brief Calcula o tempo atual da simulacao somando ao inicio o tempo simulado decorrido.
 * @return Timestamp simulado atual.
 */
time_t Relogio::GetTempo() const {
    return tInicio + (time_t)GetSegundosSimulados();
}

/**
 * @brief Altera a velocidade da simulacao acumulando o tempo simulado anterior.
 */
void Relogio::MudarVelocidade(int vel) {
    if (vel >= 1) {
        segundosAcumulados = GetSegundosSimulados();
        tInicio = time(NULL);
        velocidade = vel;
    }
}

/**
 * @brief Obtem a velocidade atual configurada no relogio.
 * @return Fator de velocidade atual.
 */
int Relogio::GetVelocidade() const {
    return velocidade;
}
