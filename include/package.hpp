//
// Created by julia on 30.11.2023.
//

#ifndef SYMULACJASIECIZPO_PACKAGE_HPP
#define SYMULACJASIECIZPO_PACKAGE_HPP

#include <set>
#include "types.hpp"

//definicja klasy Package - do 8 grudnia
class Package{
public:
    Package();
    Package(ElementID id);
    Package(Package&&) = default;
    Package(const Package& p) = default;
    Package& operator = (Package&&) = default;

    ElementID get_id() const {return id_;}
    ~Package();
private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif //SYMULACJASIECIZPO_PACKAGE_HPP
