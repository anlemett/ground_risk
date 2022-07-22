#include "BicriteriaDijkstraInstance.h"
#include "Neighbours.h"

BicriteriaDijkstraInstance::BicriteriaDijkstraInstance(const RiskMap& risk_map, Coord<int> from, Coord<int> to,
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
    
    struct comparator {
        bool operator()(
            const std::pair<std::vector<int>, int>& a,
            const std::pair<std::vector<int>, int>& b)
            {
                return a.second > b.second;
            }
        };
    
    
    std::priority_queue<std::pair<std::vector<int>,int>, std::vector<std::pair<std::vector<int>,int>>, comparator> intervals_queue;
    
    std::vector<int> interval = {0, 1};
    intervals_queue.push(make_pair(interval,0));
    
    while ((!intervals_queue.empty())&&(paths.size()<3)) {
        std::pair<std::vector<int>, int> pair = intervals_queue.top();
        intervals_queue.pop();
        interval = pair.first;
                
        Path path0 = paths.at(interval.at(0));
        Path path1 = paths.at(interval.at(1));
       
        double beta = (double)(path1.risk - path0.risk)/(path1.length_m - path0.length_m);
        
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
                intervals_queue.push(std::make_pair(new_interval, interval.at(1)));
                intervals_queue.push(std::make_pair(new_interval, interval.at(1)-1));
            }
        }
    }
    return paths;
}


Path BicriteriaDijkstraInstance::runWithAlpha(double alpha) {
    std::cout << "Computing for alpha: " << alpha << std::endl;
    
    std::unordered_map<Coord<int>, double, hash_fn> labels;
    std::unordered_map<Coord<int>, Coord<int>, hash_fn> previous_nodes;
    
    priority_queue_with_remove<std::pair<Coord<int>, float>> pq;
    
    pq.push(std::make_pair(this->from, 0.0));
           
    labels.insert({ this->from, 0.0 });

    previous_nodes.insert({ this->from, this->from });

    while (!pq.empty()) {

        Coord<int> current_node = pq.top().first;
        pq.pop();
        
        double current_label = labels.at(current_node);
        
        if ((current_node.x == this->to.x) &&(current_node.y == this->to.y)) {
            break;
        }

        Neighbours neigbours = this->risk_map.neighboursWithin(current_node, this->search_limit);
        
        for (auto neighbour : neigbours) {
                      
            double weight = this->risk_map.risk(current_node, neighbour, this->r_m) * alpha + 
                this->risk_map.lengthM(current_node, neighbour);
                
            double new_label = current_label + weight;
            
            if (labels.find(neighbour) == labels.end()) {
                //key is not present
                labels.insert({ neighbour, new_label });
                previous_nodes.insert({ neighbour, current_node });
                pq.push(std::make_pair(neighbour, new_label));
            }
            else {
                // key is present
                double entry = labels.at(neighbour);
                if (entry > new_label) { 
                    labels.at(neighbour) = new_label;
                    previous_nodes.at(neighbour) = current_node;
                    pq.remove(std::make_pair(neighbour, entry));
                    pq.push(std::make_pair(neighbour, new_label));
                }
            }
        }
    }
    
    return this->unwrapPath(previous_nodes, labels, alpha);
}


Path BicriteriaDijkstraInstance::unwrapPath(const std::unordered_map<Coord<int>, Coord<int>, hash_fn>& nodes_previous,
                                            const std::unordered_map<Coord<int>, double, hash_fn>& nodes_labels,
                                            double alpha) {
    std::cout << "unwrapPath\n";
    std::vector<Coord<int>> path;
    int total_risk = 0;
    float total_length = 0.0;

    Coord<int> previous_node = this->to;
        
    while (previous_node != this->from) {
        path.push_back(previous_node);
        Coord<int> new_previous_node = nodes_previous.at(previous_node);
        int new_risk = this->risk_map.risk(previous_node, new_previous_node, this->r_m);
        //std::cout << "new_risk: "<< new_risk<< "\n";
        total_risk += new_risk;
        total_length += this->risk_map.lengthM(previous_node, new_previous_node);
        previous_node = new_previous_node;
    }

    path.push_back(this->from);
    total_risk += this->risk_map.risk(previous_node, this->from, this->r_m);
    std::cout << "total_risk: "<< total_risk<< "\n";
    total_length += this->risk_map.lengthM(previous_node, this->from);
    std::cout << "total_length: "<< total_length<< "\n";

    return ((struct Path) {path, nodes_labels.at(this->to), total_risk, total_length, alpha});
}
