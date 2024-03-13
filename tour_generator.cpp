#include "tour_generator.h"
#include "geotools.h"

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) : m_geodb(geodb), m_router(router) {
	
}
string TourGenerator::getDirection(GeoPoint p1, GeoPoint p2) {
	double angle = angle_of_line(p1, p2);
	if (angle < 22.5 || angle >= 337.5) return "east";
	else if (angle < 67.5) return "northeast";
	else if (angle < 112.5) return "north";
	else if (angle < 157.5) return "northwest";
	else if (angle < 202.5) return "west";
	else if (angle < 247.5) return "southwest";
	else if (angle < 292.5) return "south";
	else if (angle < 337.5) return "southeast";
	return "unknown";
}
vector<TourCommand> TourGenerator::generate_tour(Stops& stops) {

	//m_router.route();
	vector<TourCommand> result;
	for (int i = 0; i < stops.size(); i++) { // goes through all the stops
		string poi, talkingPoints;
		if (!stops.get_poi_data(i, poi, talkingPoints)) { // getpoi data fails
			return {};
		}

		TourCommand commetary;
		commetary.init_commentary(poi, talkingPoints);
		result.push_back(commetary);
		if (i < stops.size() - 1) {
			string nextPoi, nextTalkingPoint;
			stops.get_poi_data(i + 1, nextPoi, nextTalkingPoint);
			GeoPoint currentP, nextP;
			if (!m_geodb.get_poi_location(poi, currentP) || !m_geodb.get_poi_location(nextPoi, nextP)) {
				// Handle unknown point of interest location
				return {};
			}
			vector<GeoPoint> path = m_router.route(currentP, nextP);
			if (path.empty()) {
				return {};
			}

			for (int j = 0; j < path.size() - 1; j++) {
				GeoPoint p1 = path[j];
				GeoPoint p2 = path[j + 1];

				double  distance = distance_earth_miles(p1, p2);
				string direction = getDirection(p1, p2);
				string streetName = m_geodb.get_street_name(p1, p2);
				
				TourCommand proceed;
				proceed.init_proceed(direction, streetName, distance, p1, p2);
				result.push_back(proceed);

				if (j < path.size() - 2) {
					GeoPoint p3 = path[j + 2];
					double angle = angle_of_turn(p1, p2, p3);
					if (m_geodb.get_street_name(p2, p3) != streetName && angle > 1 && angle <= 359) { // streetname differs
						string turnD;

						if (angle >= 180 && angle <= 360) {
							turnD = "right";
						}
						else if( angle >=1 && angle < 180){
							turnD = "left";
						}
						if (getDirection(path[j], path[j + 1]) == getDirection(path[j + 1], path[j + 2]))
							continue;
						streetName = m_geodb.get_street_name(p2, p3);
						TourCommand turn;
						turn.init_turn(turnD, streetName);
						result.push_back(turn);
					}
				}
			}
		}
		
	}
	
	return result;
}