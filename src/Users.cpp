#include "Users.h"

User::User(int id, const string& nome)
    : id(id), nome(nome) {}

User::~User() {}

int User::GetId() const {
    return id;
}

string User::GetNome() const {
    return nome;
}
