//
// Created by julia on 30.11.2023.
//

#include "package.hpp"

static std::set<ElementID> assigned_IDs = {};
static std::set<ElementID> freed_IDs = {};

Package::Package(){
    if (assigned_IDs.empty() && freed_IDs.empty()){
        id_ = 1;
    }
    else if (!freed_IDs.empty()){
        id_ = *freed_IDs.begin();
        freed_IDs.erase(*freed_IDs.begin());
    }
    else if (!assigned_IDs.empty()){
        id_ = *assigned_IDs.end() + 1;
    }
    assigned_IDs.insert(id_);
}

Package &Package::operator=(Package &&package) noexcept{
    if (this == &package){
        return *this;
    }
    assigned_IDs.erase(id_);
    freed_IDs.insert(id_);
    id_ = package.id_;
    assigned_IDs.insert(id_);
    return *this;
}

Package::~Package(){
    assigned_IDs.erase(id_);
    freed_IDs.insert(id_)
}