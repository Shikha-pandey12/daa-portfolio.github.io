
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
using namespace std;

// Disjoint Set Union (DSU) or Union-Find data structure
class DSU {
public:
    vector<int> parent, rank;

    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int u) {
        if (parent[u] != u)
            parent[u] = find(parent[u]);  // Path compression
        return parent[u];
    }

    void unite(int u, int v) {
        int root_u = find(u);
        int root_v = find(v);

        if (root_u != root_v) {
            // Union by rank
            if (rank[root_u] > rank[root_v]) {
                parent[root_v] = root_u;
            } else if (rank[root_u] < rank[root_v]) {
                parent[root_u] = root_v;
            } else {
                parent[root_v] = root_u;
                rank[root_u]++;
            }
        }
    }
};

// Edge structure to represent edges in the graph
struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Base class for protected areas (wetlands, wildlife habitats)
class ProtectedArea {
public:
    string name;
    string type;  // Wetland, Wildlife Habitat, Forest, etc.
    double size;  // Size in square kilometers
    double biodiversityValue;  // Represents biodiversity richness
    double ecoTourismPotential;  // Potential for eco-tourism
    double latitude, longitude;  // Geographical coordinates

    ProtectedArea(string n, string t, double s, double b, double e, double lat, double lon)
        : name(n), type(t), size(s), biodiversityValue(b), ecoTourismPotential(e), latitude(lat), longitude(lon) {}

    virtual void display() const {
        cout << name << " (" << type << ") - Size: " << size << " km², Biodiversity: "
             << biodiversityValue << ", Eco-Tourism Potential: " << ecoTourismPotential
             << ", Location: (" << latitude << ", " << longitude << ")\n";
    }

    // Distance calculation between two points on Earth using Haversine formula
    double distanceTo(double lat2, double lon2) const {
        const double R = 6371.0;  // Radius of Earth in km
        double lat1 = latitude;
        double lon1 = longitude;
        double dLat = (lat2 - lat1) * M_PI / 180.0;
        double dLon = (lon2 - lon1) * M_PI / 180.0;

        double a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) * sin(dLon / 2) * sin(dLon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));

        return R * c;  // Distance in kilometers
    }
};

// Derived class for Wetlands
class Wetland : public ProtectedArea {
public:
    Wetland(string name, double size, double biodiversityValue, double ecoTourismPotential, double lat, double lon)
        : ProtectedArea(name, "Wetland", size, biodiversityValue, ecoTourismPotential, lat, lon) {}

    void display() const override {
        ProtectedArea::display();
        cout << "Wetland Specialization: High biodiversity, water filtration role.\n";
    }
};

// Derived class for Wildlife Habitats
class WildlifeHabitat : public ProtectedArea {
public:
    WildlifeHabitat(string name, double size, double biodiversityValue, double ecoTourismPotential, double lat, double lon)
        : ProtectedArea(name, "Wildlife Habitat", size, biodiversityValue, ecoTourismPotential, lat, lon) {}

    void display() const override {
        ProtectedArea::display();
        cout << "Wildlife Habitat Specialization: Focused on species conservation.\n";
    }
};

// Derived class for Forests
class Forest : public ProtectedArea {
public:
    Forest(string name, double size, double biodiversityValue, double ecoTourismPotential, double lat, double lon)
        : ProtectedArea(name, "Forest", size, biodiversityValue, ecoTourismPotential, lat, lon) {}

    void display() const override {
        ProtectedArea::display();
        cout << "Forest Specialization: Carbon sequestration, habitat for diverse species.\n";
    }
};

// Animal class representing animals to be allocated
class Animal {
public:
    string species;
    string habitatType;  // Type of habitat needed (e.g., Wetland, Forest, Wildlife Habitat)
    int population;  // Number of animals

    Animal(string s, string h, int p) : species(s), habitatType(h), population(p) {}

