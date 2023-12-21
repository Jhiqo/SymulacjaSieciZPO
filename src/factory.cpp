#include "factory.hpp"
#include <istream>
#include <string>
#include <sstream>


template<class Node>
void NodeCollection<Node>::remove_by_id(ElementID id) {
    auto it = find_by_id(id);
    if (it != nodes_.end()){
        nodes_.erase(it);
    }
}

enum class NodeColour { UNVISITED, VISITED, VERIFIED };

template <class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    auto ptr = dynamic_cast<IPackageReceiver*>(&(*collection.find_by_id(id)));
    for (Worker& sender : worker_) {
        sender.receiver_preferences_.remove_receiver(ptr);
    }
    if (std::is_same<Node, Worker>::value) {
        for (Ramp& sender : ramp_) {
            sender.receiver_preferences_.remove_receiver(ptr);
        }
    }
}

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColour>& node_colours){
    if(node_colours[sender] == NodeColour::VERIFIED){return true;}
    node_colours[sender] = NodeColour::VISITED;

    if(sender -> receiver_preferences_.get_preferences().empty()){
        throw std::logic_error("Brak zdefiniowanych odbiorcow");
    }
    bool has_receivers = false;

    for(auto& [receiver, p]: sender -> receiver_preferences_.get_preferences()){
        if(receiver -> get_receiver_type() == ReceiverType::Storehouse){
            has_receivers = true;
        }
        else if (receiver->get_receiver_type() == ReceiverType::Worker) {
            IPackageReceiver* receiver_ptr = receiver;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto package_sender_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if(package_sender_ptr == sender){
                continue;
            }

            has_receivers = true;

            if(node_colours[package_sender_ptr] == NodeColour::UNVISITED){
                has_reachable_storehouse(package_sender_ptr, node_colours);
            }
        }
    }
    node_colours[sender] = NodeColour::VERIFIED;

    if(has_receivers)
        return true;
    else
        throw std::logic_error("Brak odbiorcow");
}

bool Factory::is_consistent() {
    std::map<const PackageSender*, NodeColour> colour;


    for (Worker &worker : worker_) {
        const PackageSender *worker_ptr = &worker;
        colour[worker_ptr] = NodeColour::UNVISITED;
    }
    for (Ramp &ramp : ramp_){
        const PackageSender *ramp_ptr = &ramp;
        colour[ramp_ptr] = NodeColour::UNVISITED;



    try {
        has_reachable_storehouse(ramp_ptr, colour);
    }
    catch (std::logic_error &err) {
        return false;
    }}
    return true;
}

void Factory::do_delivery(Time t) {
    for (auto& ramp : ramp_) {
        ramp.deliver_goods(t);
    }
};

void Factory::do_package_passing() {
    for (auto& ramp : ramp_) {
        ramp.send_package();
    }

    for (auto& worker : worker_) {
        worker.send_package();
    }
};

void Factory::do_work(Time t) {
    for (auto& worker : worker_) {
        worker.do_work(t);
    }
};

void Factory::remove_worker(ElementID id) {
    remove_receiver(worker_, id);
    worker_.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id) {
    remove_receiver(storehouse_, id);
    storehouse_.remove_by_id(id);
}

template <class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    auto ptr = dynamic_cast<IPackageReceiver*>(&(*collection.find_by_id(id)));
    for (Worker& sender : worker_) {
        sender.receiver_preferences_.remove_receiver(ptr);
    }
    if (std::is_same<Node, Worker>::value) {
        for (Ramp& sender : ramp_) {
            sender.receiver_preferences_.remove_receiver(ptr);
        }
    }
}
