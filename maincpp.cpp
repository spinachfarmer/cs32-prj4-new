//#include "HashMap.h"
//#include "geodb.h"
//#include "router.h"
//
//#include  "stops.h"
//#include <cassert>
//
//#if defined(_MSC_VER)  &&  !defined(_DEBUG)
//#include <iostream>
//#include <windows.h>
//#include <conio.h>
//
//struct KeepWindowOpenUntilDismissed
//{
//    ~KeepWindowOpenUntilDismissed()
//    {
//        DWORD pids[1];
//        if (GetConsoleProcessList(pids, 1) == 1)
//        {
//            std::cout << "Press any key to close this window . . . ";
//            _getch();
//        }
//    }
//} keepWindowOpenUntilDismissed;
//#endif
//int main() {
//
//    // Define a hashmap that maps strings to doubles and has a maximum
//        // load factor of 0.3. It will initially have 10 buckets when empty.
//    HashMap<double> nameToGPA(0.3);
//    // Add new items to the hashmap. Inserting the third item will cause
//    // the hashmap to increase the number of buckets (since the maximum
//    // load factor is 0.3), forcing a rehash of all items.
//    nameToGPA.insert("Carey", 3.5); // Carey has a 3.5 GPA
//    nameToGPA.insert("David", 2.99); // David needs to up his game
//
//    // you can also use brackets like C++'s unordered_map!
//    nameToGPA["Annie"] = 3.85; // Adds Annie, who has the highest GPA of all
//
//    double* davidsGPA = nameToGPA.find("David");
//    if (davidsGPA != nullptr)
//        *davidsGPA = 3.1; // after a re-grade of David's exam, update 2.99 -> 3.1
//    nameToGPA.insert("Carey", 4.0); // Carey deserves a 4.0
//    // sees if linda is in the map; if not, creates a new entry for linda in map
//     cout << nameToGPA["Linda"]; // prints zero
//
//    HashMap<int> map;
//    map.insert("TestKey1", 100);
//    map.insert("TestKey2", 200);
//
//    const int* val1 = map.find("TestKey1");
//    const int* val2 = map.find("TestKey2");
//
//    assert(val1 && *val1 == 100);
//    assert(val2 && *val2 == 200);
//
//    map.insert("DuplicateKey", 300);
//    map.insert("DuplicateKey", 400); // Reinsert with new value
//
//    const int* valDuplicate = map.find("DuplicateKey");
//    assert(valDuplicate && *valDuplicate == 400);
//
//    const int* valNonExistent = map.find("NonExistentKey");
//    assert(valNonExistent == nullptr);
//
//    // Assuming your map automatically rehashes after a certain load factor or number of elements
//    for (int i = 0; i < 100; ++i) { // Insert more items than the initial bucket size to force rehash
//        map.insert("Key" + std::to_string(i), i);
//    }
//
//    // Check a few random elements to ensure they're still retrievable
//    const int* rehashedVal1 = map.find("Key10");
//    const int* rehashedVal2 = map.find("Key99");
//
//    assert(rehashedVal1 && *rehashedVal1 == 10);
//    assert(rehashedVal2 && *rehashedVal2 == 99);
//
//    //map.remove("TestKey1");
//    /*const int* valDeleted = map.find("TestKey1");
//    assert(valDeleted == nullptr);*/
//
//  
//     // tests getpoi location
//    GeoDatabase g;
//    g.load("mapdata.txt"); // assume this works to avoid error checking
//    cout << "sdf";
//    GeoPoint p;
//    if (g.get_poi_location("Diddy Riese", p))
//        cout << "The PoI is at " << p.sLatitude << ", "
//        << p.sLongitude << endl;
//    else
//        cout << "PoI not found!\n";
//    // tests get connected points
//
//    std::vector<GeoPoint> pts = g.get_connected_points( GeoPoint("34.0871665", "-118.4288835"));
//   /* if (pts.empty())
//        cout << "There are no points connected to your specified point\n";
//    else {
//        for (const auto p : pts)
//            cout << p.sLatitude << ", " << p.sLongitude << endl;
//    }*/
//    string a = "34.0547000";
//    string b = "-118.4794734";
//        string c = "34.0544590";
//        string d = "-118.4801137";
//    GeoPoint p1(a, b), p2(c , d);
//
//    cout << g.get_street_name(p1, p2 ) << endl;
//
//     GeoDatabaseBase* geoDb = new GeoDatabase(); // Or a mock version for testing
//
//
//     Stops stop;
//     stop.load("stop.txt");
//
//
//    // Populate your geoDb with data here, or ensure your mock returns appropriate values
//
//    // Create a Router instance with your geoDb
//    Router router(g);
//
//    // Define start and end points for your test
//    GeoPoint start("34.0626647", "-118.4472813"); 
//    GeoPoint end("34.0614911", "-118.4464410"); 
//
//    // Use the router to calculate a route
//    auto path = router.route(end, start);
//
//    // Output the results
//    if (path.empty()) {
//        std::cout << "No path found between the points." << std::endl;
//    }
//    else {
//        std::cout << "Path found:" << std::endl;
//        for (const auto& point : path) {
//            std::cout << point.to_string() << "," << std::endl;
//        }
//    }
//
//    // Clean up if necessary
//    delete geoDb;
//   
//    cout << "pasased all tests";
//}
//

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "geodb.h"
#include "router.h"
#include "stops.h"
#include "tourcmd.h"
#include "tour_generator.h"

