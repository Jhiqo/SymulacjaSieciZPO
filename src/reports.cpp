#include "reports.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

void generate_structure_report(const Factory& f, std::ostream& os){
    os<<"\n== LOADING RAMPS ==\n\n";

    for(auto it = f.ramp_cbegin(); it != f.ramp_cend(); it++){
        os<<"LOADING RAMP #"<<(*it).get_id()<<"\n";
        os<<"  Delivery interval: "<<(*it).get_delivery_interval()<<"\n";
        os<<"  Receivers:\n";

        for(auto receiver_preference : it->receiver_preferences_){
            os<<"    worker #"<<receiver_preference.first->get_id()<<"\n";
        }
        os<<"\n";
    }
    os<<"\n== WORKERS ==\n\n";

    for(auto it = f.worker_cbegin(); it != f.worker_cend(); it++){
        std::string queue_type = (*it).get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO";

        os << "WORKER #" << (*it).get_id() << "\n";
        os << "  Processing time: " << (*it).get_processing_duration() << "\n";
        os << "  Queue type: " << queue_type << "\n";
        os << "  Receivers:\n";

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

        for(auto receiver_worker : receivers_workers)
            os << "    worker #" << receiver_worker << "\n";

        for(auto receiver_storehouse : receivers_storehouses)
            os << "    storehouse #" << receiver_storehouse << "\n";

        os << "\n";
    }

    os<<"\n== STOREHOUSES ==\n";

    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++){
        os<<"\n"<<"STOREHOUSE #"<<(*it).get_id()<<"\n";
    }
    os<<"\n";
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){
    os << "=== [ Turn: "<<t<<" ] ===\n\n";

    os << "== WORKERS ==\n";

    for(auto it = f.worker_cbegin(); it != f.worker_cend(); it++){

        os<<"\nWORKER #"<<(*it).get_id()<<"\n";
        os<<"  PBuffer: ";
        if (!(*it).get_processing_buffer().has_value())
        {
            os<<"(empty)";
        }
        else
        {
            os<<"#"<<(*(*it).get_processing_buffer()).get_id()<<" (pt = "<<t - (*it).get_package_processing_start_time() + 1<<")";
        }
        os<<"\n";
        os<<"  Queue: ";
        auto el = (*it).get_queue()->cbegin();
        if(el == (*it).get_queue()->cend()){
            os<<"(empty)";
        }
        else
        {
            os<<"#"<<(*el).get_id();
            el++;
            while(el != (*it).get_queue()->cend()){
                os<<", #"<<(*el).get_id();
                el++;
            }
        }
        os<<"\n";
        os<<"  SBuffer: ";
        if (!(*it).get_sending_buffer().has_value()){
            os<<"(empty)";
        }
        else
        {
            os<<"#"<<(*(*it).get_sending_buffer()).get_id();
        }
        os<<"\n";
    }
    os<<"\n";

    os<<"\n== STOREHOUSES ==\n\n";
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++){
        os<<"STOREHOUSE #"<<(*it).get_id()<<"\n";
        os<<"  Stock: ";
        auto el = (*it).cbegin();
        if(el == (*it).cend()){
            os<<"(empty)";
        }
        else
        {
            os<<"#"<<(*el).get_id();
            el++;
            while(el != (*it).cend()){
                os<<", #"<<(*el).get_id();
                el++;
            }

        }
        os<<"\n\n";
    }
}
