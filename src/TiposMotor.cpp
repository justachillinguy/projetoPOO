#include "TiposMotor.h"

using namespace std;

MotorCombustao::MotorCombustao(int id, const string& marca, double consumoHora,
                               double probabilidadeAvaria, int posX, int posY,
                               const IntervaloTemperatura& v,
                               const IntervaloTemperatura& a,
                               const IntervaloTemperatura& vm)
    : Motor(id, "MCOMBUSTAO", marca, consumoHora,
            a.minimo,
            vm.minimo,
            probabilidadeAvaria, posX, posY),
      verde(v), amarelo(a), vermelho(vm)
{
}

MotorCombustao::~MotorCombustao()
{
}

string MotorCombustao::GetTipo() const
{
    return "MCOMBUSTAO";
}

IntervaloTemperatura MotorCombustao::GetIntervaloVerde() const   { return verde; }
IntervaloTemperatura MotorCombustao::GetIntervaloAmarelo() const { return amarelo; }
IntervaloTemperatura MotorCombustao::GetIntervaloVermelho() const{ return vermelho; }

/** @brief Combustao aquece mais depressa: 1.5x os graus. */
void MotorCombustao::Aquecer(double graus)
{
    Motor::Aquecer(graus * 1.5);
}

MotorEletrico::MotorEletrico(int id, const string& marca, double consumoHora,
                             double probabilidadeAvaria, int posX, int posY,
                             const IntervaloTemperatura& v,
                             const IntervaloTemperatura& a,
                             const IntervaloTemperatura& vm)
    : Motor(id, "MELETRICO", marca, consumoHora,
            a.minimo, vm.minimo,
            probabilidadeAvaria, posX, posY),
      verde(v), amarelo(a), vermelho(vm)
{
}

MotorEletrico::~MotorEletrico()
{
}

string MotorEletrico::GetTipo() const
{
    return "MELETRICO";
}

IntervaloTemperatura MotorEletrico::GetIntervaloVerde() const    { return verde; }
IntervaloTemperatura MotorEletrico::GetIntervaloAmarelo() const  { return amarelo; }
IntervaloTemperatura MotorEletrico::GetIntervaloVermelho() const { return vermelho; }

/** @brief Eletrico aquece devagar: 0.5x os graus. */
void MotorEletrico::Aquecer(double graus)
{
    Motor::Aquecer(graus * 0.5);
}

MotorInducao::MotorInducao(int id, const string& marca, double consumoHora,
                           double probabilidadeAvaria, int posX, int posY,
                           const IntervaloTemperatura& v,
                           const IntervaloTemperatura& a,
                           const IntervaloTemperatura& vm)
    : Motor(id, "MINDUCAO", marca, consumoHora,
            a.minimo, vm.minimo,
            probabilidadeAvaria, posX, posY),
      verde(v), amarelo(a), vermelho(vm)
{
}

MotorInducao::~MotorInducao()
{
}

string MotorInducao::GetTipo() const
{
    return "MINDUCAO";
}

IntervaloTemperatura MotorInducao::GetIntervaloVerde() const    { return verde; }
IntervaloTemperatura MotorInducao::GetIntervaloAmarelo() const  { return amarelo; }
IntervaloTemperatura MotorInducao::GetIntervaloVermelho() const { return vermelho; }

/** @brief Inducao aquece muito depressa: 2x os graus. */
void MotorInducao::Aquecer(double graus)
{
    Motor::Aquecer(graus * 2.0);
}
