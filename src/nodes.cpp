//
// Created by julia on 30.11.2023.
//

=======
#include "nodes.hpp"

void Worker::do_work(Time t) {
    if (pst_ == 0){
        if (!q_->empty()){
            pst_ = t;
        } else {
            return;
        }
    }
    if (t - (pst_ + pd_ - 1) == 0){ // jesli praca skonczona
        push_package(std::move(q_->pop()));
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
        receiver->receive_package(std::move(*bufor_));
        bufor_.reset();
    }
}

void Ramp::deliver_goods(Time t) {
    if (!bufor_) {
        push_package(Package());
        bufor_.emplace(id_);
        t_ = t;
    }
    else {
        if (t - di_ == t_) {
            push_package(Package());
        }
    }