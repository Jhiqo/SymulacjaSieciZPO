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
    os << "=== [ Turn: " << t << " ] ===\n" << std::endl;
    os << "== WORKERS ==\n" << std::endl;
    std::vector<Worker*> Workers;
    for (NodeCollection<Ramp>::const_iterator ramp = f.ramp_cbegin(); ramp != f.ramp_cend(); ramp++) {
        auto iter = (*ramp).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;}) != (*ramp).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;});
            Workers.push_back((Worker*)iter->first);
            iter++;
        }
    }
    for (NodeCollection<Worker>::const_iterator worker = f.worker_cbegin(); worker != f.worker_cend(); worker++) {
        auto iter = (*worker).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;}) != (*worker).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Worker;});
            Workers.push_back((Worker*)iter->first);
            iter++;
        }
    }
    std::sort(Workers.begin(),Workers.end(),[](Worker* a, Worker* b){return a->get_id() < b->get_id();});
    for(auto worker : Workers){
        os << "WORKER #" << worker->get_id() << std::endl;
        if(worker->get_processing_buffer().has_value()){
            os << "  PBuffer: #" << worker->get_processing_buffer()->get_id() << " (pt = " << worker->get_package_processing_start_time() << ")" << std::endl;
        }
        else{
            os << "  PBuffer: (empty)" << std::endl;
        }
        os << "  Queue: ";
        if (worker->begin() == worker->end()){
            os << "(empty)" << std::endl;
        }
        else{
            for(auto it = worker->begin(); it != worker->end(); it++){
                if(it == worker->begin()){
                    os << "#" << it->get_id();
                }
                else{
                    os << ", #" << it->get_id();
                }
                os << std::endl;
            }
        }
        //os << "  SBuffer: (empty)\n" << std::endl;
        if (worker->get_sending_buffer().has_value()) {
            os << "  SBuffer: #" << worker->get_sending_buffer()->get_id() << "\n" << std::endl;
        } else {
            os << "  SBuffer: (empty)\n" << std::endl;
        }


    }

    os << "\n== STOREHOUSES ==\n" << std::endl;
    std::vector<Storehouse*> Storehouses;
    for (NodeCollection<Ramp>::const_iterator ramp = f.ramp_cbegin(); ramp != f.ramp_cend(); ramp++) {
        auto iter = (*ramp).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;}) != (*ramp).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*ramp).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;});
            Storehouses.push_back((Storehouse*)iter->first);
            iter++;
        }
    }
    for (NodeCollection<Worker>::const_iterator worker = f.worker_cbegin(); worker != f.worker_cend(); worker++) {
        auto iter = (*worker).receiver_preferences_.get_preferences().begin();
        while(std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;}) != (*worker).receiver_preferences_.get_preferences().end()){
            iter = std::find_if(iter,(*worker).receiver_preferences_.get_preferences().end(),[](std::pair<IPackageReceiver*, double> i){return i.first->get_receiver_type() == ReceiverType::Storehouse;});
            Storehouses.push_back((Storehouse*)iter->first);
            iter++;
        }
    }
    std::sort(Storehouses.begin(),Storehouses.end(),[](Storehouse* a, Storehouse* b){return a->get_id() < b->get_id();});
    for(auto storehouse : Storehouses){
        os << "STOREHOUSE #" << storehouse->get_id() << std::endl;
        os << "  Stock: ";
        std::vector<ElementID> storehouse_ids2;
        if (storehouse->begin() == storehouse->end()){
            os << "(empty)" << std::endl;
        }
        else{
            for(auto it = storehouse->begin(); it != storehouse->end(); it++){
                storehouse_ids2.push_back(it->get_id());
            }
            std::sort(storehouse_ids2.begin(),storehouse_ids2.end());
            for(auto it = storehouse_ids2.begin(); it != storehouse_ids2.end();it++){
                if(it == storehouse_ids2.begin()){
                    os << "#" << *it;
                }
                else{
                    os << ", #" << *it;
                }
                os << std::endl;
            }
        }
    }
    os << std::endl;

}