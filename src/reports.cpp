#include "reports.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

void generate_structure_report(const Factory& f, std::ostream& os){

    os<<"\n== LOADING RAMPS ==\n\n";

    std::vector<ElementID> ramps_id;

    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); it++)
        ramps_id.push_back((*it).get_id());

    std::sort(ramps_id.begin(), ramps_id.end());

    for(int ramp_i : ramps_id){

        auto it = f.find_ramp_by_id(ramp_i);

        os << "LOADING RAMP #" << it->get_id() << "\n";
        os << "  Delivery interval: " << it->get_delivery_interval() << "\n";
        os<<"  Receivers:\n";

        std::vector<ElementID> receivers_workers;

        for(auto receiver_preference : (*it).receiver_preferences_)
            receivers_workers.push_back(receiver_preference.first->get_id());

        std::sort(receivers_workers.begin(), receivers_workers.end());

        for(auto receiver_worker : receivers_workers)
            os<<"    worker #"<<receiver_worker<<"\n";

        os<<"\n";
    }

    ramps_id.clear();

    os<<"\n== WORKERS ==\n\n";

    std::vector<ElementID> workers_id;

    for (auto it = f.worker_cbegin(); it != f.worker_cend(); it++)
        workers_id.push_back((*it).get_id());

    std::sort(workers_id.begin(), workers_id.end());

    for(int worker_i : workers_id){
        auto it = f.find_worker_by_id(worker_i);

        std::string queue_type = (*it).get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO";

        os << "WORKER #" << (*it).get_id() << "\n";
        os << "  Processing time: " << (*it).get_processing_duration() << "\n";
        os<<"  Queue type: "<<queue_type<<"\n";
        os<<"  Receivers:\n";

        std::vector<ElementID> receivers_workers;
        std::vector<ElementID> receivers_storehouses;

        for(auto receiver_preference : (*it).receiver_preferences_){
            ReceiverType type = receiver_preference.first->get_receiver_type();

            if(type == ReceiverType::Worker)
                receivers_workers.push_back(receiver_preference.first->get_id());
            else
                receivers_storehouses.push_back(receiver_preference.first->get_id());
        }

        std::sort(receivers_workers.begin(), receivers_workers.end());
        std::sort(receivers_storehouses.begin(), receivers_storehouses.end());

        for(auto receiver_storehouse : receivers_storehouses)
            os<<"    storehouse #"<<receiver_storehouse<<"\n";

        for(auto receiver_worker : receivers_workers)
            os<<"    worker #"<<receiver_worker<<"\n";

        os<<"\n";
    }

    workers_id.clear();

    os<<"\n== STOREHOUSES ==\n";

    std::vector<ElementID> storehouses_id;

    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++)
        storehouses_id.push_back((*it).get_id());

    std::sort(storehouses_id.begin(), storehouses_id.end());

    for(int storehouse_i : storehouses_id){
        auto it = f.find_storehouse_by_id(storehouse_i);
        os<<"\n"<<"STOREHOUSE #"<<(*it).get_id()<<"\n";
    }
    os<<"\n";

    storehouses_id.clear();
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){
    os<<"=== [ Turn: "<<t<<" ] ===\n";

    os<<"\n== WORKERS ==\n";

    std::vector<ElementID> workers_id;
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); it++)
        workers_id.push_back((*it).get_id());
    std::sort(workers_id.begin(), workers_id.end());

    for(int worker_i : workers_id){
        auto it = f.find_worker_by_id(worker_i);
        os<<"\nWORKER #"<<(*it).get_id()<<"\n";


        if(!(*it).get_processing_buffer().has_value())
            os<<"  PBuffer: (empty)\n";
        else
            os<<"  PBuffer: #"<<(*it).get_processing_buffer()->get_id()<<" (pt = "<<(*it).get_package_processing_start_time()<<")\n";


        if((*it).get_queue()->cbegin() == (*it).get_queue()->cend())
            os<<"  Queue: (empty)\n";
        else{
            os<<"  Queue: #"<<(*it).get_queue()->cbegin()->get_id();
            for(auto queue_i = ++((*it).get_queue()->cbegin()); queue_i != (*it).get_queue()->end(); queue_i++){
                os<<", #"<<queue_i->get_id();
            }
            os<<"\n";
        }

        if(!(*it).get_sending_buffer().has_value())
            os<<"  SBuffer: (empty)\n";
        else
            os<<"  SBuffer: #"<<(*it).get_sending_buffer()->get_id()<<"\n";
    }

    workers_id.clear();

    os<<"\n\n== STOREHOUSES ==\n\n";
    std::vector<ElementID> storehouses_id;

    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++)
        storehouses_id.push_back((*it).get_id());

    std::sort(storehouses_id.begin(), storehouses_id.end());

    for(int storehouse_i : storehouses_id){
        auto it = f.find_storehouse_by_id(storehouse_i);
        os<<"STOREHOUSE #"<<(*it).get_id()<<"\n";

        if((*it).cbegin() == (*it).cend())
            os<<"  Stock: (empty)\n\n";
        else{
            os<<"  Stock: #"<<(*it).cbegin()->get_id();
            for(auto stock_i = ++((*it).cbegin()); stock_i != (*it).cend(); stock_i++){
                os<<", #"<<stock_i->get_id();
            }
            os<<"\n\n";
        }
    }

    storehouses_id.clear();
}