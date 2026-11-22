#include "Tipos_Users.h"

Admin::Admin(int idIn, const string& nomeIn)
    : User(idIn, nomeIn) {}

Admin::~Admin() {}

bool Admin::PodeAdicionar() const { return true; }
bool Admin::PodeRemover() const { return true; }
bool Admin::PodeListar() const { return true; }
bool Admin::PodeRun() const { return true; }
bool Admin::PodeManutencao() const { return true; }
string Admin::GetTipo() const { return "ADMIN"; }

Normal::Normal(int idIn, const string& nomeIn)
    : User(idIn, nomeIn) {}

Normal::~Normal() {}

bool Normal::PodeAdicionar() const { return true; }
bool Normal::PodeRemover() const { return false; }
bool Normal::PodeListar() const { return true; }
bool Normal::PodeRun() const { return true; }
bool Normal::PodeManutencao() const { return true; }
string Normal::GetTipo() const { return "NORMAL"; }

Visitante::Visitante(int idIn, const string& nomeIn)
    : User(idIn, nomeIn) {}

Visitante::~Visitante() {}

bool Visitante::PodeAdicionar() const { return false; }
bool Visitante::PodeRemover() const { return false; }
bool Visitante::PodeListar() const { return true; }
bool Visitante::PodeRun() const { return false; }
bool Visitante::PodeManutencao() const { return false; }
string Visitante::GetTipo() const { return "VISITANTE"; }
