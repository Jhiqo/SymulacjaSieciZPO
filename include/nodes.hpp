//
// Created by julia on 30.11.2023.
//

#ifndef SYMULACJASIECIZPO_NODES_HPP
#define SYMULACJASIECIZPO_NODES_HPP

#include <memory>
#include <optional>

#include "types.hpp"
#include "package.hpp"
#include "storage_types.hpp"

//definicje klas IPackageReceiver(done), Storehouse, ReceiverPreferences, PackageSender, Ramp i Worker(done) - do 15 grudnia

enum class ReceiverType{
    Worker, Storehouse
};

class IPackageReceiver{
public:
    IPackageReceiver() = default;
    virtual ElementID get_id() const = 0;
    virtual void receive_package(Package&& p) = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual ~IPackageReceiver() = default;
protected:
    static inline ReceiverType receiver_type_;
};

class Worker : public IPackageReceiver, public PackageSender{
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)), pst_(0) {};
    void do_work(Time t);
    TimeOffset get_processing_duration() const {return pd_};
    Time get_package_processing_start_time() const {return pst_;};
    void receive_package(Package&& p) override;
    ElementID get_id() const override {return id_;}
    IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); }
    IPackageStockpile::const_iterator cend() const override { return q_->cend(); }
    IPackageStockpile::const_iterator begin() const override { return q_->begin(); }
    IPackageStockpile::const_iterator end() const override { return q_->end(); }
private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    Time pst_;
    static inline ReceiverType receiver_type_ = ReceiverType::Worker;
};

#endif //SYMULACJASIECIZPO_NODES_HPP
