#include <iostream>  // Includes the standard library for reading input (cin) and printing output (cout).
#include <vector>    // Includes the library to use dynamic arrays (vectors) which can change size.
#include <string>    // Includes the library for working with text strings.
#include <climits>   // Includes definitions for integer limits (like INT_MAX), though less used here.
#include <iomanip>   // Includes tools for formatting output (e.g., column width, precision).
#include <queue>     // Includes the priority_queue container needed for Dijkstra's algorithm.
#include <cmath>     // Includes math functions like max() or sqrt().
#include <map>       // Includes the map container (key-value pairs), though unused in this specific logic.

using namespace std; // Allows using standard library names (like cout, vector) without the std:: prefix.

// ==========================================
//        CONFIGURATION CONSTANTS
// ==========================================
const double PRICE_PETROL = 280.0;  // Sets the global constant price for petrol.
const double PRICE_DIESEL = 295.0;  // Sets the global constant price for diesel (unused but defined).
const int MAX_CITIES = 20;          // Defines the maximum number of cities the system can handle.
const double INF = 1e9;             // Defines a very large number (1 billion) to represent infinity.

// ==========================================
//          DATA STRUCTURES
// ==========================================

// Enum to define traffic severity levels as named constants.
enum TrafficLevel {
    LOW,        // Represents clear roads (Index 0).
    MODERATE,   // Represents normal traffic (Index 1).
    HIGH,       // Represents rush hour traffic (Index 2).
    JAMMED      // Represents severe blockage (Index 3).
};

// Enum to define different types of roads.
enum RoadType {
    MOTORWAY,   // Represents high-speed motorways.
    HIGHWAY,    // Represents standard highways.
    LOCAL       // Represents slower local roads.
};

// Structure representing a single connection (road) between cities.
struct Edge {
    int destination;      // Stores the ID of the city this road leads to.
    double distanceKM;    // Stores the length of the road in kilometers.
    TrafficLevel traffic; // Stores the traffic condition on this road.
    RoadType type;        // Stores the type of road (Motorway, Highway, etc.).
    string roadName;      // Stores the name of the road (e.g., "M-2 Motorway").
};

// Structure used in the Priority Queue to order cities by travel time.
struct PqNode {
    int id;           // Stores the city ID.
    double timeCost;  // Stores the total time taken to reach this city.

    // Overloads the > operator so the Priority Queue works as a Min-Heap (smallest time on top).
    bool operator>(const PqNode& other) const {
        return timeCost > other.timeCost; // Returns true if current time is greater than the other.
    }
};

// ==========================================
//        CORE ROUTING CLASS
// ==========================================
class RoutePlanner {
private:
    // Adjacency List: An array of vectors where each index holds a list of roads from that city.
    vector<Edge> adj[MAX_CITIES];
    string cityNames[MAX_CITIES]; // Array to store the names of the cities based on their ID.
    int cityCount;                // Variable to keep track of how many cities have been added.

public:
    // Constructor to initialize the RoutePlanner object.
    RoutePlanner() {
        cityCount = 0;       // Starts the city count at 0.
        initializeMapData(); // Calls the function to load all hardcoded map data.
    }

    // Helper function: converts TrafficLevel enum to a numerical time multiplier.
    double getTrafficMultiplier(TrafficLevel level) {
        switch (level) {
            case LOW: return 1.0;       // No delay.
            case MODERATE: return 1.2;  // 20% slower.
            case HIGH: return 1.5;      // 50% slower.
            case JAMMED: return 2.5;    // 150% slower (2.5x time).
            default: return 1.0;        // Default fallback is no delay.
        }
    }

    // Helper function: converts TrafficLevel enum to a readable string.
    string getTrafficString(TrafficLevel level) {
        switch (level) {
            case LOW: return "Clear";       // Returns "Clear" text.
            case MODERATE: return "Moderate"; // Returns "Moderate" text.
            case HIGH: return "Heavy";        // Returns "Heavy" text.
            case JAMMED: return "Jammed";     // Returns "Jammed" text.
            default: return "Unknown";      // Default fallback text.
        }
    }

