#include "BicriteriaDijkstraInstance.h"
#include "Neighbours.h"

BicriteriaDijkstraInstance::BicriteriaDijkstraInstance(RiskMap& risk_map, Coord<int> from, Coord<int> to,
    int search_limit, float r)
{
    (*this).risk_map = risk_map;
    (*this).from = from;
    (*this).to = to;
    (*this).search_limit = search_limit;
    (*this).r_m = r;
}

BicriteriaDijkstraInstance::~BicriteriaDijkstraInstance()
{
}


std::vector<Path> BicriteriaDijkstraInstance::computeParetoApxPaths() {

    std::vector<Path> paths = {};
    
    Path path = runWithAlpha(0);
    paths.push_back(path);
    
    path = runWithAlpha(100000);
    paths.push_back(path);
    
    // By default a max heap is created ordered by first element of pair
    std::priority_queue<std::pair<int, std::vector<int>>> intervals_queue;
    
    std::vector<int> interval = {0, 1};
    intervals_queue.push(make_pair(0, interval));
    
    while ((!intervals_queue.empty())&&(paths.size()<3)) {
        std::pair<int, std::vector<int>> pair = intervals_queue.top();
        intervals_queue.pop();
        interval = pair.second;
        
        Path path0 = paths.at(interval.at(0));
        Path path1 = paths.at(interval.at(1));
        
        float beta = (float)(path1.risk - path0.risk)/(path1.length_m - path0.length_m);
        
        if (beta < - 0.0000001) {
            
            Path new_path = this->runWithAlpha(-1.0/beta);
            if (interval.at(1) - interval.at(0) != 1) {
                std::cerr << "An error with intervals!" << std::endl;
                exit(1);
            }
            int index = interval.at(1);
            
            if (new_path.risk < path0.risk && new_path.length_m < path1.length_m) {
                auto itPos = paths.begin() + index;
                paths.insert(itPos, new_path);
                
                std::vector<int> new_interval = {interval.at(1)-1, interval.at(1)};
                intervals_queue.push(std::make_pair(interval.at(1), new_interval));
                intervals_queue.push(std::make_pair(interval.at(1)-1, new_interval));
            }
        }
    }
    return paths;
}


Path BicriteriaDijkstraInstance::runWithAlpha(float alpha) {
    std::cout << "Computing for alpha: " << alpha << std::endl;
    
    std::unordered_map<Coord<int>, float, hash_fn> labels;
    std::unordered_map<Coord<int>, Coord<int>, hash_fn> previous_nodes;
    
    struct comparator {
        bool operator()(
            std::pair<Coord<int>, float>& a,
            std::pair<Coord<int>, float>& b)
                {
                return a.second > b.second;
                }
        };
        
    std::priority_queue<std::pair<Coord<int>, float>, std::vector<std::pair<Coord<int>, float>>, comparator> pq;
    
    pq.push(std::make_pair(this->from, 0.0));
           
    labels.insert({ this->from, 0.0 });

    previous_nodes.insert({ this->from, this->from });

    while (!pq.empty()) {

        Coord<int> current_node = pq.top().first;
        pq.pop();
        
        //std::cout << "new current_node in dijkstra" << current_node.x << " " << current_node.y << "\n";
        
        float current_label = labels.at(current_node);

        if (current_node == this->to) {
            break;
        }

        Neighbours neigbours = this->risk_map.neighboursWithin(current_node, this->search_limit);
        
       for (auto neighbour : neigbours) {
           
            float weight = this->risk_map.risk(current_node, neighbour, this->r_m) * alpha + 
                this->risk_map.lengthM(current_node, neighbour);
                
            float new_label = current_label + weight;
            
            if (labels.find(neighbour) == labels.end()) {
                //key is not present
                labels.insert({ neighbour, new_label });
                previous_nodes.insert({ neighbour, current_node });
                pq.push(std::make_pair(neighbour, new_label));
            }
            else {
                // key is present
                float entry = labels.at(neighbour);
                if (entry > new_label) {
                    labels.at(neighbour) = new_label;
                    previous_nodes.at(neighbour) = current_node;
                    pq.push(std::make_pair(neighbour, new_label));
                }
            }
        }
    }
    
    return this->unwrapPath(previous_nodes, labels, alpha);
}


Path BicriteriaDijkstraInstance::unwrapPath(std::unordered_map<Coord<int>, Coord<int>, hash_fn> nodes_previous,
                                            std::unordered_map<Coord<int>, float, hash_fn> nodes_labels,
                                            float alpha) {
    std::cout << "unwrapPath\n";
    std::vector<Coord<int>> path;
    int total_risk = 0;
    float total_length = 0.0;

    Coord<int> previous_node = this->to;
        
    while (previous_node != this->from) {
        path.push_back(previous_node);
        Coord<int> new_previous_node = nodes_previous.at(previous_node);
        int new_risk = this->risk_map.risk(previous_node, new_previous_node, this->r_m);
        total_risk += new_risk;
        total_length += this->risk_map.lengthM(previous_node, new_previous_node);
        previous_node = new_previous_node;
    }

    path.push_back(this->from);
    total_risk += this->risk_map.risk(previous_node, this->from, this->r_m);
    total_length += this->risk_map.lengthM(previous_node, this->from);

    return ((struct Path) {path, nodes_labels.at(this->to), total_risk, total_length, alpha});
}
