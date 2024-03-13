#include "geodb.h"
#include "geopoint.h"
#include "geotools.h"


GeoDatabase::GeoDatabase() {
	
}
GeoDatabase::~GeoDatabase() {
	
}
bool GeoDatabase::load(const std::string& map_data_file) {
	ifstream file(map_data_file);
	if (!file.is_open()) {
		return false; // File could not be found or opened
	}
	string line;
	while (getline(file, line)) { // Parse the line to extract GeoPoints, street names, and POIs
		string streetName = line; // first line alwayss street name
		if (getline(file, line)) {
			istringstream segmentStream(line);
			string latS, longS, latE, longE;
			int poiCount;
			segmentStream >> latS >> longS >> latE >> longE;
			getline(file, line);
			poiCount = stoi(line);
			
			GeoPoint start(latS, longS), end(latE, longE);
			
			string segAddress = start.to_string() + "," + end.to_string(); // key for street seg map[
			
		
			if (poiCount > 0) {
				for (int i = 0; i < poiCount; i++) { // iterates through the diff poi
					// poi storage stuyff
					getline(file, line, '|');
					string poiName = line;
					getline(file, line, ' ');
					string poiLat = line;
					getline(file, line);
					string poiLong = line;
					GeoPoint pCoord(poiLat, poiLong);
					Poi poi;
					poi.location = pCoord;
					poi.streetName = streetName;
					Pois.insert(poiName, poi);

					// midpoint stuff
					GeoPoint mid = midpoint(start, end);
					StreetSegment AM, MB, MP;

					AM.start = start;
					AM.end = mid;
					AM.streetName = streetName;

					MB.start = mid;
					MB.end = end;
					MB.streetName = streetName;

					MP.start = mid;
					MP.end = pCoord;
					MP.streetName = "a path";
					segAddress = start.to_string() + "," + mid.to_string();
					StreetSegments.insert(segAddress, AM);
					segAddress = mid.to_string() + "," + end.to_string();
					StreetSegments.insert(segAddress, MB);
					segAddress = mid.to_string() + "," + pCoord.to_string();
					StreetSegments.insert(segAddress, MP);
					

					connections[start.to_string()].push_back(mid); 
					connections[mid.to_string()].push_back(start);
					connections[mid.to_string()].push_back(end);
					connections[end.to_string()].push_back(mid);
					connections[mid.to_string()].push_back(pCoord);
					connections[pCoord.to_string()].push_back(mid);
					connections[start.to_string()].push_back(end);
					connections[end.to_string()].push_back(start);
				}
			}
		
				StreetSegment temp;
				temp.start = start;
				temp.end = end;
				temp.streetName = streetName;
				segAddress = start.to_string() + "," + end.to_string();
				StreetSegments.insert(segAddress, temp);    // puts streeg seg into hashmap
				connections[start.to_string()].push_back(end);
				connections[end.to_string()].push_back(start);
			
		}
	}

	file.close();
	return true;
}


bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const {
	const Poi* poiPointer = Pois.find(poi);
	if (poiPointer != nullptr) {
		point = poiPointer->location;
		return true;
	}
	else {
		return false;
	}
}
vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
	vector<GeoPoint> connectedPoints;
	string key = pt.to_string();

	const vector<GeoPoint>* points = connections.find(key);
	if (points != nullptr) {
		connectedPoints = *points;
	}
	return connectedPoints;
}
string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const {

	string segAddress = pt1.to_string() +  "," + pt2.to_string();
	const StreetSegment* ssPointer = StreetSegments.find(segAddress);
	string segAddressB = pt2.to_string() + "," + pt1.to_string();
	if (ssPointer != nullptr) {
		return ssPointer->streetName;
	}
	else {
		const StreetSegment* ssPointerB = StreetSegments.find(segAddressB);
		if(ssPointerB != nullptr)
			return ssPointerB->streetName;
		return "";
	}
}