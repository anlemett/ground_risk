#include "BicriteriaDijkstraInstance.h"
#include "NeighboursIter.h"

BicriteriaDijkstraInstance::BicriteriaDijkstraInstance(RiskMap& risk_map, std::vector<int> from, std::vector<int> to,
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
    
    std::map<std::vector<int>, float> labels;
    std::map<std::vector<int>, std::vector<int>> previous_nodes;
    
//        let mut pq: PriorityQueue<_, Reverse<OrderedFloat<f64>>, DefaultHashBuilder> = 
//          PriorityQueue::<_, Reverse<OrderedFloat<f64>>, DefaultHashBuilder>::with_default_hasher();
    std::priority_queue<std::pair<float, std::vector<int>>> pq;

    pq.push(make_pair(0.0, this->from));
    previous_nodes.insert({ this->from, this->from });

    while (!pq.empty()) {

        std::vector<int> current_node = pq.top().second;
        pq.pop();

        float current_label = labels.at(current_node);

        if (current_node == this->to) {
            break;
        }
        
        NeighboursIter neigbours = this->risk_map.neighboursWithin(current_node, this->search_limit);
        
        for (auto neighbour : neigbours)
            std::cout << "test " << "\n";
        
        

//        for neighbour in self.risk_map.neighbours_within(current_node, self.search_limit) {
//                let weight = self.risk_map.risk(current_node, neighbour, self.r_m) as f64 * alpha + self.risk_map.length_m(current_node, neighbour);
//                let new_label = current_label + weight;

//                let mut entry = labels.entry(neighbour);

//                match entry {
//                    Occupied(mut entry) => {
//                        if entry.get() > &new_label {
//                            entry.insert(new_label);
//                            *previous_nodes.entry(neighbour).or_insert(current_node) = current_node;
//                            pq.push(neighbour, Reverse(OrderedFloat(new_label)));
//                        }
//                    },
//                    Vacant(entry) => {
//                        entry.insert(new_label);
//                        *previous_nodes.entry(neighbour).or_insert(current_node) = current_node;
//                        pq.push(neighbour, Reverse(OrderedFloat(new_label)));
//                    }
//                }
//            }
    }

//        return self.unwrap_path(&previous_nodes, &labels, alpha);
    Path temp; // remove
    return temp;
}


Path BicriteriaDijkstraInstance::unwrapPath(std::unordered_map<std::vector<int>, std::vector<int>> nodes_previous,
                                            std::unordered_map<std::vector<int>, int> nodes_labels,
                                            float alpha) {
//    fn unwrap_path(&self, nodes_previous: &HashMap<Coord<i16>, Coord<i16>>, nodes_labels: &HashMap<Coord<i16>, f64>, alpha: f64) -> Path {
//        let mut path = vec![];
//        let mut total_risk = 0;
//        let mut total_length = 0.0;

//        let mut previous_node = &self.to;

//        while previous_node != &self.from {
//            path.push(*previous_node);

//            let new_previous_node = nodes_previous.get(previous_node).unwrap();

//            total_risk += self.risk_map.risk(*previous_node, *new_previous_node, self.r_m);
//            total_length += self.risk_map.length_m(*previous_node, *new_previous_node);

//            previous_node = new_previous_node;
//        }

//        path.push(self.from);
//        total_risk += self.risk_map.risk(*previous_node, self.from, self.r_m);
//        total_length += self.risk_map.length_m(*previous_node, self.from);

//        return Path{
//            path,
//            linear_combination_weight: *nodes_labels.get(&self.to).unwrap(),
//            risk: total_risk,
//            length_m: total_length,
//            alpha: alpha
//        }
//    }
    Path temp; // remove
    return temp;
}

//impl Display for Path {
//    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
//        write!(f, "linear_weight: {}, risk: {}, length: {}, path: {:?}", &self.linear_combination_weight, &self.risk, &self.length_m, &self.path)
//    }
//}