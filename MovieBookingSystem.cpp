#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

// ==========================================
// DATA STRUCTURES
// ==========================================

struct Movie {
    string id;
    string name;
    string genre;
    int duration; // in minutes
    string language;
    double rating;
};

struct Show {
    string id;
    string movieId;
    string time;
    int screenNumber;
    double ticketPrice;
    vector<vector<char>> seats; // 10x10 grid (Rows A-J, Cols 1-10)
    int availableSeats;

    Show() {}
    Show(string id, string mId, string t, int screen, double price)
        : id(id), movieId(mId), time(t), screenNumber(screen), ticketPrice(price), availableSeats(100) {
        // Initialize 10x10 grid with 'O' (Available)
        seats = vector<vector<char>>(10, vector<char>(10, 'O'));
    }
};

struct Booking {
    string id;
    string customerName;
    string showId;
    vector<string> seatNumbers;
    double totalAmount;
};

// ==========================================
// SYSTEM CLASS
// ==========================================

class MovieBookingSystem {
private:
    unordered_map<string, Movie> movies;
    unordered_map<string, Show> shows;
    unordered_map<string, Booking> bookings;
    
    double totalRevenue;
    int totalSeatsBooked;

    // Helper to safely read inputs and clear buffer
    void clearInput() const {
        if(cin.fail()) {
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string toLower(string s) const {
        string res = s;
        transform(res.begin(), res.end(), res.begin(), ::tolower);
        return res;
    }

    string generateBookingId() {
        return "BKG" + to_string(1000 + bookings.size() + 1);
    }

public:
    MovieBookingSystem() : totalRevenue(0.0), totalSeatsBooked(0) {
        // Pre-populate some dummy data for demonstration
        movies["M1"] = {"M1", "Inception", "Sci-Fi", 148, "English", 8.8};
        movies["M2"] = {"M2", "Interstellar", "Sci-Fi", 169, "English", 8.6};
        
        shows["S1"] = Show("S1", "M1", "18:00", 1, 15.0);
        shows["S2"] = Show("S2", "M2", "20:30", 2, 20.0);
    }

    void run() {
        int choice = 0;
        do {
            cout << "\n=========================================\n";
            cout << "       MOVIE TICKET BOOKING SYSTEM       \n";
            cout << "=========================================\n";
            cout << "1. Admin Module\n";
            cout << "2. Customer Module\n";
            cout << "3. Exit System\n";
            cout << "Enter your choice: ";
            cin >> choice;
            clearInput();

            switch (choice) {
                case 1: adminMenu(); break;
                case 2: customerMenu(); break;
                case 3: cout << "Exiting system. Goodbye!\n"; break;
                default: cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 3);
    }

private:
    // ==========================================
    // MENUS
    // ==========================================

    void adminMenu() {
        int choice = 0;
        do {
            cout << "\n--- ADMIN MODULE ---\n";
            cout << "1. Add Movie\n";
            cout << "2. Remove Movie\n";
            cout << "3. Display All Movies\n";
            cout << "4. Add Show\n";
            cout << "5. Remove Show\n";
            cout << "6. Display All Shows\n";
            cout << "7. View Reports\n";
            cout << "8. Back to Main Menu\n";
            cout << "Enter choice: ";
            cin >> choice;
            clearInput();

            switch (choice) {
                case 1: addMovie(); break;
                case 2: removeMovie(); break;
                case 3: displayMovies(); break;
                case 4: addShow(); break;
                case 5: removeShow(); break;
                case 6: displayShows(); break;
                case 7: generateReports(); break;
                case 8: break;
                default: cout << "Invalid choice!\n";
            }
        } while (choice != 8);
    }

    void customerMenu() {
        int choice = 0;
        do {
            cout << "\n--- CUSTOMER MODULE ---\n";
            cout << "1. View All Movies\n";
            cout << "2. Search Movies\n";
            cout << "3. View Shows\n";
            cout << "4. Book Tickets\n";
            cout << "5. Cancel Tickets\n";
            cout << "6. View Booking History\n";
            cout << "7. Back to Main Menu\n";
            cout << "Enter choice: ";
            cin >> choice;
            clearInput();

            switch (choice) {
                case 1: displayMovies(); break;
                case 2: searchMovies(); break;
                case 3: displayShows(); break;
                case 4: bookTickets(); break;
                case 5: cancelTickets(); break;
                case 6: viewBookingHistory(); break;
                case 7: break;
                default: cout << "Invalid choice!\n";
            }
        } while (choice != 7);
    }

    // ==========================================
    // ADMIN FUNCTIONS
    // ==========================================

    void addMovie() {
        Movie m;
        cout << "Enter Movie ID: "; getline(cin, m.id);
        if (movies.find(m.id) != movies.end()) {
            cout << "Movie ID already exists!\n";
            return;
        }
        cout << "Enter Name: "; getline(cin, m.name);
        cout << "Enter Genre: "; getline(cin, m.genre);
        cout << "Enter Language: "; getline(cin, m.language);
        cout << "Enter Duration (mins): "; cin >> m.duration;
        cout << "Enter Rating (out of 10): "; cin >> m.rating;
        clearInput();
        
        movies[m.id] = m;
        cout << "Movie added successfully!\n";
    }

    void removeMovie() {
        string id;
        cout << "Enter Movie ID to remove: "; getline(cin, id);
        if (movies.erase(id)) {
            cout << "Movie removed successfully!\n";
        } else {
            cout << "Movie ID not found.\n";
        }
    }

    void addShow() {
        string id, mId, time;
        int screen;
        double price;

        cout << "Enter Show ID: "; getline(cin, id);
        if (shows.find(id) != shows.end()) {
            cout << "Show ID already exists!\n";
            return;
        }
        
        cout << "Enter Movie ID: "; getline(cin, mId);
        if (movies.find(mId) == movies.end()) {
            cout << "Movie ID not found! Add the movie first.\n";
            return;
        }

        cout << "Enter Time (e.g., 18:00): "; getline(cin, time);
        cout << "Enter Screen Number: "; cin >> screen;
        cout << "Enter Ticket Price: $"; cin >> price;
        clearInput();

        shows[id] = Show(id, mId, time, screen, price);
        cout << "Show added successfully!\n";
    }

    void removeShow() {
        string id;
        cout << "Enter Show ID to remove: "; getline(cin, id);
        if (shows.erase(id)) {
            cout << "Show removed successfully!\n";
        } else {
            cout << "Show ID not found.\n";
        }
    }

    void generateReports() {
        cout << "\n--- SYSTEM REPORTS ---\n";
        cout << "Total Bookings Made: " << bookings.size() << "\n";
        cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << "\n";
        cout << "Total Seats Booked: " << totalSeatsBooked << "\n";
        
        int totalCapacity = shows.size() * 100;
        int available = totalCapacity - totalSeatsBooked;
        cout << "Total Seats Available: " << available << "\n";
        
        if (totalCapacity > 0) {
            double occupancy = ((double)totalSeatsBooked / totalCapacity) * 100.0;
            cout << "Overall Occupancy Rate: " << occupancy << "%\n";
        }
    }

    // ==========================================
    // SHARED / CUSTOMER FUNCTIONS
    // ==========================================

    void displayMovies() {
        if (movies.empty()) {
            cout << "No movies available.\n";
            return;
        }
        cout << "\n--- AVAILABLE MOVIES ---\n";
        cout << left << setw(10) << "ID" << setw(25) << "Name" << setw(15) << "Genre" 
             << setw(15) << "Language" << setw(10) << "Duration" << "Rating\n";
        cout << string(85, '-') << "\n";
        
        for (const auto& pair : movies) {
            const Movie& m = pair.second;
            cout << left << setw(10) << m.id << setw(25) << m.name << setw(15) << m.genre 
                 << setw(15) << m.language << setw(10) << to_string(m.duration) + "m" 
                 << m.rating << "\n";
        }
    }

    void displayShows() {
        if (shows.empty()) {
            cout << "No shows available.\n";
            return;
        }
        cout << "\n--- SCHEDULED SHOWS ---\n";
        cout << left << setw(10) << "Show ID" << setw(25) << "Movie Name" << setw(10) << "Time" 
             << setw(10) << "Screen" << setw(15) << "Price" << "Available Seats\n";
        cout << string(85, '-') << "\n";
        
        for (const auto& pair : shows) {
            const Show& s = pair.second;
            string mName = movies.count(s.movieId) ? movies[s.movieId].name : "Unknown";
            cout << left << setw(10) << s.id << setw(25) << mName << setw(10) << s.time 
                 << setw(10) << s.screenNumber << "$" << setw(14) << s.ticketPrice 
                 << s.availableSeats << "/100\n";
        }
    }

    void searchMovies() {
        cout << "\nSearch by: 1. Name  2. Genre  3. Language\nSelect option: ";
        int opt;
        cin >> opt;
        clearInput();

        string query;
        cout << "Enter search term: ";
        getline(cin, query);
        query = toLower(query);

        bool found = false;
        cout << "\n--- SEARCH RESULTS ---\n";
        for (const auto& pair : movies) {
            const Movie& m = pair.second;
            bool match = false;
            
            if (opt == 1 && toLower(m.name).find(query) != string::npos) match = true;
            else if (opt == 2 && toLower(m.genre).find(query) != string::npos) match = true;
            else if (opt == 3 && toLower(m.language).find(query) != string::npos) match = true;

            if (match) {
                found = true;
                cout << "[" << m.id << "] " << m.name << " (" << m.language << ") - " << m.genre << " | Rating: " << m.rating << "\n";
            }
        }
        if (!found) cout << "No matching movies found.\n";
    }

    void displaySeatLayout(const Show& show) {
        cout << "\n--- SEAT LAYOUT FOR SHOW " << show.id << " ---\n";
        cout << "    ";
        for (int i = 1; i <= 10; ++i) cout << setw(3) << i;
        cout << "\n";
        
        for (int i = 0; i < 10; ++i) {
            cout << char('A' + i) << " | ";
            for (int j = 0; j < 10; ++j) {
                cout << setw(3) << show.seats[i][j];
            }
            cout << "\n";
        }
        cout << "O = Available | X = Booked\n";
    }

    void bookTickets() {
        string customerName, showId;
        cout << "Enter your Name: "; getline(cin, customerName);
        
        displayShows();
        cout << "\nEnter Show ID to book: "; getline(cin, showId);

        if (shows.find(showId) == shows.end()) {
            cout << "Invalid Show ID!\n";
            return;
        }

        Show& show = shows[showId];
        displaySeatLayout(show);

        int numSeats;
        cout << "\nHow many seats would you like to book? ";
        cin >> numSeats;
        clearInput();

        if (numSeats <= 0 || numSeats > show.availableSeats) {
            cout << "Invalid number of seats or not enough available seats!\n";
            return;
        }

        vector<string> selectedSeats;
        for (int i = 0; i < numSeats; ++i) {
            string seatCode;
            cout << "Enter Seat " << (i + 1) << " (e.g., A5): ";
            getline(cin, seatCode);

            if (seatCode.length() < 2 || seatCode.length() > 3) {
                cout << "Invalid format. Try again.\n";
                i--; continue;
            }

            char rChar = toupper(seatCode[0]);
            int row = rChar - 'A';
            int col = stoi(seatCode.substr(1)) - 1;

            if (row < 0 || row > 9 || col < 0 || col > 9) {
                cout << "Seat out of bounds. Try again.\n";
                i--; continue;
            }

            if (show.seats[row][col] == 'X') {
                cout << "Seat " << seatCode << " is already booked! Please select another.\n";
                i--; continue;
            }

            // Temporarily mark as selected for this session
            show.seats[row][col] = 'X';
            selectedSeats.push_back(seatCode);
        }

        // Finalize booking
        double amount = numSeats * show.ticketPrice;
        string bkgId = generateBookingId();
        
        Booking b = {bkgId, customerName, showId, selectedSeats, amount};
        bookings[bkgId] = b;
        
        show.availableSeats -= numSeats;
        totalRevenue += amount;
        totalSeatsBooked += numSeats;

        cout << "\n--- BOOKING SUCCESSFUL ---\n";
        cout << "Booking ID: " << bkgId << "\n";
        cout << "Total Amount: $" << fixed << setprecision(2) << amount << "\n";
        cout << "Seats: ";
        for (const string& s : selectedSeats) cout << s << " ";
        cout << "\nEnjoy the movie!\n";
    }

    void cancelTickets() {
        string bkgId;
        cout << "Enter Booking ID to cancel: "; getline(cin, bkgId);

        if (bookings.find(bkgId) == bookings.end()) {
            cout << "Booking ID not found.\n";
            return;
        }

        Booking& b = bookings[bkgId];
        Show& show = shows[b.showId];

        // Free the seats
        for (const string& seatCode : b.seatNumbers) {
            int row = toupper(seatCode[0]) - 'A';
            int col = stoi(seatCode.substr(1)) - 1;
            show.seats[row][col] = 'O'; // Make available again
        }

        show.availableSeats += b.seatNumbers.size();
        totalRevenue -= b.totalAmount;
        totalSeatsBooked -= b.seatNumbers.size();

        bookings.erase(bkgId);
        cout << "Booking cancelled successfully. Seats have been freed.\n";
    }

    void viewBookingHistory() {
        string name;
        cout << "Enter Customer Name to search: "; getline(cin, name);
        string lowerQuery = toLower(name);

        bool found = false;
        cout << "\n--- BOOKING HISTORY ---\n";
        for (const auto& pair : bookings) {
            const Booking& b = pair.second;
            if (toLower(b.customerName) == lowerQuery) {
                found = true;
                string mName = movies[shows[b.showId].movieId].name;
                cout << "Booking ID: " << b.id << " | Movie: " << mName 
                     << " | Show: " << b.showId << " | Amount: $" << b.totalAmount << "\n"
                     << "Seats: ";
                for (const string& s : b.seatNumbers) cout << s << " ";
                cout << "\n--------------------------\n";
            }
        }
        if (!found) {
            cout << "No bookings found for customer '" << name << "'.\n";
        }
    }
};

// ==========================================
// MAIN ENTRY POINT
// ==========================================
int main() {
    MovieBookingSystem system;
    system.run();
    return 0;
}
