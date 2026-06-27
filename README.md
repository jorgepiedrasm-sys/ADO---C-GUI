[README.md](https://github.com/user-attachments/files/29402418/README.md)
# ABN System - Bus Ticket Reservation System

An interactive ticket sales, reservation, and management system developed in **C++** using a graphical user interface based on the classic **graphics.h (BGI)** library. This project simulates a visual seat assignment layout for buses traveling to major destinations in Mexico.

---

## 🚀 Key Features

*   **GUI & Interactive Registration:** Features custom menus with colored buttons, dynamic input boxes, and full mouse-click support on the graphics window.
*   **Interactive Seat Map:** Real-time visualization and selection of bus seating (20 seats per destination). Occupied seats are marked in dark grey, free seats in light grey, and the active selection in magenta.
*   **Trip Management:** Support for destination select (Veracruz, CDMX, Tlaxcala, Oaxaca, Jalisco) and trip types (Round-trip or One-way) with automated fare calculations.
*   **Cancellations & Searching:** Instant passenger search and booking cancellation, which dynamically frees up the assigned seat on the respective bus.
*   **Screen & File Reports:** Display of registered passengers directly on the GUI screen, plus file-export functionality that writes records to a local text file `boletos_registrados.txt`.

---

## 🛠️ Technical Skills & C++ Concepts Demonstrated

The development of this project showcases the practical application of fundamental and advanced structured programming and systems concepts in C++:

### 1. Custom Data Structures (`struct`)
The `boleto` struct is used to encapsulate all data relating to a passenger ticket:
*   Passenger contact info (name, email, phone).
*   Trip details (destination, trip type, seat tag, total fare, medical assistance flag).
*   Linking pointers for the data structure list (`boleto *siguiente`).

### 2. Pointers & Dynamic Memory (`pointers`)
*   **Dynamic Allocation:** Implements `new` and `delete` operators to allocate and free memory at runtime when adding new passengers or canceling tickets, avoiding the limitations of static arrays.
*   **Node Linking:** Utilizes pointers to traverse, insert, and delete nodes within the memory list.

### 3. Sorted Simply Linked Lists (`Linked Lists`)
*   Passenger records are stored in a simply linked list.
*   **Sorted Insertion:** The `agregarBoleto` function runs a custom sorting algorithm that inserts every new passenger node into its correct alphabetical order based on their name, using the `strcmp` string comparison function.

### 4. State Management with Arrays
*   Bus occupancy is tracked independently for each destination using boolean arrays (`bus_veracruz[20]`, `bus_cdmx[20]`, etc.). This links the visual layout directly with database records for seat availability and releases.

### 5. Custom Event Handling & Input Controls
*   **Backspace-aware String Input:** Implements a keyboard listening function (`capturarString`) that reads characters in real-time without echo (`getch`) and prints them dynamically, handling the *Backspace* key to modify user input in the GUI.
*   **Mouse Interaction:** Uses library routines such as `ismouseclick` and `getmouseclick` to map pixel click coordinates to logical menu actions or grid selections.

---

## 📚 Libraries Used

*   `<graphics.h>` / BGI (Borland Graphics Interface): Renders the visual GUI, draws shapes, manages color palettes, and handles mouse tracking.
*   `<conio.h>`: Listens for raw keyboard events (`getch`, `kbhit`) to capture text inside the graphical windows.
*   `<iostream>` & `<fstream>`: Manages files and console streams to generate `boletos_registrados.txt`.
*   `<cstring>` & `<string>`: Performs string operations, alphabetical string sorting (`strcmp`), and conversions.

---

## 💻 Requirements & Compilation

Since this project depends on the legacy `<graphics.h>` library, you need a C++ compiler set up with BGI library packages:
1.  **Recommended IDE:** Dev-C++ with the `WinBGIm` package pre-installed, or Code::Blocks configured with BGI.
2.  Ensure that the header `graphics.h` and the library `libbgi.a` are correctly linked. Linker flags needed: `-lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32`.
