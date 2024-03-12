#ifndef ROUTER_H
#define ROUTER_H

#include "base_classes.h"
#include <vector>
#include "geopoint.h"
#include <unordered_map>
#include "geotools.h"
#include <queue>

using namespace std;



class Router : public RouterBase
{

public:
	Router(const GeoDatabaseBase& geo_db);
	virtual ~Router();
	virtual vector<GeoPoint> route(const GeoPoint& pt1,
		const GeoPoint& pt2) const;
private: 
	

	const GeoDatabaseBase& gdbb;
	struct Node {
		GeoPoint point;  // Current GeoPoint
		double D;  // actual distance to goal
		GeoPoint* prev;

		double fScore;   // Estimated total cost from start to goal through this point
		bool operator>(const Node& other) const { return fScore > other.fScore; }
	};
	struct Neighbor {
		vector<GeoPoint> p;
		bool inNextPt = false;
	};
	struct GeoPointKey {
		GeoPoint point;

		GeoPointKey(const GeoPoint& pt) : point(pt) {}

		// Equality comparison based on GeoPoint's string representation
		bool operator==(const GeoPointKey& other) const {
			return point.to_string() == other.point.to_string();
		}
	};
	class Compare { // custon operatore for nodes for the priority queue
	public:
		bool operator()(Node below, Node above)
		{
				return below.fScore > above.fScore; // Assuming you want the smallest fScore to have highest priority
		}
	};
	struct GeoPointHash {
		size_t operator()(const GeoPointKey& pt) const {
			size_t h = hash<string>()(pt.point.to_string());
			return h ; // DO I NEED MODULO??
		}
	};
	double getScore(const unordered_map<GeoPointKey, double, GeoPointHash>& scoreMap, const GeoPointKey& point) const {
		auto it = scoreMap.find(point);
		if (it == scoreMap.end()) {
			return numeric_limits<double>::infinity();
		}
		else {
			return it->second;
		}
	}
	vector<GeoPoint> getPath(const unordered_map<GeoPointKey, GeoPoint, GeoPointHash>& cameFrom,const GeoPoint& start, const GeoPoint& end) const;

};
#endif