<h1 align="center">Library Management System </h1>

A robust Command Line application designed to streamline library operations. This project manages book inventories, student records, and automates administrative tasks like issuing, returning, and fine calculation.

### Features
- Inventory Management: Add, delete, and display books with unique IDs and tracking for multiple copies.

- Member Records: Maintain a database of students, including their age and a history of issued books.

- Smart Issuing System: Automatically decrements book stock upon issuance and tracks issue dates.

- Automated Fine Calculation: Built-in logic to detect late returns (beyond 7 days) and calculate fines using the **chrono** library.

- Data Persistence: Automatically saves and loads all data from books.txt and students.txt, ensuring no data loss between sessions.

## Build & Run

1. Clone the Repo
    
    ```
    git clone https://github.com/terminalpirate/Library-Management-System.git
    ```
2. Compile the script

    ```
    make lms
    ```
3. run the output file
    ```
    ./lms
    ```