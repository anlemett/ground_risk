#include "AStarSearchInstance.h"

// https://en.wikipedia.org/wiki/A*_search_algorithm

AStarSearchInstance::AStarSearchInstance(const RiskMap& risk_map, Coord<int> from, Coord<int> to,
    int search_limit, float r)
{
    this->risk_map = risk_map;
    this->from = from;
    this->to = to;
    this->search_limit = search_limit;
    this->r_m = r;
}

AStarSearchInstance::~AStarSearchInstance()
{
}

std::vector<Path> AStarSearchInstance::computeParetoApxPaths() {

    std::vector<Path> paths = {};
    
    Path path = runWithAlpha(0);
    paths.push_back(path);
    
    path = runWithAlpha(100000);
    paths.push_back(path);
    
    Path path0 = paths.at(0);
    Path path1 = paths.at(1);
       
    double beta = (double)(path1.risk - path0.risk)/(path1.length_m - path0.length_m);
        
    if (beta < - 0.0000001) {
            
        path = this->runWithAlpha(-1.0/beta);
        
        paths.push_back(path);
    }
    return paths;
}

double AStarSearchInstance::h (Coord<int> p) {
    double x_diff = (p.x - (this->to).x);
    double y_diff = (p.y - (this->to).y);
    double length_squared = pow(y_diff, 2) + pow(x_diff, 2);
    double length = sqrt(length_squared);
    return length;
}


Path AStarSearchInstance::runWithAlpha(double alpha) {
    std::cout << "Computing for alpha: " << alpha << std::endl;
    
    std::unordered_map<Coord<int>, double, hash_fn> labels; //point-g_score
    std::unordered_map<Coord<int>, Coord<int>, hash_fn> previous_nodes;
    
    priority_queue_with_remove<std::pair<Coord<int>, float>> open_set_pq;
    
    open_set_pq.push(std::make_pair(this->from, 0.0)); //point-f_score
           
    labels.insert({ this->from, 0.0 });

    previous_nodes.insert({ this->from, this->from });

    while (!open_set_pq.empty()) {

        Coord<int> current_node = open_set_pq.top().first;
        open_set_pq.pop();
        
        double current_label = labels.at(current_node);
        
        //std::cout << "current_node: " << current_node.x << " "<< current_node.y << " current_label" << current_label << "\n";
        
        if ((current_node.x == this->to.x) &&(current_node.y == this->to.y)) {
            std::cout << "break out of while loop\n";
            break;
        }

        Neighbours neighbours = this->risk_map.neighboursWithin(current_node, this->search_limit);

        for (auto neighbour : neighbours) {
                      
            double weight = this->risk_map.risk(current_node, neighbour, this->r_m) * alpha + 
                    this->risk_map.lengthM(current_node, neighbour);
                
            double new_label = current_label + weight; // new g_score
            
            double new_f_score = new_label + h(neighbour);
            
            auto got = labels.find(neighbour);
            
            if (got == labels.end()) {
                //key is not present
                labels.insert({ neighbour, new_label });
                previous_nodes.insert({ neighbour, current_node });
                open_set_pq.push(std::make_pair(neighbour, new_f_score));
            }
            else {
                // key is present
                if (got->second > new_label) { 
                    previous_nodes.at(neighbour) = current_node;
                    open_set_pq.remove(std::make_pair(neighbour, got->second));
                    got->second = new_label;
                    open_set_pq.push(std::make_pair(neighbour, new_f_score));
                }
            }
        }
    }
    
    return this->unwrapPath(previous_nodes, labels, alpha);
}


Path AStarSearchInstance::unwrapPath(const std::unordered_map<Coord<int>, Coord<int>, hash_fn>& nodes_previous,
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
