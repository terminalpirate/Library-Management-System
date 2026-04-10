#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono> // for date/time operations

using namespace std;

class book {
private:
    int id, copies;
    string author, name;
    chrono::system_clock::time_point issueDate;

public:
    book(int id, int copies, string name, string author) {
        this->id = id;
        this->copies = copies;
        this->name = name;
        this->author = author;
        this->issueDate = chrono::system_clock::now(); // Set current date/time as issue date
    }

    // Calculate fine based on current date and issue date
    int calculateFine() {
        chrono::system_clock::time_point now = chrono::system_clock::now();
        chrono::duration<double> diff = now - issueDate;
        int daysLate = (int)diff.count() / (60 * 60 * 24); // Calculate days difference
        if (daysLate > 7) {
            return (daysLate - 7); // Rs. 1 per day after a week
        }
        return 0; // No fine within the first week
    }

    // Getter methods
    int getid() { return id; }
    int getcopies() { return copies; }
    string getname() { return name; }
    string getauthor() { return author; }

    // Display book information
    void displaybookinfo() {
        cout << "ID: " << id << ", Name: " << name << ", Author: " << author << ", Copies: " << copies << endl;
    }

    // Update copies when a book is returned
    void returnBook() {
        copies++;
    }

    // Issue a book (decrement copies)
    bool issueBook() {
        if (copies > 0) {
            copies--;
            issueDate = chrono::system_clock::now(); // Update issue date to current date
            return true; // Book issued successfully
        }
        return false; // Book not available to issue
    }

    // Save book data to file
    void saveToFile(ofstream& outFile) {
        outFile << id << "\n" << copies << "\n" << name << "\n" << author << "\n";
    }

    // Load book data from file
    static book loadFromFile(ifstream& inFile) {
        int id, copies;
        string name, author;
        inFile >> id >> copies;
        inFile.ignore();
        getline(inFile, name);
        getline(inFile, author);
        return book(id, copies, name, author);
    }
};

class student {
private:
    int stu_id, age;
    string name;
    vector<int> issuedBooks; // Track IDs of books issued by student

public:
    student(int age, int stu_id, string name) {
        this->age = age;
        this->name = name;
        this->stu_id = stu_id;
    }

    // Getter methods
    int getstu_id() { return stu_id; }
    int getage() { return age; }
    string getname() { return name; }

    // Issue a book (add book ID to issuedBooks)
    bool issueBook(int bookID) {
        issuedBooks.push_back(bookID);
        return true; // Book issued successfully (no checks for now)
    }

    // Display student information
    void displayStudentInfo() {
        cout << "Student ID: " << stu_id << ", Name: " << name << ", Age: " << age << endl;
        cout << "Books Issued: ";
        if (issuedBooks.empty()) {
            cout << "None" << endl;
        } else {
            for (int id : issuedBooks) {
                cout << id << " ";
            }
            cout << endl;
        }
    }

    // Save student data to file
    void saveToFile(ofstream& outFile) {
        outFile << stu_id << "\n" << age << "\n" << name << "\n";
        outFile << issuedBooks.size() << "\n";
        for (int id : issuedBooks) {
            outFile << id << "\n";
        }
    }

    // Load student data from file
    static student loadFromFile(ifstream& inFile) {
        int stu_id, age, numBooks;
        string name;
        inFile >> stu_id >> age;
        inFile.ignore();
        getline(inFile, name);
        student s(age, stu_id, name);
        inFile >> numBooks;
        for (int i = 0; i < numBooks; ++i) {
            int bookID;
            inFile >> bookID;
            s.issueBook(bookID);
        }
        return s;
    }
};

vector<book> books;
vector<student> students;

void add_book(int id, int copies, string name, string author) {
    books.push_back(book(id, copies, name, author));
    cout << "Book added successfully!" << endl;
}

void display_books() {
    cout << "Displaying all books:" << endl;
    for (book& b : books) {
        b.displaybookinfo();
    }
}

void delete_book(int id) {
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->getid() == id) {
            books.erase(it);
            cout << "Book with ID " << id << " deleted successfully!" << endl;
            return;
        }
    }
    cout << "Book with ID " << id << " not found!" << endl;
}

void return_book(int id) {
    for (auto& b : books) {
        if (b.getid() == id) {
            int fine = b.calculateFine(); // Calculate fine for the book
            if (fine > 0) {
                cout << "Fine of Rs. " << fine << " is applicable for returning this book late." << endl;
            }
            b.returnBook(); // Update copies when book is returned
            cout << "Book returned successfully!" << endl;
            return;
        }
    }
    cout << "Book with ID " << id << " not found!" << endl;
}

