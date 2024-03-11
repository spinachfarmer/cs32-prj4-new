#include "HashMap.h"
#include "geodb.h"
#include "router.h"

#include <cassert>

#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
    ~KeepWindowOpenUntilDismissed()
    {
        DWORD pids[1];
        if (GetConsoleProcessList(pids, 1) == 1)
        {
            std::cout << "Press any key to close this window . . . ";
            _getch();
        }
    }
} keepWindowOpenUntilDismissed;
#endif
int main() {

    // Define a hashmap that maps strings to doubles and has a maximum
        // load factor of 0.3. It will initially have 10 buckets when empty.
    HashMap<double> nameToGPA(0.3);
    // Add new items to the hashmap. Inserting the third item will cause
    // the hashmap to increase the number of buckets (since the maximum
    // load factor is 0.3), forcing a rehash of all items.
    nameToGPA.insert("Carey", 3.5); // Carey has a 3.5 GPA
    nameToGPA.insert("David", 2.99); // David needs to up his game

    // you can also use brackets like C++'s unordered_map!
    nameToGPA["Annie"] = 3.85; // Adds Annie, who has the highest GPA of all

    double* davidsGPA = nameToGPA.find("David");
    if (davidsGPA != nullptr)
        *davidsGPA = 3.1; // after a re-grade of David's exam, update 2.99 -> 3.1
    nameToGPA.insert("Carey", 4.0); // Carey deserves a 4.0
    // sees if linda is in the map; if not, creates a new entry for linda in map
     cout << nameToGPA["Linda"]; // prints zero

    HashMap<int> map;
    map.insert("TestKey1", 100);
    map.insert("TestKey2", 200);

    const int* val1 = map.find("TestKey1");
    const int* val2 = map.find("TestKey2");

    assert(val1 && *val1 == 100);
    assert(val2 && *val2 == 200);

    map.insert("DuplicateKey", 300);
    map.insert("DuplicateKey", 400); // Reinsert with new value

    const int* valDuplicate = map.find("DuplicateKey");
    assert(valDuplicate && *valDuplicate == 400);

    const int* valNonExistent = map.find("NonExistentKey");
    assert(valNonExistent == nullptr);

    // Assuming your map automatically rehashes after a certain load factor or number of elements
    for (int i = 0; i < 100; ++i) { // Insert more items than the initial bucket size to force rehash
        map.insert("Key" + std::to_string(i), i);
    }

    // Check a few random elements to ensure they're still retrievable
    const int* rehashedVal1 = map.find("Key10");
    const int* rehashedVal2 = map.find("Key99");

    assert(rehashedVal1 && *rehashedVal1 == 10);
    assert(rehashedVal2 && *rehashedVal2 == 99);

    //map.remove("TestKey1");
    /*const int* valDeleted = map.find("TestKey1");
    assert(valDeleted == nullptr);*/

  
     // tests getpoi location
    GeoDatabase g;
    g.load("mapdata.txt"); // assume this works to avoid error checking
    cout << "sdf";
    GeoPoint p;
    if (g.get_poi_location("Diddy Riese", p))
        cout << "The PoI is at " << p.sLatitude << ", "
        << p.sLongitude << endl;
    else
        cout << "PoI not found!\n";
    // tests get connected points

    std::vector<GeoPoint> pts = g.get_connected_points( GeoPoint("34.0871665", "-118.4288835"));
   /* if (pts.empty())
        cout << "There are no points connected to your specified point\n";
    else {
        for (const auto p : pts)
            cout << p.sLatitude << ", " << p.sLongitude << endl;
    }*/
    string a = "34.0547000";
    string b = "-118.4794734";
        string c = "34.0544590";
        string d = "-118.4801137";
    GeoPoint p1(a, b), p2(c , d);

    cout << g.get_street_name(p1, p2 ) << endl;

     GeoDatabaseBase* geoDb = new GeoDatabase(); // Or a mock version for testing

    // Populate your geoDb with data here, or ensure your mock returns appropriate values

    // Create a Router instance with your geoDb
    Router router(g);

    // Define start and end points for your test
    GeoPoint start("34.0871665", "-118.4288835"); 
    GeoPoint end("34.0630614", "-118.4468781"); 

    // Use the router to calculate a route
    auto path = router.route(start, end);

    // Output the results
    if (path.empty()) {
        std::cout << "No path found between the points." << std::endl;
    }
    else {
        std::cout << "Path found:" << std::endl;
        for (const auto& point : path) {
            std::cout << point.to_string() << std::endl;
        }
    }

    // Clean up if necessary
    delete geoDb;
   
    cout << "pasased all tests";
}