using namespace std;

void print_tour(vector<TourCommand>& tcs)
{
    double total_dist = 0;
    std::string direction;
    double street_distance = 0;

    cout << "Starting tour...\n";

    for (size_t i = 0; i < tcs.size(); ++i)
    {
        if (tcs[i].get_command_type() == TourCommand::commentary)
        {
            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
            cout << tcs[i].get_commentary() << "\n";
        }
        else if (tcs[i].get_command_type() == TourCommand::turn)
        {
            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
        }
        else if (tcs[i].get_command_type() == TourCommand::proceed)
        {
            total_dist += tcs[i].get_distance();
            if (direction.empty())
                direction = tcs[i].get_direction();
            street_distance += tcs[i].get_distance();
            if (i + 1 < tcs.size() && tcs[i + 1].get_command_type() == TourCommand::proceed
                && tcs[i + 1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
            {
                continue;
            }

            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
            street_distance = 0;
            direction.clear();
        }
    }

    cout << "Your tour has finished!\n";
    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
}

int main(int argc, char* argv[])
{
    /*
    if (argc != 3)
    {
        cout << "usage: BruinTour mapdata.txt stops.txt\n";
        return 1;
    }*/

    GeoDatabase geodb;
    if (!geodb.load("mapdata.txt"))
    {
        cout << "Unable to load map data: " << argv[1] << endl;
        return 1;
    }

    Router router(geodb);
    TourGenerator tg(geodb, router);
    GeoPoint end("34.0705851", "-118.4439429");
    GeoPoint start("34.0614911", "-118.4464410");
    GeoPoint testM("34.0628867", "-118.4470365");
    string segAddressB = end.to_string() + "," + testM.to_string();
    auto path = router.route(end, start);

    // Output the results
    if (path.empty()) {
        std::cout << "No path found between the points." << std::endl;
    }
    else {
        std::cout << "Path found:" << std::endl;
        for (const auto& point : path) {
            std::cout << point.to_string() << "," << std::endl;
        }
    }

    Stops stops;
    if (!stops.load("stops.txt"))
    {
        cout << "Unable to load tour data: " << argv[2] << endl;
        return 1;
    }

    std::cout << "Routing...\n\n";

    vector<TourCommand> tcs = tg.generate_tour(stops);
    if (tcs.empty())
        cout << "Unable to generate tour!\n";
    else
        print_tour(tcs);
}

