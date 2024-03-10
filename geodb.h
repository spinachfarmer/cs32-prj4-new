#ifndef GEODB_H
#define GEODB_H

#include "base_classes.h"
#include "HashMap.h"
#include <list>
using namespace std;

class GeoDatabase : public GeoDatabaseBase
{
private:
	struct Poi {
		GeoPoint location;
		string streetName;
		string name;
	};

	struct StreetSegment {
		GeoPoint start, end;
		string streetName;
		vector<string> pointsOfInterest;
	};

	HashMap<Poi> Pois;
	HashMap<StreetSegment> StreetSegments;
	HashMap<vector<GeoPoint>> connections;
	
public:
	GeoDatabase();

	virtual ~GeoDatabase();
	virtual bool load(const std::string& map_data_file);
	virtual bool get_poi_location(const std::string& poi,
		GeoPoint& point) const;
	virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
		const;
	virtual std::string get_street_name(const GeoPoint& pt1,
		const GeoPoint& pt2) const;
};

#endif