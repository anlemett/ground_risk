#include "NeighboursIter.h"
#include "RiskMap.h"

// https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
NeighboursIter::NeighboursIter(RiskMap* map, int search_limit, std::vector<int> p)
{
    int width = map->width();
}

//    fn new(map: &'a RiskMap, search_limit: i16, p: Coord<i16>) -> Self {
//        let width= map.width() as i16;
//        let height = map.height() as i16;

//        let x_from = cmp::max(0, p.x - map.offset - search_limit);
//        let x_to = cmp::min(width - 1, p.x - map.offset + search_limit);
//        let y_from = cmp::max(0, p.y - map.offset - search_limit);
//        let y_to = cmp::min(height - 1, p.y - map.offset + search_limit);

//        return NeighboursIter{
//            map: map,
//            search_limit,
//            p,
//            current_x: x_from,
//            current_y: y_from,
//            x_from: x_from,
//            x_to: x_to,
//            y_from: y_from,
//            y_to: y_to
//        }
//    }


NeighboursIter::~NeighboursIter()
{
}

