#include "Users.h"

User::User(int idIn, const string& nomeIn)
    : id(idIn), nome(nomeIn) {}

User::~User() {}

int User::GetId() const {
    return id;
}

string User::GetNome() const {
    return nome;
}
