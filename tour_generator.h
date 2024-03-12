#ifndef ROUTER_H
#define ROUTER_H

#include "base_classes.h"
#include <vector>
#include "tourcmd.h"
#include "stops.h"

using namespace std;

class TourGenerator
{
public:
	vector<TourCommand> generate_tour(Stops& stops);
	TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
	~TourGenerator() {}
private:
	const RouterBase& m_router; // OK!
	const GeoDatabaseBase& m_geodb;
	string getDirection(GeoPoint p1, GeoPoint p2);


};

#endif
