//
// Created by julia on 30.11.2023.
//

#ifndef SYMULACJASIECIZPO_STORAGE_TYPES_HPP
#define SYMULACJASIECIZPO_STORAGE_TYPES_HPP

//definicje klas IPackageStockpile, IPackageQueue i PackageQueue
// oraz typu wyliczeniowego PackageQueueType - do 8 grudnia

enum class PackageQueueType{
    LIFO, FIFO
};

class IPackageStockpile{
public:
    void push();

    // tu metody pozwalające na uzyskanie dostępu “tylko do odczytu” do
    // kontenera przechowującego półprodukty
    // (tj. metody [c]begin(), [c]end() – łącznie 4 metody)

    int size() const;
    bool empty();
    virtual ~IPackageStockpile() = 0;
};

class IPackageQueue{
    throw;
};

class PackageQueue{
    throw;
};

#endif //SYMULACJASIECIZPO_STORAGE_TYPES_HPP
