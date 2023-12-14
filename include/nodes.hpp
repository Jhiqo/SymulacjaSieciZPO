//
// Created by julia on 30.11.2023.
//

#ifndef SYMULACJASIECIZPO_NODES_HPP
#define SYMULACJASIECIZPO_NODES_HPP


class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;
    PackageSender() = default;
    PackageSender(PackageSender &&pack_sender) = default;
    void send_package();
    const std::optional<Package> &get_sending_buffer() const {return bufor_;}

protected:
    void push_package(Package &&package) {bufor_.emplace(package.get_id());};

private:
    std::optional<Package> bufor_ = std::nullopt;
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : PackageSender(), id_(id), di_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const {return di_;}
    ElementID get_id() const {return id_;}

private:
    ElementID id_;
    TimeOffset di_;
    Time t_;
    std::optional<Package> bufor_ = std::nullopt;
};

#endif //SYMULACJASIECIZPO_NODES_HPP
