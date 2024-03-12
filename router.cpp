#include "router.h"
#include <limits>


Router::Router(const GeoDatabaseBase& geo_db): gdbb(geo_db){
}
Router::~Router() {
}
vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
	priority_queue<Node, vector<Node>, Compare> nextPts; // min heap
	unordered_map<GeoPointKey, GeoPoint, GeoPointHash> cameFrom;
	unordered_map<GeoPointKey, double, GeoPointHash> gScore; // actual length
	unordered_map<GeoPointKey, double, GeoPointHash> fScore; // actual of known path + hueristic
	gScore[pt1] = 0;
	fScore[pt1] = distance_earth_km(pt1, pt2);
	nextPts.push({pt1, distance_earth_km(pt1, pt2)});

	while (!nextPts.empty()) {
		GeoPoint current = nextPts.top().point;
		nextPts.pop();
		// if current is the end
		if (current.to_string() == pt2.to_string()) {
			return getPath(cameFrom, pt1, pt2);
		}

		vector<GeoPoint> neighbors = gdbb.get_connected_points(current); // gets neighbors
		//for (const auto& neighbor : neighbors) {
		//	GeoPointKey neighborKey(neighbor);
		//	
		//	double tentative_gScore = getScore(gScore, current) + distance_earth_km(current, neighbor);
		//	// Only update if the neighbor's current gScore is not set or if the new score is better
		//	if (tentative_gScore < getScore(gScore, neighborKey)) {
		//		cameFrom[neighborKey] = current;
		//
		//		gScore[neighborKey] = tentative_gScore;
		//		fScore[neighborKey] = tentative_gScore + distance_earth_km(neighbor, pt2); // Ensure heuristic is defined correctly
		//		// If neighbor not in openSet, add it. Since priority_queue doesn't support find, manage this separately if needed.
		//		if (fScore.find(neighbor) == fScore.end() ) {
		//							fScore[neighbor] = newFScore;
		//							nextPts.push({ neighbor, newFScore });
		//						} 
		//		nextPts.push(Node{ neighbor, fScore[neighborKey] });
		//	}
		//}
		for (const auto& neighbor : neighbors) {
			if (gScore.find(neighbor) == gScore.end()) { // if neighnor not ini map yet then set the g and f score to infinity
				gScore[neighbor] = INFINITY;
			}

			if (fScore.find(neighbor) == fScore.end()) {
				fScore[neighbor] = INFINITY;
			}
			
			double NewGScore = gScore[current] + distance_earth_km(current, neighbor);

			if ( NewGScore < gScore[neighbor]) { // lower score than neighbors
				cameFrom[neighbor] = current;
				gScore[neighbor] = NewGScore;
				double newFScore= NewGScore + distance_earth_km(neighbor, pt2);
					if (newFScore < fScore[neighbor]) {
						fScore[neighbor] = newFScore;
				         Node temp = Node();
						temp.point = neighbor;
						temp.fScore = newFScore;
						nextPts.push(temp);
					} 
			}
		}
	}
	return {};
}

vector<GeoPoint> Router::getPath(const unordered_map<GeoPointKey, GeoPoint, GeoPointHash>& cameFrom, const GeoPoint& start, const GeoPoint& end) const {
	vector<GeoPoint> path;
	GeoPoint current = end;
	while (current.to_string() != start.to_string()) { 
		path.insert(path.begin(), current);
		current = cameFrom.at(current);
	}
	path.insert(path.begin(), start);
	return path;
}