    // Function to calculate fuel efficiency (km/L) based on speed and road type.
    double calculateFuelEfficiency(int speed, RoadType type) {
        double baseEfficiency = 16.0; // Sets a baseline efficiency for a standard car.
        
        // Reduces efficiency by 4 km/L if driving on local roads (stop-and-go).
        if (type == LOCAL) baseEfficiency -= 4.0; 
        
        // Adjusts efficiency for high speeds (aerodynamic drag).
        if (speed > 90) {
            double excess = speed - 90; // Calculates how much over 90 km/h the car is going.
            // Formula to reduce efficiency quadratically as speed increases.
            double drop = (excess * excess) / 400.0; 
            return max(5.0, baseEfficiency - drop); // Returns the result, ensuring it doesn't drop below 5.0.
        } else if (speed < 40) {
             // Reduces efficiency by 3 km/L for very low speeds (inefficient engine range).
            return baseEfficiency - 3.0;
        }
        return baseEfficiency; // Returns base efficiency for optimal speeds (40-90 km/h).
    }

    // ==========================================
    //      MAP DATA INITIALIZATION
    // ==========================================
    // Function to register a city name with an ID.
    void addCity(int id, string name) {
        if (id < MAX_CITIES) {          // Checks if the ID is within the valid range.
            cityNames[id] = name;       // Assigns the name to the array at the given index.
            cityCount = max(cityCount, id); // Updates total count to the highest ID used.
        }
    }

    // Function to add a road (edge) between two cities.
    void addRoad(int u, int v, double dist, TrafficLevel traf, RoadType type, string name) {
        // Adds connection from City U to City V.
        adj[u].push_back({v, dist, traf, type, name});
        // Adds connection from City V to City U (since roads are two-way).
        adj[v].push_back({u, dist, traf, type, name});
    }

