#ifndef SYMULACJASIECIZPO_FACTORY_HPP
#define SYMULACJASIECIZPO_FACTORY_HPP

#include "types.hpp"
#include "nodes.hpp"
#include <vector>

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

    iterator begin() {return nodes_.begin();}
    iterator end() {return nodes_.end();}
    const_iterator cbegin() const {return nodes_.cbegin();}
    const_iterator cend() const {return nodes_.cend();}
private:
    container_t nodes_;
};

#endif //SYMULACJASIECIZPO_FACTORY_HPP
