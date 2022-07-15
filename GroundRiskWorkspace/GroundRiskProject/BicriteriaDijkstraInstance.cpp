#include "BicriteriaDijkstraInstance.h"
#include "NeighboursIter.h"

BicriteriaDijkstraInstance::BicriteriaDijkstraInstance(RiskMap& risk_map, Coord from, Coord to,
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
    //std::priority_queue<std::pair<std::vector<int>, int>> intervals_queue;
    std::priority_queue<std::pair<int, std::vector<int>>> intervals_queue;
    
    std::vector<int> interval = {0, 1};
    intervals_queue.push(make_pair(0, interval));
    
    while (!intervals_queue.empty()) {
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
                intervals_queue.push(make_pair(interval.at(1), new_interval));
                intervals_queue.push(make_pair(interval.at(1)-1, new_interval));
            }
        }
    }
    return paths;
}


Path BicriteriaDijkstraInstance::runWithAlpha(float alpha) {
    std::cout << "Computing for alpha: " << alpha << std::endl;
    
    std::unordered_map<Coord, float, hash_fn> labels;
    std::unordered_map<Coord, Coord, hash_fn> previous_nodes;
    
//        let mut pq: PriorityQueue<_, Reverse<OrderedFloat<f64>>, DefaultHashBuilder> = 
//          PriorityQueue::<_, Reverse<OrderedFloat<f64>>, DefaultHashBuilder>::with_default_hasher();

    struct comparator {
        bool operator()(
            std::pair<Coord, float>& a,
            std::pair<Coord, float>& b)
                {
                return a.second > b.second;
                }
        };
        
    std::priority_queue<std::pair<Coord, float>, std::vector<std::pair<Coord, float>>, comparator> pq;
    
    pq.push(std::make_pair(this->from, 0.0));
           
    labels.insert({ this->from, 0.0 });

    previous_nodes.insert({ this->from, this->from });

    while (!pq.empty()) {

        
        Coord current_node = pq.top().first;
        if (current_node == this->to) {
            //std::cout << "!!!current_node == this->to!!!\n";
        }
        pq.pop();
        
        //std::cout << "new current_node in dijkstra" << current_node.x << " " << current_node.y << "\n";
        
        float current_label = labels.at(current_node);

        if (current_node == this->to) {
            break;
        }

        Neighbours neigbours = this->risk_map.neighboursWithin(current_node, this->search_limit);
        
        //std::cout << "neighboursWithin from: " << neigbours.x_from << " " << neigbours.y_from << std::endl;
        //std::cout << "neighboursWithin to: " << neigbours.x_to << " " << neigbours.y_to << std::endl;
        
       for (auto neighbour : neigbours) {
           
           //std::cout << neighbour.x << " " << neighbour.y << std::endl;
            
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


Path BicriteriaDijkstraInstance::unwrapPath(std::unordered_map<Coord, Coord, hash_fn> nodes_previous,
                                            std::unordered_map<Coord, float, hash_fn> nodes_labels,
                                            float alpha) {
    std::cout << "unwrapPath\n";
    std::vector<Coord>  path;
    int total_risk = 0;
    float total_length = 0.0;

    Coord previous_node = this->to;
        
    while (previous_node != this->from) {
        path.push_back(previous_node);
        Coord new_previous_node = nodes_previous.at(previous_node);
        total_risk += this->risk_map.risk(previous_node, new_previous_node, this->r_m);
        total_length += this->risk_map.lengthM(previous_node, new_previous_node);
        previous_node = new_previous_node;
    }
    path.push_back(this->from);
    total_risk += this->risk_map.risk(previous_node, this->from, this->r_m);
    total_length += this->risk_map.lengthM(previous_node, this->from);

    return ((struct Path) {path, nodes_labels.at(this->to), total_risk, total_length, alpha});
}

//impl Display for Path {
//    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
//        write!(f, "linear_weight: {}, risk: {}, length: {}, path: {:?}", &self.linear_combination_weight, &self.risk, &self.length_m, &self.path)
//    }
//}