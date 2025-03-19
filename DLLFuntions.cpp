#include "pch.h"
#include "DLLFunctionsHeader.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <cmath>
#include <execution>
#include <utility>   
#include <cstdlib>  
#include <sstream>  
#include <string>

using namespace std;
using namespace std::chrono;
namespace fs = filesystem;

// Global kintamieji
steady_clock::time_point start_time;
static int file_index = 0;
const string surname = "Jasiukevicius";
const string firstname = "Adomas";

// Laiko funkcijos
extern "C" __declspec(dllexport) void StartTimer() {
    start_time = high_resolution_clock::now();
}

extern "C" __declspec(dllexport) void EndTimer() {
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Total Execution Time: " << duration.count() << " ms" << endl;
}

extern "C" __declspec(dllexport) void OpenControlPanelComponent() {
    //Galimi pasirinkimai naudotojui
    std::cout << "Please choose a component to open by entering a number (1-5):" << std::endl;
    std::cout << "1. Display" << std::endl;
    std::cout << "2. Security" << std::endl;
    std::cout << "3. Power" << std::endl;
    std::cout << "4. Storage" << std::endl;
    std::cout << "5. Update" << std::endl;

    int choice = 0;
    bool validInput = false;
    std::string input;

    const char* commands[] = {
        "start control desk.cpl",                  // Display (1)
        "start control /name Microsoft.SecurityCenter", // Security (2)
        "start control powercfg.cpl",               // Power (3)
        "start diskmgmt.msc",                       // Storage (4)
        "start ms-settings:windowsupdate"           // Update (5)
    };

    while (!validInput) {
        std::cout << "Enter your choice (1-5): ";
        std::getline(std::cin, input);  

        std::stringstream ss(input);
        if (ss >> choice && ss.eof()) {  
            if (choice >= 1 && choice <= 5) {
                validInput = true; 
            }
            else {
                std::cout << "Invalid choice. Please enter a number between 1 and 5." << std::endl;
            }
        }
        else {
            std::cout << "Invalid input. Please enter a valid number between 1 and 5." << std::endl;
        }
    }

    system(commands[choice - 1]);
}

extern "C" __declspec(dllexport) void CreateFolders() {
    fs::remove_all(surname); //Nors ivykdzius programa sitas folderis yra deletinimas, atsarga gedos nedaro

    try {
        fs::create_directory(surname); //Sukuriam folderi ten pat, kur yra solutionas

        for (int i = 1; i <= 3; i++) { //Paprastas loopas sukurt 9 subfolderiukus AdomoXAdomasY
            string l1 = surname + "\\Adomas" + to_string(i);
            fs::create_directory(l1);

            for (int j = 1; j <= 3; j++) {
                string l2 = l1 + "\\Adomo" + to_string(i) + "Adomas" + to_string(j);
                fs::create_directory(l2);
            }
        }
    }
    catch (const exception& e) {
        cerr << "Folder creation error: " << e.what() << endl;
    }
}

extern "C" __declspec(dllexport) void ComputeTschirnhausen(double F, double x0, double xn, double dx) {
    vector<string> folders;

    for (const auto& l1_entry : fs::directory_iterator(surname)) { // Surenkam visus folderius kuriuos turim, AKA Adomo1Adomas1 ir tt.
        if (l1_entry.is_directory()) {
            for (const auto& l2_entry : fs::directory_iterator(l1_entry.path())) {
                if (l2_entry.is_directory()) {
                    folders.push_back(l2_entry.path().string());
                }
            }
        }
    }

    if (folders.empty()) {
        cerr << "No valid folders found!" << endl;
        return;
    }

    vector<ofstream> files;
    for (const auto& folder : folders) { //I kiekviena folderi idedam data.txt i kuriuos cikliskai dedam gautu tasku koordinates
        string path = folder + "\\data.txt";
        ofstream file(path, ios::app);
        if (!file) {
            cerr << "Failed to open file: " << path << endl;
            continue;
        }
        files.push_back(move(file));
    }

    if (files.empty()) {
        cerr << "No valid files opened!" << endl;
        return;
    }


    // Compute range where y_sq is positive
    for (double x = x0; x <= xn; x += dx) {
        const double y_sq = x * x * x + 3 * x * x - F; // Ziurim ar gauname teigiama reiksme y, is kurios trauksime sakni

        // Jei y_sq maziau uz 0, praleidziam ta x reiksme ir einam toliau
        if (y_sq < 0) continue;

        const double y = sqrt(y_sq);

        for (int sign = -1; sign <= 1; sign += 2) { //Apskaiciavimui dvieju reiksmiu, vienos neigiamos ir vienos teigiamos, +-
            if (y == 0 && sign == 1) break; // Jei y = 0, tai nereikes israsyti dvieju reiksmiu, 0 negali buti ir neigiamas ir teigiamas

            stringstream data_stream;

            data_stream << fixed << setprecision(6) << x << "," << y * sign << endl;

            int file_id = file_index % files.size(); // Apskaiciuojam, i kuri faila rasysim dabar. Musu atveju, nuo 0 iki 8.

            files[file_id] << data_stream.str();

            file_index++; // Padidinam indeksa, kad rasytume i sekanti faila
        }
    }

    for (auto& file : files) { //Uzdaro failus
        file.close();
    }

    MergeAndClean(F); //Naudojama apacioje aprasyta funkcija
}

extern "C" __declspec(dllexport) void MergeAndClean(double F) {
    std::vector<std::pair<double, double>> points;

    for (const auto& entry : fs::recursive_directory_iterator(surname)) { //Is data.txt isimam visus duomenis 
        if (entry.path().filename() == "data.txt") {
            std::ifstream in(entry.path());
            double x, y;
            char comma;

            while (in >> x >> comma >> y) {
                points.emplace_back(x, y);
            }

            in.close(); //Uzdarom ifstreama
            fs::remove(entry.path()); // Panaikinam Data.txt kai is jo isimam duomenis
        }
    }
    std::sort(std::execution::par, points.begin(), points.end()); //Paralelinis sortas dideliam duomenu kiekiui

    std::string out_name = "merged_F_" + std::to_string(static_cast<int>(F)) + ".txt";

    std::ofstream out(out_name, std::ios::out);
    std::vector<char> buffer;

    for (const auto& [x, y] : points) { //Naudoju bufferi rasyti duomenis i faila, taip pagreitinant procesa
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << x << "," << y << "\n";
        std::string line = oss.str();

        buffer.insert(buffer.end(), line.begin(), line.end());

        if (buffer.size() > 1024 * 64) { //Jei bufferio dydis pakankamai didelis pagal musu nustatyta parametra, viska isvedam i faila
            out.write(buffer.data(), buffer.size());
            buffer.clear();
        }
    }

    if (!buffer.empty()) { //Jei dar yra bufferyje duomenu, viska dumpinam i faila
        out.write(buffer.data(), buffer.size());
    }

    out.close(); //Uzdarom faila
}

extern "C" __declspec(dllexport) void DeleteFolders() { //Panaikinam Jasiukevicius Directory
    fs::remove_all(surname);
}