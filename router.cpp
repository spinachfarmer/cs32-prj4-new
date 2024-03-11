#include "router.h"



Router::Router(const GeoDatabaseBase& geo_db): gdbb(geo_db){
}
Router::~Router() {
}
vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
	priority_queue<Node, vector<Node>, Compare> nextPts; // min heap
	unordered_map<GeoPointKey, GeoPoint, GeoPointHash> cameFrom;
	unordered_map<GeoPointKey, double, GeoPointHash> gScore;
	gScore[pt1] = 0;
	nextPts.push({pt1, distance_earth_km(pt1, pt2)});

	while (!nextPts.empty()) {
		GeoPoint current = nextPts.top().point;
		nextPts.pop();
		// if current is the end
		if (current.to_string() == pt2.to_string()) {
			return getPath(cameFrom, pt1, pt2);
		}

		vector<GeoPoint> neighbors = gdbb.get_connected_points(current); // gets neighbors
		/*if (neighbors.empty())
			cout << "There are no points connected to your specified point\n";
		else {
			for (const auto p : neighbors)
				cout << p.sLatitude << ", " << p.sLongitude << endl;
		}*/

		for (const auto& neighbor : neighbors) {
			double NewGScore = gScore[current] + distance_earth_km(current, neighbor);

			if (gScore.find(neighbor) == gScore.end() || NewGScore < gScore[neighbor]) { // lower score than neighbors
				cameFrom[neighbor] = current;
				gScore[neighbor] = NewGScore;
				nextPts.push({ neighbor, NewGScore + distance_earth_km(neighbor, pt2) }); // chekcs out better scoreere later
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