void add_student(int age, int stu_id, string name) {
    students.push_back(student(age, stu_id, name));
    cout << "Student added successfully!" << endl;
}

void display_students() {
    cout << "Displaying all students:" << endl;
    for (student& s : students) {
        s.displayStudentInfo();
    }
}

void delete_student(int stu_id) {
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->getstu_id() == stu_id) {
            students.erase(it);
            cout << "Student with ID " << stu_id << " deleted successfully!" << endl;
            return;
        }
    }
    cout << "Student with ID " << stu_id << " not found!" << endl;
}

void issue_book_to_student(int bookID, int studentID) {
    for (auto& b : books) {
        if (b.getid() == bookID) {
            for (auto& s : students) {
                if (s.getstu_id() == studentID) {
                    if (b.issueBook()) {
                        s.issueBook(bookID);
                        cout << "Book issued successfully to student ID: " << studentID << endl;
                    } else {
                        cout << "Book with ID " << bookID << " is not available to issue." << endl;
                    }
                    return;
                }
            }
            cout << "Student with ID " << studentID << " not found!" << endl;
            return;
        }
    }
    cout << "Book with ID " << bookID << " not found!" << endl;
}

void save_data() {
    ofstream bookFile("books.txt");
    ofstream studentFile("students.txt");

    if (bookFile.is_open()) {
        bookFile << books.size() << "\n";
        for (book& b : books) {
            b.saveToFile(bookFile);
        }
        bookFile.close();
    }

    if (studentFile.is_open()) {
        studentFile << students.size() << "\n";
        for (student& s : students) {
            s.saveToFile(studentFile);
        }
        studentFile.close();
    }
}

void load_data() {
    ifstream bookFile("books.txt");
    ifstream studentFile("students.txt");

    if (bookFile.is_open()) {
        size_t bookCount;
        bookFile >> bookCount;
        bookFile.ignore();
        for (size_t i = 0; i < bookCount; ++i) {
            books.push_back(book::loadFromFile(bookFile));
        }
        bookFile.close();
    }

    if (studentFile.is_open()) {
        size_t studentCount;
        studentFile >> studentCount;
        studentFile.ignore();
        for (size_t i = 0; i < studentCount; ++i) {
            students.push_back(student::loadFromFile(studentFile));
        }
        studentFile.close();
    }
}

int main() {
    load_data();

    int choice;
    do {
        cout << "\nLibrary Management System Menu:" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Display Books" << endl;
        cout << "3. Delete Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. Add Student" << endl;
        cout << "6. Display Students" << endl;
        cout << "7. Delete Student" << endl;
        cout << "8. Issue Book to Student" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int id, copies;
                string name, author;
                cout << "Enter Book ID: ";
                cin >> id;
                cout << "Enter Copies: ";
                cin >> copies;
                cin.ignore();
                cout << "Enter Book Name: ";
                getline(cin, name);
                cout << "Enter Author Name: ";
                getline(cin, author);
                add_book(id, copies, name, author);
                break;
            }
            case 2:
                display_books();
                break;
            case 3: {
                int id;
                cout << "Enter Book ID to delete: ";
                cin >> id;
                delete_book(id);
                break;
            }
            case 4: {
                int id;
                cout << "Enter Book ID to return: ";
                cin >> id;
                return_book(id);
                break;
            }
            case 5: {
                int age, stu_id;
                string name;
                cout << "Enter Student Age: ";
                cin >> age;
                cout << "Enter Student ID: ";
                cin >> stu_id;
                cin.ignore();
                cout << "Enter Student Name: ";
                getline(cin, name);
                add_student(age, stu_id, name);
                break;
            }
            case 6:
                display_students();
                break;
            case 7: {
                int stu_id;
                cout << "Enter Student ID to delete: ";
                cin >> stu_id;
                delete_student(stu_id);
                break;
            }
            case 8: {
                int bookID, studentID;
                cout << "Enter Book ID to issue: ";
                cin >> bookID;
                cout << "Enter Student ID to issue to: ";
                cin >> studentID;
                issue_book_to_student(bookID, studentID);
                break;
            }
            case 9:
                cout << "Exiting program." << endl;
                save_data();
                break;
            default:
                cout << "Invalid choice. Please enter a number from 1 to 9." << endl;
                break;
        }
    } while (choice != 9);

    return 0;
}