    // Function to hardcode all the cities and roads into the system.
    void initializeMapData() {
        // 1. Define Cities
        addCity(1, "Karachi");      // Adds Karachi as City 1.
        addCity(2, "Hyderabad");    // Adds Hyderabad as City 2.
        addCity(3, "Sukkur");       // Adds Sukkur as City 3.
        addCity(4, "Multan");       // Adds Multan as City 4.
        addCity(5, "Faisalabad");   // Adds Faisalabad as City 5.
        addCity(6, "Lahore");       // Adds Lahore as City 6.
        addCity(7, "Islamabad");    // Adds Islamabad as City 7.
        addCity(8, "Peshawar");     // Adds Peshawar as City 8.
        addCity(9, "Quetta");       // Adds Quetta as City 9.
        addCity(10, "Gwadar");      // Adds Gwadar as City 10.
        addCity(11, "Sialkot");     // Adds Sialkot as City 11.
        addCity(12, "Abbottabad");  // Adds Abbottabad as City 12.
        addCity(13, "Gilgit");      // Adds Gilgit as City 13.
        addCity(14, "Sahiwal");     // Adds Sahiwal as City 14.
        addCity(15, "Bahawalpur");  // Adds Bahawalpur as City 15.

        // 2. Define Roads (Source, Dest, Dist, Traffic, Type, Name)
        
        // South Corridor
        addRoad(1, 2, 165, JAMMED, MOTORWAY, "M-9 Motorway"); // Adds M-9 from Karachi to Hyd.
        addRoad(2, 3, 330, MODERATE, HIGHWAY, "N-5 National Hwy"); // Adds N-5 from Hyd to Sukkur.
        addRoad(3, 4, 420, LOW, MOTORWAY, "M-5 Sukkur-Multan"); // Adds M-5 from Sukkur to Multan.
        addRoad(3, 9, 390, LOW, HIGHWAY, "N-65 Highway"); // Adds N-65 from Sukkur to Quetta.
        
        // Central Corridor
        addRoad(4, 5, 240, LOW, MOTORWAY, "M-4 Motorway"); // Adds M-4 from Multan to Faisalabad.
        addRoad(4, 15, 90, MODERATE, HIGHWAY, "N-5 Lodhran"); // Adds road from Multan to Bahawalpur.
        addRoad(15, 3, 300, LOW, HIGHWAY, "N-5 South"); // Adds road from BWP to Sukkur.
        addRoad(4, 14, 180, MODERATE, HIGHWAY, "N-5 GT Road"); // Adds road from Multan to Sahiwal.
        addRoad(14, 6, 170, HIGH, HIGHWAY, "N-5 Okara"); // Adds road from Sahiwal to Lahore.

        // Punjab Grid
        addRoad(5, 6, 150, HIGH, MOTORWAY, "M-3 Motorway"); // Adds M-3 from Fsd to Lahore.
        addRoad(5, 7, 320, LOW, MOTORWAY, "M-4 (Goa-Pindi)"); // Adds M-4 from Fsd to Islamabad.
        addRoad(6, 7, 375, MODERATE, MOTORWAY, "M-2 Motorway"); // Adds M-2 from Lahore to Islamabad.
        addRoad(6, 11, 130, MODERATE, MOTORWAY, "M-11 Sialkot"); // Adds M-11 from Lahore to Sialkot.
        
        // North Corridor
        addRoad(7, 8, 180, LOW, MOTORWAY, "M-1 Motorway"); // Adds M-1 from Islamabad to Peshawar.
        addRoad(7, 12, 120, HIGH, HIGHWAY, "N-35 Karakoram"); // Adds N-35 from Isb to Abbottabad.
        addRoad(12, 13, 450, HIGH, HIGHWAY, "KKH (Hazara)"); // Adds KKH from Abbottabad to Gilgit.

        // West Corridor
        addRoad(1, 10, 650, LOW, HIGHWAY, "N-10 Coastal Hwy"); // Adds Coastal Hwy from Karachi to Gwadar.
        addRoad(10, 9, 920, LOW, HIGHWAY, "N-85 Highway"); // Adds N-85 from Gwadar to Quetta.
        addRoad(9, 8, 800, LOW, HIGHWAY, "N-50 Zhob Route"); // Adds N-50 from Quetta to Peshawar.
    }

