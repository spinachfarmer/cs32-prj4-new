#include "HashMap.h"
#include <cassert>

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

    cout << "pasased all tests";
}

