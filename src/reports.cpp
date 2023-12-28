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
        std::string queue_type = (*it).get_queue()->get_queue_type() == PackageQueueType::LIFO ? "LIFO" : "FIFO";

        os<<"WORKER #"<<(*it).get_id()<<"\n";
        os<<"  Processing time: "<<(*it).get_processing_duration()<<"\n";
        os<<"  Queue type: "<<queue_type<<"\n";
        os<<"  Receivers:\n";


        for(auto receiver_preference : it->receiver_preferences_){
            ReceiverType type = receiver_preference.first->get_receiver_type();
            if(type == ReceiverType::Worker)
                os << "    worker #" << receiver_preference.first->get_id() << "\n";
            else
                os << "    storehouse #" << receiver_preference.first->get_id() << "\n";
        }


        os<<"\n";
    }

    os<<"\n== STOREHOUSES ==\n";

    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++){
        os<<"\n"<<"STOREHOUSE #"<<(*it).get_id()<<"\n";
    }
    os<<"\n";
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){
    os << "\n=== [ Turn: "<<t<<" ] ===\n";

    for(auto i = f.ramp_cbegin(); i != f.ramp_cend(); i++){
        os << "LOADING RAMP #" << (*i).get_id() << "\n";
        os << "  Delivery interval: " << (*i).get_delivery_interval() << "\n";
        os << "  Receivers:\n";

        for(auto receiver_preference : i->receiver_preferences_){
            os << "    worker #" << receiver_preference.first->get_id() << "\n";
        }
        os << "\n";
    }
}