    // ==========================================
    //      MAIN ALGORITHM (DIJKSTRA)
    // ==========================================
    // Main function to calculate the shortest path.
    void findRoute(int startNode, int endNode, int speed) {
        // Validates that the input IDs exist in our data.
        if (startNode < 1 || startNode > cityCount || endNode < 1 || endNode > cityCount) {
            cout << "Invalid City ID Selected!" << endl; // Prints error if invalid.
            return; // Exits the function.
        }

        // DP Arrays and Priority Queue setup
        priority_queue<PqNode, vector<PqNode>, greater<PqNode>> pq; // Creates a Min-Heap priority queue.
        vector<double> minTime(MAX_CITIES, INF);    // Initializes all times to Infinity.
        vector<int> parent(MAX_CITIES, -1);         // Initializes parent array to track the path.
        vector<double> fuelConsumed(MAX_CITIES, 0.0); // Initializes fuel tracking array.
        vector<double> pathDist(MAX_CITIES, 0.0);   // Initializes distance tracking array.

        // Initialize Start Node
        minTime[startNode] = 0;          // Time to reach start node is 0.
        pq.push({startNode, 0});         // Adds start node to the queue.

        // Loop until there are no more nodes to process.
        while (!pq.empty()) {
            int u = pq.top().id;         // Gets the city ID with the lowest time cost.
            double currentTime = pq.top().timeCost; // Gets the time cost of that city.
            pq.pop();                    // Removes that city from the queue.

            // Optimization: If we found a faster way to 'u' previously, skip this one.
            if (currentTime > minTime[u]) continue;

            // Iterate through all roads connected to the current city 'u'.
            for (auto& edge : adj[u]) {
                int v = edge.destination; // Get the neighbor city ID.
                
                // --- PHYSICS LOGIC START ---
                double multiplier = getTrafficMultiplier(edge.traffic); // Get traffic delay factor.
                
                // Calculates base time: (Distance / Speed) * 60 to get minutes.
                double baseTime = (edge.distanceKM / speed) * 60.0;
                // Calculates real time including traffic delay.
                double realTime = baseTime * multiplier;

                // Relaxation Step: Check if this new path is faster than the known path.
                if (minTime[u] + realTime < minTime[v]) {
                    minTime[v] = minTime[u] + realTime; // Update shortest time to v.
                    parent[v] = u;                      // Set u as the parent of v (for path rebuilding).
                    pathDist[v] = pathDist[u] + edge.distanceKM; // Update total distance to v.
                    
                    // Calculate Fuel for this segment based on road type and speed.
                    double segmentEff = calculateFuelEfficiency(speed, edge.type);
                    // Add this segment's fuel usage to total fuel used so far.
                    fuelConsumed[v] = fuelConsumed[u] + (edge.distanceKM / segmentEff);
                    
                    pq.push({v, minTime[v]}); // Add v to the queue to explore its neighbors.
                }
                // --- PHYSICS LOGIC END ---
            }
        }

        // Check if the destination is reachable.
        if (minTime[endNode] == INF) {
            cout << "\nError: No road connection exists between these cities." << endl; // Prints error if unreachable.
            return;
        }

        // If reachable, print the full receipt/itinerary.
        printDetailedReceipt(startNode, endNode, parent, minTime[endNode], pathDist[endNode], fuelConsumed[endNode], speed);
    }

    // ==========================================
    //          OUTPUT FORMATTING
    // ==========================================
    // Function to print the final results table.
    void printDetailedReceipt(int start, int end, vector<int>& parent, double totalTime, double totalDist, double totalFuel, int speed) {
        // Reconstruct path by backtracking from destination to start.
        vector<int> path;
        for (int v = end; v != -1; v = parent[v]) {
            path.push_back(v); // Adds current city to path.
        }

        cout << "\n";
        cout << "########################################################" << endl;
        cout << "              SMART ROUTE NAVIGATOR RESULTS             " << endl;
        cout << "########################################################" << endl;
        cout << " Origin      : " << cityNames[start] << endl; // Prints origin city name.
        cout << " Destination : " << cityNames[end] << endl;   // Prints destination city name.
        cout << " Avg Speed   : " << speed << " km/h" << endl; // Prints user speed.
        cout << "--------------------------------------------------------" << endl;
        // Sets up table headers with specific widths.
        cout << left << setw(20) << "Leg From -> To" 
             << setw(18) << "Via Road" 
             << setw(10) << "Cond." 
             << "Dist." << endl;
        cout << "--------------------------------------------------------" << endl;

        // Print Path in correct order (Reverse of the backtracking loop).
        for (int i = path.size() - 1; i > 0; i--) {
            int u = path[i];   // Current city.
            int v = path[i-1]; // Next city in path.
            
            // Variables to hold road details for printing.
            string rName = "Unknown";
            string tCond = "Unknown";
            double d = 0;
            
            // Loop through edges to find the specific road connecting u and v.
            for(auto& e : adj[u]) {
                if(e.destination == v) {
                    rName = e.roadName;                // Get road name.
                    tCond = getTrafficString(e.traffic); // Get traffic string.
                    d = e.distanceKM;                  // Get distance.
                    break;                             // Stop looking once found.
                }
            }

            string leg = cityNames[u] + "->" + cityNames[v]; // Create string "CityA->CityB".
            // Truncate leg name if too long for cleaner output alignment.
            if(leg.length() > 18) leg = leg.substr(0, 18);

            // Print the row for this leg of the journey.
            cout << left << setw(20) << leg
                 << setw(18) << rName 
                 << setw(10) << tCond 
                 << d << " km" << endl;
        }

        cout << "--------------------------------------------------------" << endl;
        
        // Final Calculations for time and cost.
        int hrs = (int)totalTime / 60;      // Convert total minutes to hours.
        int mins = (int)totalTime % 60;     // Get remaining minutes.
        double cost = totalFuel * PRICE_PETROL; // Calculate total cost.

        // Print the final summary totals.
        cout << right << setw(35) << "TOTAL DISTANCE : " << setw(10) << totalDist << " km" << endl;
        cout << right << setw(35) << "ESTIMATED TIME : " << hrs << "h " << mins << "m" << endl;
        cout << right << setw(35) << "FUEL REQUIRED : " << fixed << setprecision(1) << totalFuel << " L" << endl;
        cout << right << setw(35) << "EST. FUEL COST : " << "PKR " << setprecision(2) << cost << endl;
        cout << "########################################################" << endl;
        cout << "Note: Traffic conditions may vary based on weather." << endl;
    }

