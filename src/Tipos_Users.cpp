#include "Tipos_Users.h"

// ==================== ADMIN ====================
Admin::Admin(int id, const string& nome)
    : User(id, nome) {}

Admin::~Admin() {}

bool Admin::PodeAdicionar() const { return true; }
bool Admin::PodeRemover() const { return true; }
bool Admin::PodeListar() const { return true; }
bool Admin::PodeRun() const { return true; }
bool Admin::PodeManutencao() const { return true; }
string Admin::GetTipo() const { return "ADMIN"; }

// ==================== NORMAL ====================
Normal::Normal(int id, const string& nome)
    : User(id, nome) {}

Normal::~Normal() {}

bool Normal::PodeAdicionar() const { return true; }
bool Normal::PodeRemover() const { return false; }
bool Normal::PodeListar() const { return true; }
bool Normal::PodeRun() const { return true; }
bool Normal::PodeManutencao() const { return true; }
string Normal::GetTipo() const { return "NORMAL"; }

// ==================== VISITANTE ====================
Visitante::Visitante(int id, const string& nome)
    : User(id, nome) {}

Visitante::~Visitante() {}

bool Visitante::PodeAdicionar() const { return false; }
bool Visitante::PodeRemover() const { return false; }
bool Visitante::PodeListar() const { return true; }
bool Visitante::PodeRun() const { return false; }
bool Visitante::PodeManutencao() const { return false; }
string Visitante::GetTipo() const { return "VISITANTE"; }
