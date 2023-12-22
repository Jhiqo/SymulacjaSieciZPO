#ifndef SYMULACJASIECIZPO_FACTORY_HPP
#define SYMULACJASIECIZPO_FACTORY_HPP

#include "types.hpp"
#include "nodes.hpp"
#include <vector>
#include <istream>
#include <stdexcept>
#include <string>
#include <algorithm>

template <class Node>
class NodeCollection{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add_node(Node&& node) {nodes_.push_back(std::move(node));}
    void remove_by_id(ElementID id);
    NodeCollection<Node>::iterator find_by_id(ElementID id) {return std::find_if(nodes_.begin(), nodes_.end(), [id](Node& elem) {return elem.get_id()==id;});}
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {return std::find_if(nodes_.cbegin(), nodes_.cend(), [id] (const Node& elem) {return elem.get_id()==id;});}

    iterator begin() { return nodes_.begin(); }
    iterator end() { return nodes_.end(); }
    const_iterator begin() const {return nodes_.begin();}
    const_iterator end() const {return nodes_.end();}
    const_iterator cbegin() const {return nodes_.cbegin();}
    const_iterator cend() const {return nodes_.cend();}
private:
    container_t nodes_;
};


enum ElementType { RAMP, WORKER, STOREHOUSE, LINK };

class Factory{
public:
    //Ramp
    void add_ramp(Ramp&& r){ramp_.add_node(std::move(r));};
    void remove_ramp(ElementID id){ramp_.remove_by_id((id));};
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id){return ramp_.find_by_id((id));};
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramp_.find_by_id((id));};
    NodeCollection<Ramp>::const_iterator ramp_cbegin() {return ramp_.cbegin();};
    NodeCollection<Ramp>::const_iterator ramp_cend() {return ramp_.cend();};
    //Worker
    void add_worker(Worker&& w){worker_.add_node(std::move(w));};
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){return worker_.find_by_id((id));};
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return worker_.find_by_id((id));};
    NodeCollection<Worker>::const_iterator worker_cbegin() {return worker_.cbegin();};
    NodeCollection<Worker>::const_iterator worker_cend() {return worker_.cend();};
    //Storehouse
    void add_storehouse(Storehouse&& s){storehouse_.add_node(std::move(s));};
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id){return storehouse_.find_by_id((id));};
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return storehouse_.find_by_id((id));};
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() {return storehouse_.cbegin();};
    NodeCollection<Storehouse>::const_iterator storehouse_cend() {return storehouse_.cend();};

    bool is_consistent();
    void do_delivery(Time t);
    void do_package_passing();
    void do_work(Time t);

private:
    NodeCollection<Ramp> ramp_;
    NodeCollection<Worker> worker_;
    NodeCollection<Storehouse> storehouse_;
    enum class NodeColour { UNVISITED, VISITED, VERIFIED };
 
    template <class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColour> &node_colours);
};

struct ParsedLineData
{
    ElementType type;
    std::map<std::string,std::string> map;
};

ParsedLineData parse_line(std::string line);
std::pair<std::string, std::string> parse_type(std::string line);

Factory load_factory_structure(std::istream& is);

std::list<std::pair<std::string, std::string>> make_pairs_r(const Ramp& sender);

std::list<std::pair<std::string, std::string>> make_pairs_w(const Worker& sender);

void save_factory_structure(Factory& factory, std::ostream& os);

#endif //SYMULACJASIECIZPO_FACTORY_HPP