    // Function to display the list of cities to the user.
    void displayMenu() {
        cout << "\n--- AVAILABLE CITIES ---" << endl;
        for (int i = 1; i <= cityCount; i++) {
            // Prints ID and Name in 3 columns for better layout.
            cout << left << setw(3) << i << ". " << setw(15) << cityNames[i];
            if (i % 3 == 0) cout << endl; // Inserts a new line every 3 cities.
        }
        if (cityCount % 3 != 0) cout << endl; // Ensures final newline if not divisible by 3.
    }
};

// ==========================================
//            MAIN EXECUTION
// ==========================================
int main() {
    RoutePlanner app;       // Creates an instance of the RoutePlanner application.
    int source, dest, speedInput; // Variables to store user inputs.
    char choice = 'y';      // Variable to control the main loop (y/n).

    // Main loop keeps program running until user chooses to exit.
    while (choice == 'y' || choice == 'Y') {
        cout << "\n=============================================" << endl;
        cout << "      PAKISTAN INTER-CITY ROUTE FINDER       " << endl;
        cout << "=============================================" << endl;
        
        app.displayMenu(); // Shows the city list.

        // Input Validation Loop for Source City.
        while (true) {
            cout << "\nEnter Start Location ID (1-15): ";
            // Reads input and checks if it's a number within range.
            if (cin >> source && source >= 1 && source <= 15) break;
            cout << "Invalid Input! Please enter a number between 1 and 15." << endl;
            cin.clear(); cin.ignore(1000, '\n'); // Clears error flags and bad input buffer.
        }

        // Input Validation Loop for Destination City.
        while (true) {
            cout << "Enter Destination ID (1-15) : ";
            if (cin >> dest && dest >= 1 && dest <= 15) break;
            cout << "Invalid Input! Please enter a number between 1 and 15." << endl;
            cin.clear(); cin.ignore(1000, '\n');
        }

        // Input Validation Loop for Driving Speed.
        while (true) {
            cout << "Enter Average Speed (40-160 km/h): ";
            if (cin >> speedInput && speedInput >= 40 && speedInput <= 160) break;
            cout << "Unrealistic speed! Please keep it between 40 and 160." << endl;
            cin.clear(); cin.ignore(1000, '\n');
        }

        // Runs the pathfinding algorithm with the gathered inputs.
        app.findRoute(source, dest, speedInput);

        // Asks user if they want to restart.
        cout << "\nDo you want to plan another trip? (y/n): ";
        cin >> choice;
    }

    cout << "\nThank you for using Smart Route Navigator. Drive Safely!" << endl;
    return 0; // Exits the program successfully.
}