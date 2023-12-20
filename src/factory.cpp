#include "factory.hpp"

template<class Node>
void NodeCollection<Node>::remove_by_id(ElementID id) {
    auto it = find_by_id(id);
    if (it != nodes_.end()){
        nodes_.erase(it);
    }
}
