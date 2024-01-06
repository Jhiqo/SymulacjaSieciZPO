//
// Created by julia on 30.11.2023.
//


#include "nodes.hpp"

void Worker::do_work(Time t) {
    if (pst_ == 0){
        if (!q_->empty()){
            pst_ = t;
            pbufor_.emplace(q_->pop());
        } else {
            return;
        }
    }
    if (t - (pst_ + pd_ - 1) == 0){ // jesli praca skonczona
        push_package(std::move(pbufor_.value()));
        pbufor_.reset();
        pst_ = 0;
    }
}

void Worker::receive_package(Package &&p) {
    q_->push(std::move(p));
}

void PackageSender::send_package() {
    IPackageReceiver *receiver;
    if (bufor_) {
        receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(bufor_.value()));
        bufor_.reset();
    }
}

void Ramp::deliver_goods(Time t) {
    if (t % di_ == 1) {
        push_package(Package());
    }
}

void ReceiverPreferences::add_receiver(IPackageReceiver *r){
    double first_num_of_receivers = double(preferences_.size());
    if (first_num_of_receivers == 0){
        preferences_[r] = 1.0;
    }
    else{
        for(auto &receiver : preferences_){
            receiver.second = 1/(first_num_of_receivers + 1);
        }
        preferences_[r] = 1/(first_num_of_receivers + 1);
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    double first_num_of_receivers = double(preferences_.size());
    if (first_num_of_receivers > 1){
        for (auto &receiver : preferences_){
            receiver.second = 1/(first_num_of_receivers - 1);
        }
    }
    preferences_.erase(r);
}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    auto p = pg_();
    if (p >= 0 && p <= 1){
        double D = 0.0;
        for (auto &receiver : preferences_){
            D += receiver.second;
            if (D < 0 || D > 1) {
                return nullptr;
            }
            if (p <= D) {
                return receiver.first;
            }
        }
    }
    return nullptr;
}
