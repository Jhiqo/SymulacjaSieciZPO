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

bool Factory::has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColour>& node_colours){
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

ParsedLineData parse_line(std::string line){
    ParsedLineData pld;
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char space = ' ';
    char eq = '=';
    std::getline(token_stream, token, space);
    if(token == "LOADING_RAMP"){
        pld.type = RAMP;
    }
    if(token == "WORKER"){
        pld.type = WORKER;
    }
    if(token == "STOREHOUSE"){
        pld.type = STOREHOUSE;
    }
    if(token == "LINK"){
        pld.type = LINK;
    }

    while (std::getline(token_stream, token, space)) {
        std::string key = token.substr(0, token.find(eq));
        std::string id = token.substr(token.find(eq) + 1, token.size()-1);
        pld.map.insert(std::make_pair(key,id));
    }
    return pld;
}

std::pair<std::string, std::string> parse_type(std::string line){
    std::pair<std::string, std::string> kid;
    std::istringstream token_stream(line);
    std::string token;

    std::getline(token_stream, token, '=');

    std::string key = token.substr(0, token.find('-'));
    std::string id = token.substr(token.find('-') + 1, token.size()-1);
    kid = std::make_pair(key, id);
    return kid;
}

Factory load_factory_structure(std::istream& is){
    Factory f;
    std::string line;
    while (std::getline(is, line)){
        if(line[0] == ';' || line.size() == 0){
            continue;
        }
        ParsedLineData el = parse_line(line);
        if(el.type == RAMP){
            ElementID id = std::stoi((*el.map.find("id")).second);
            TimeOffset l = static_cast<TimeOffset>(std::stoi((*el.map.find("delivery-interval")).second));
            f.add_ramp(Ramp(id,l));
        }
        else if (el.type == WORKER){
            ElementID id = std::stoi((*el.map.find("id")).second);
            TimeOffset l = static_cast<TimeOffset>(std::stoi((*el.map.find("processing-time")).second));
            PackageQueueType type;
            if((*el.map.find("queue-type")).second == "LIFO"){
                type = PackageQueueType::LIFO;
            }
            }
            if((*el.map.find("queue-type")).second == "FIFO"){
                type = PackageQueueType::FIFO;
            }

            std::unique_ptr<PackageQueue> q = std::make_unique<PackageQueue>(PackageQueue(type));
            f.add_worker(Worker(id,l,std::move(q)));
        }
        else if (el.type == STOREHOUSE){
            ElementID id = std::stoi((*el.map.find("id")).second);
            f.add_storehouse(Storehouse(id));
        }
        else if(el.type == LINK){
            std::pair<std::string, std::string> type_id_sender = parse_type((*el.map.find("src")).second);
            std::pair<std::string, std::string> type_id_receiver = parse_type((*el.map.find("dest")).second);
            if(type_id_sender.first == "ramp" && type_id_receiver.first == "store") {
                ElementID id_sender = std::stoi(type_id_sender.second);
                ElementID id_receiver = std::stoi(type_id_receiver.second);
                Storehouse& receiver = *(f.find_storehouse_by_id(id_receiver));
                f.find_ramp_by_id(id_sender)->receiver_preferences_.add_receiver(&receiver);
            }
            else if(type_id_sender.first == "ramp" && type_id_receiver.first == "worker") {
                ElementID id_sender = std::stoi(type_id_sender.second);
                ElementID id_receiver = std::stoi(type_id_receiver.second);
                Worker& receiver = *(f.find_worker_by_id(id_receiver));
                f.find_ramp_by_id(id_sender)->receiver_preferences_.add_receiver(&receiver);
            }
            else if(type_id_sender.first == "worker" && type_id_receiver.first == "worker") {
                ElementID id_sender = std::stoi(type_id_sender.second);
                ElementID id_receiver = std::stoi(type_id_receiver.second);
                Worker& receiver = *(f.find_worker_by_id(id_receiver));
                f.find_worker_by_id(id_sender)->receiver_preferences_.add_receiver(&receiver);
            }
            else if(type_id_sender.first == "worker" && type_id_receiver.first == "store") {
                ElementID id_sender = std::stoi(type_id_sender.second);
                ElementID id_receiver = std::stoi(type_id_receiver.second);
                Storehouse& receiver = *(f.find_storehouse_by_id(id_receiver));
                f.find_worker_by_id(id_sender)->receiver_preferences_.add_receiver(&receiver);
            }
        }

    }
    return f;
}

