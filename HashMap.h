#include <iostream>
#include <list>
#include <vector>
#include <string>
using namespace std;

template <typename T>
class HashMap
{
private:
	int m_nbuckets;
	int nnodes;
	double maxLoadFactor;
	struct node {
		string key;
		T value;
		bool isOccupied = false; // for handeling remove
	};
	vector<list<node>> hashTable; // vector of lists of nodes
	int hashFunction(string key, int size) const {
		size_t h = hash<string>()(key);
		return h % size; // Modulo to fit into table size;
	}
	void remove(const string& key) {
		int hashValue = hashFunction(key, m_nbuckets); // Get the bucket index
		auto& bucket = hashTable[hashValue]; // Reference to the bucket (list of nodes)

		// Use iterators to find the node with the matching key
		for (auto it = bucket.begin(); it != bucket.end(); it++) {
			if (it->key == key) {
				bucket.erase(it); // Remove the node from the list
				return; // Exit after removing the node
			}
		}
		nnodes--;
	}
	void checkLoadFactor() {
		if (static_cast<double>(nnodes) / hashTable.size() > maxLoadFactor) {
			rehash(hashTable.size() * 2); // Double the size
		}
	}
	void rehash(int newSize) {
		vector<list<node>> newTable(newSize);

		typename vector<list<node>>::iterator list;
		for (const auto& bucket : hashTable) {
			// Iterate over each node within a bucket
			for (const auto& entry : bucket) {
				// Compute the new index based on the newSize
				int newIndex = hashFunction(entry.key, newSize);
				// Push the entry node to the new table at the new index
				newTable[newIndex].push_back(entry);
			}
		}

		hashTable.swap(newTable);
		m_nbuckets = newSize;
	}


public:
	HashMap(double max_load = 0.75) : maxLoadFactor(max_load), nnodes(0), m_nbuckets(10) {// constructor
		hashTable.resize(m_nbuckets);
	}

	~HashMap() {} // destructor; deletes all of the items in the hashmap
	int size() const {// return the number of associations in the hashmap
		return m_nbuckets;
	}


	void insert(const std::string& key, const T& value) {
		checkLoadFactor();
		int hashValue = hashFunction(key, m_nbuckets);
		typename list<node>::iterator it;
		bool keyExists = false;
		for (it = hashTable[hashValue].begin(); it != hashTable[hashValue].end(); it++) { // iterates through list
			if (it->key == key) { // key already exists inside of hastable
				it->value = value;
				keyExists = true;
				break;
			}
		}
		if (!keyExists) {  // if key not in map then add node to the list
			node newNode; // Create a node instance
			newNode.key = key;
			newNode.value = value;
			hashTable[hashValue].push_back(newNode); // Add the new node
		}
		nnodes++;
	}

	T& operator[](const std::string& key) {
		int hashValue = hashFunction(key, m_nbuckets);
		if (find(key) == nullptr) {
			node newNode; // Create a node instance
			newNode.key = key;
			newNode.value = T();
			hashTable[hashValue].push_back(newNode); // Add the new node
		}

		typename list<node>::iterator it;
		for (it = hashTable[hashValue].begin(); it != hashTable[hashValue].end(); it++) { // iterates through list
			if (it->key == key) { // key already exists inside of hastable
				return it->value;
			}
		}
		return hashTable[hashValue].back().value;
	}


	const T* find(const string& key) const {
		int hashValue = hashFunction(key, m_nbuckets);
		typename list<node>::const_iterator it;
		for (it = hashTable[hashValue].begin(); it != hashTable[hashValue].end(); it++) { // iterates through list
			if (it->key == key) { // key already exists inside of hastable
				return &(it->value);

			}
		}
		return nullptr;
	}
	// If no association exists with the given key, return nullptr; otherwise,
	// return a pointer to the value associated with that key. This pointer can be
	// used to examine that value or modify it directly within the map.
	T* find(const std::string& key) {
		const auto& hm = *this;
		return const_cast<T*>(hm.find(key));
	}
};