    void display() const {
        cout << species << " (Required Habitat: " << habitatType << ", Population: " << population << ")\n";
    }
};

// Function to input protected areas
void inputProtectedAreas(vector<ProtectedArea*>& areas) {
    int n;
    cout << "Enter the number of protected areas: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "\nEnter details for protected area " << (i + 1) << ":\n";
        string name, type;
        double size, biodiversityValue, ecoTourismPotential, latitude, longitude;

        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Type (Wetland, Wildlife Habitat, Forest): ";
        cin >> type;
        cout << "Size (km²): ";
        cin >> size;
        cout << "Biodiversity Value: ";
        cin >> biodiversityValue;
        cout << "Eco-Tourism Potential: ";
        cin >> ecoTourismPotential;
        cout << "Latitude: ";
        cin >> latitude;
        cout << "Longitude: ";
        cin >> longitude;

        if (type == "Wetland") {
            areas.push_back(new Wetland(name, size, biodiversityValue, ecoTourismPotential, latitude, longitude));
        } else if (type == "Wildlife Habitat") {
            areas.push_back(new WildlifeHabitat(name, size, biodiversityValue, ecoTourismPotential, latitude, longitude));
        } else if (type == "Forest") {
            areas.push_back(new Forest(name, size, biodiversityValue, ecoTourismPotential, latitude, longitude));
        } else {
            cout << "Invalid type! Skipping this area.\n";
        }
    }
}

// Function to allocate areas to animals
void allocateAreasToAnimals(vector<ProtectedArea*>& areas, vector<Animal>& animals) {
    for (auto& animal : animals) {
        cout << "\nAllocating area for " << animal.species << ":\n";

        bool allocated = false;

        for (auto& area : areas) {
            if (area->type == animal.habitatType && area->size >= animal.population * 0.1) {
                // Simple allocation based on habitat type and size (assuming 0.1 km² per animal)
                cout << "Allocated " << animal.population << " " << animal.species
                     << " to " << area->name << " (" << area->type << ")\n";
                area->size -= animal.population * 0.1;  // Reduce the area size after allocation
                allocated = true;
                break;
            }
        }

        if (!allocated) {
            cout << "Could not allocate a suitable area for " << animal.species << ".\n";
        }
    }
}

// Function to handle government alert and find nearby areas
void handleGovernmentAlert(const vector<ProtectedArea*>& areas, double alertLat, double alertLon, double threshold) {
    cout << "\nGovernment Alert: Searching for protected areas nearby...\n";

    for (int i = 0; i < areas.size(); ++i) {
        double distance = areas[i]->distanceTo(alertLat, alertLon);

        if (distance <= threshold) {
            cout << "Nearby Area: " << areas[i]->name << " (Distance: " << distance << " km)\n";
            areas[i]->display();
        }
    }
}

int main() {
    vector<ProtectedArea*> areas;
    vector<Animal> animals;

    cout << "Welcome to the Kosi River Biodiversity Conservation Project!\n";

    // Input protected areas
    inputProtectedAreas(areas);

    // Input animals
    animals.push_back(Animal("Tiger", "Forest", 50));
    animals.push_back(Animal("Elephant", "Forest", 100));
    animals.push_back(Animal("Flamingo", "Wetland", 200));
    animals.push_back(Animal("Rhino", "Wildlife Habitat", 30));

    // Display animals
    cout << "\nAnimals needing allocation:\n";
    for (const auto& animal : animals) {
        animal.display();
    }

    // Allocate areas to animals
    allocateAreasToAnimals(areas, animals);

    // Handle government alert
    double alertLat = 26.3, alertLon = 87.3;
    double threshold = 50.0;  // Distance threshold in km
    handleGovernmentAlert(areas, alertLat, alertLon, threshold);

    // Clean up dynamic memory
    for (int i = 0; i < areas.size(); ++i) {
        delete areas[i];
    }

    return 0;
}
