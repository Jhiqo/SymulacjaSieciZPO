//
// Created by julia on 30.11.2023.
//

#include "package.hpp"

std::set<ElementID> Package::assigned_IDs {0};
std::set<ElementID> Package::freed_IDs;

Package::Package() {
    if (freed_IDs.empty() && assigned_IDs.empty()) {
        id_ = 1;
    }
    else if (!freed_IDs.empty()) {
        id_ = *freed_IDs.begin();
        freed_IDs.erase((*freed_IDs.begin()));
    }
    else if (!assigned_IDs.empty()) {
        //id_ = *assigned_IDs.end() + 1;
        id_ = *std::prev(assigned_IDs.end());
    }


    assigned_IDs.insert(id_);
}

Package::Package(ElementID id) {
    id_ = id;
    assigned_IDs.insert(id);
}

/*Package& Package::operator=(Package&& package) noexcept {
    if (this == &package){
        return *this;
    }
    assigned_IDs.erase(id_);
    freed_IDs.insert(id_);
    id_ = package.id_;
    assigned_IDs.insert(id_);
    return *this;
}*/

Package::~Package(){
    if (id_ != -1) {
        assigned_IDs.erase(id_);
        freed_IDs.insert(id_);
    }
}