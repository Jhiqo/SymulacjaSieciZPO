//
// Created by julia on 30.11.2023.
//

#ifndef SYMULACJASIECIZPO_STORAGE_TYPES_HPP
#define SYMULACJASIECIZPO_STORAGE_TYPES_HPP

#include <cstdio>

// definicje klas IPackageStockpile (done), IPackageQueue i PackageQueue
// oraz typu wyliczeniowego PackageQueueType - do 8 grudnia

enum class PackageQueueType{
    LIFO, FIFO
};

class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& package) = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;

    virtual std::size_t size() const = 0;
    virtual bool empty() = 0;
    virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile{
    throw;
};

class PackageQueue{
    throw;
};

#endif //SYMULACJASIECIZPO_STORAGE_TYPES_HPP
