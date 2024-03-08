#include <iostream>
#include <list>
#include <string>
using namespace std;

template <typename T>
class HashMap
{
private:
	int m_nbuckets;
	double maxLoadFactor;
	struct node {
		string key;
		T value;
	};
	vector<list<node>> hashTable; // vector of lists of nodes
	int hashFunction(string key);

public:
	HashMap(double max_load = 0.75);
	~HashMap(); // destructor; deletes all of the items in the hashmap
	int size() const {// return the number of associations in the hashmap
		return m_nbuckets;
	}
	void insert(const std::string& key, const T& value);
	T* find(const std::string& key);
	
	T& operator[](const std::string& key);
};

template <typename T>
int HashMap<T>::hashFunction(string key) {
		size_t h = hash<string>()(key);
		return h % m_nbuckets; // Modulo to fit into table size;
}

template <typename T>
HashMap<T>::HashMap(double max_load) {// constructor
	maxLoadFactor = max_load;
	m_nbuckets = 10;
	hashTable.resize(m_nbuckets);
}
// The insert method associates one item (key) with another (value).
// If no association currently exists with that key, this method inserts
// a new association into the hashmap with that key/value pair. If there is
// already an association with that key in the hashmap, then the item
// associated with that key is replaced by the second parameter (value).
// Thus, the hashmap must contain no duplicate keys.
template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value) {
	int hashValue = hashFunction(key);
	list<int>::iterator it;
	bool keyExists = false;
	for (it = hashTable[hashValue].begin(); it != hashTable[hashValue].end();) { // iterates through list
		if (hashTable[hashValue].key == key) { // key already exists inside of hastable
			hashTable[hashValue].value = value;
			keyExists = true;
			break;
		}
	}
	if(!keyExists) {  // if key not in map then add node to the list
		hashTable.push_back(key, value);
	}
}
// If no association exists with the given key, return nullptr; otherwise,
// return a pointer to the value associated with that key. This pointer can be
// used to examine that value or modify it directly within the map.
template <typename T>
T* HashMap<T>::find(const std::string& key) {
	int hashValue = hashFunction(key);
	for (it = hashTable[hashValue].begin(); it != hashTable[hashValue].end();) { // iterates through list
		if (hashTable[hashValue].key == key) { // key already exists inside of hastable
			return hashTable[hashValue].value;
			
		}
	}
	return nullptr;
}
// Defines the bracket operator for HashMap, so you can use your map like this:
	// your_map["david"] = 2.99;
	// If the key does not exist in the hashmap, this will create a new entry in
	// the hashmap and map it to the default value of type T. Then it will return a
	// reference to the newly created value in the map.
template <typename T>
T& HashMap<T>::operator[](const std::string& key) {

}