std::list<std::pair<std::string, std::string>> make_pairs_r(const Ramp& sender){
    std::list<std::pair<std::string, std::string>> paired;

    std::string sender_type = "ramp";

    std::string sender_id = std::to_string(sender.get_id());

    std::string sender_string = sender_type + "-" + sender_id;

    std::map<IPackageReceiver*, double> receiver_list = sender.receiver_preferences_.get_preferences();
    for(auto& receiver : receiver_list) {
        ReceiverType type = receiver.first->get_receiver_type();
        std::string receiver_type;

        if (type == ReceiverType::Worker) {
            receiver_type = "worker";
        } else {
            receiver_type = "store";
        }
        std::string receiver_id = std::to_string(receiver.first->get_id());

        std::string receiver_string = receiver_type + "-" + receiver_id;

        std::pair<std::string, std::string> pair = std::make_pair(sender_string, receiver_string);
        paired.insert(paired.end(), pair);
    }

    return paired;
}

std::list<std::pair<std::string, std::string>> make_pairs_w(const Worker& sender){
    std::list<std::pair<std::string, std::string>> paired;

    std::string sender_type = "worker";

    std::string sender_id = std::to_string(sender.get_id());

    std::string sender_string = sender_type + "-" + sender_id;

    std::map<IPackageReceiver*, double> receiver_list = sender.receiver_preferences_.get_preferences();
    for(auto& receiver : receiver_list) {
        ReceiverType type = receiver.first->get_receiver_type();
        std::string receiver_type;

        if (type == ReceiverType::Worker) {
            receiver_type = "worker";
        } else {
            receiver_type = "store";
        }
        std::string receiver_id = std::to_string(receiver.first->get_id());

        std::string receiver_string = receiver_type + "-" + receiver_id;

        std::pair<std::string, std::string> pair = std::make_pair(sender_string, receiver_string);
        paired.insert(paired.end(), pair);
    }

    return paired;
}

void save_factory_structure(Factory& factory, std::ostream& os){
    os << "; == LOADING RAMPS ==" << std::endl;
    os.put(os.widen('\n'));

    for(auto i = factory.ramp_cbegin(); i != factory.ramp_cend();i++){
        os << "LOADING_RAMP id=" << (*i).get_id() << " delivery-interval=" << (*i).get_delivery_interval() << "\n";
    }
    os << "; == WORKERS ==\n\n" << std::endl;
    for(auto i = factory.worker_cbegin(); i != factory.worker_cend();i++){
        std::string type;
        if ((*i).get_queue()->get_queue_type() == PackageQueueType::FIFO) {
            type = "FIFO";
        } else if ((*i).get_queue()->get_queue_type() == PackageQueueType::LIFO) {
            type = "LIFO";
        }
        os << "WORKER id=" << (*i).get_id() << " processing-time=" << (*i).get_processing_duration() << " queue-type=" << type << "\n";
    }
    os << "; == STOREHOUSES ==\n\n" << std::endl;
    for(auto i = factory.storehouse_cbegin(); i != factory.storehouse_cend();i++){
        os << "STOREHOUSE id=" << (*i).get_id() << "\n";
    }
    os << "; == LINKS ==\n\n" << std::endl;

    std::list<std::pair<std::string, std::string>> links;
    for (auto i = factory.ramp_cbegin(); i != factory.ramp_cend();i++) {
        links.merge(make_pairs_r(*i));
    }

    for (auto i = factory.worker_cbegin(); i != factory.worker_cend();i++) {
        links.merge(make_pairs_w(*i));
    }

    for(auto i = links.cbegin(); i != links.cend(); i++){
        os << "LINK src=" << (*i).first << " dest=" << (*i).second <<"\n";
    }
}
