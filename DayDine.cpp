#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Data Structures
struct Task {
    string name;
    int duration;
    int priority;
    char mark;
    bool is_done = false;
    string star_type = "Gold";
    string original_date;
};

struct DailyStats {
    string date;
    int total_tasks;
    int completed_tasks;
    int satisfaction_percentage = 0;
    string self_message = "";
    vector<Task> tasks_snapshot;
};

class User {
public:
    string name = "Naema";
    int xp = 0;
    int streak = 0;
    string last_login_date = "2025-12-12";

    void displayRank() const {
        cout << "XP: " << xp << " | Streak: " << streak << " days \n";
        if (streak > 5) cout << "Rank: Master Achiever\n";
        else if (streak > 0) cout << "Rank: Junior Achiever\n";
        else cout << "Rank: New Starter\n";
    }

    void checkStreak() {
        srand(time(0));
        if (streak == 0 && (rand() % 10) > 5) {
             streak = 1;
        } else if (streak > 0 && (rand() % 10) < 3) {
             cout << "\n! Note: Your streak was reset.\n";
             streak = 0;
        } else if (streak > 0) {
             streak++;
        }
    }
};

// Core Functions
string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d", ltm);
    return string(buffer);
}

void sortTasks(vector<Task>& tasks) {
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.priority > b.priority;
    });
}

// دالة التحفيز الجديدة (Updated Encouragement Function)
string getEncouragement() {
    vector<string> messages = {
        "proud of you.",
        "This is a great achievement!",
        "You did an amazing job!",
        "That's a big win!",
        "Well done, champion!",
        "Keep it up!",
        "You're doing so well!",
        "Don't stop now!",
        "Keep going, you're strong!",
        "You're on fire!"
    };
    return messages[rand() % messages.size()];
}

void saveDailyStats(const DailyStats& stats) {
    ofstream file("daydone_archive.txt", ios::app);
    if (file.is_open()) {
        file << stats.date << "|"
             << stats.total_tasks << "|"
             << stats.completed_tasks << "|"
             << stats.satisfaction_percentage << "|"
             << stats.self_message << "\n";

        for (const auto& task : stats.tasks_snapshot) {
            file << "TASK_SNA| " << stats.date << "|"
                 << task.name << "|"
                 << task.duration << "|"
                 << task.is_done << "|"
                 << task.star_type << "|"
                 << task.original_date << "\n";
        }

        file.close();
        cout << "\n(System: Data saved to archive.)\n";
    } else {
        cout << "\n(System: Error saving data.)\n";
    }
}

vector<Task> loadAccumulatedTasks(const string& todayDate) {
    vector<Task> accumulatedTasks;
    ifstream file("daydone_archive.txt");
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.rfind("TASK_SNA|", 0) == 0) {
                stringstream ss(line.substr(9));
                string segment;
                vector<string> segments;
                while (getline(ss, segment, '|')) {
                    segments.push_back(segment);
                }

                if (segments.size() >= 6) {
                    Task t;
                    t.original_date = segments[0];
                    t.name = segments[1];
                    t.duration = stoi(segments[2]);
                    bool isDone = (segments[3] == "1");
                    t.star_type = segments[4];

                    if (!isDone && t.original_date != todayDate) {
                        accumulatedTasks.push_back(t);
                    }
                }
            }
        }
        file.close();
    }
    return accumulatedTasks;
}

void updateArchiveForSilverStar(const string& originalDate, const string& taskName) {
    ifstream inputFile("daydone_archive.txt");
    string line;
    string updatedContent = "";
    bool updated = false;

    if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
            if (line.rfind("TASK_SNA|", 0) == 0) {
                if (line.find("|" + originalDate + "|") != string::npos &&
                    line.find("|" + taskName + "|") != string::npos) {

                    size_t pos_done = line.find("0|Gold|");
                    if (pos_done != string::npos) {
                        line.replace(pos_done, 6, "1|Silver|");
                        updated = true;
                    }
                }
            }
            updatedContent += line + "\n";
        }
        inputFile.close();

        if (updated) {
            ofstream outputFile("daydone_archive.txt");
            if (outputFile.is_open()) {
                outputFile << updatedContent;
                outputFile.close();
            }
        }
    }
}


void printDashboard(const vector<Task>& tasks) {
    cout << "\n\t\t=== Daily Dashboard ===\n";
    cout << "+-----+---------------------------------+----------+----------------+\n";
    cout << "| Code | Task Name                       | Duration (h) | To Complete     |\n";
    cout << "+-----+---------------------------------+----------+----------------+\n";
    for (const auto& task : tasks) {
        string status = task.is_done ? "[DONE]" : "Type '" + string(1, task.mark) + "'";
        printf("|  %c  | %-30s | %10d | %-15s |\n", task.mark, task.name.c_str(), task.duration, status.c_str());
    }
    cout << "+-----+---------------------------------+----------+----------------+\n";
    cout << "\n* To finish your study day: Type 'DAYDONE'\n";
}

void showAnalytics() {
    cout << "\n\t\t=== DayDone Analytics (Archive) ===\n";
    cout << "+------------+-------+-------+----------+--------------------------------------+\n";
    cout << "| Date       | Total | Done  | Satisf. %| Self Message                         |\n";
    cout << "+------------+-------+-------+----------+--------------------------------------+\n";

    ifstream file("daydone_archive.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.rfind("TASK_SNA|", 0) == 0) continue;

            stringstream ss(line);
            string segment;
            vector<string> segments;
            while (getline(ss, segment, '|')) {
                segments.push_back(segment);
            }

            if (segments.size() >= 5) {
                cout << "| " << setw(10) << segments[0] << " | "
                     << setw(5) << segments[1] << " | "
                     << setw(5) << segments[2] << " | "
                     << setw(8) << segments[3] << " | "
                     << setw(36) << segments[4] << " |\n";
            }
        }
        file.close();
    }
    cout << "+------------+-------+-------+----------+--------------------------------------+\n";
}

// Main Logic
int main() {
    srand(time(0));

    User currentUser;
    vector<Task> currentTasks;
    string currentDate = getCurrentDate();
    int totalAvailableHours = 0;
    int totalTaskHours = 0;

    // [1] Smart Start
    cout << "========================================================\n";
    cout << "DayDone - Your Smart Study Companion\n";
    cout << "========================================================\n";
    cout << "Welcome " << currentUser.name << "! Today is: " << currentDate << "\n";

    currentUser.checkStreak();
    currentUser.displayRank();
    cout << "--------------------------------------------------------\n";
    cout << "Let's start the day with enthusiasm!\n";

    // --- Accumulated Tasks Logic ---
    vector<Task> accumulatedTasks = loadAccumulatedTasks(currentDate);
    if (!accumulatedTasks.empty()) {
        cout << "\n[NOTICE]: You have " << accumulatedTasks.size() << " accumulated tasks from past days.\n";
        cout << "Do you want to study an old task today? (Y/N): ";
        char reviewChoice;
        cin >> reviewChoice;
        cin.ignore();

        if (toupper(reviewChoice) == 'Y') {
            cout << "\n--- Accumulated Tasks ---\n";
            int idx = 1;
            for (const auto& t : accumulatedTasks) {
                cout << idx++ << ". " << t.name << " (" << t.original_date << ") - Time: " << t.duration << "h\n";
            }

            int taskIndex;
            cout << "Enter the number of the old task to add: ";
            // Added input validation
            while (!(cin >> taskIndex) || taskIndex <= 0 || taskIndex > accumulatedTasks.size()) {
                cout << "Invalid number. Please enter a number from the list (1 to " << accumulatedTasks.size() << "): ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            cin.ignore();

            if (taskIndex > 0 && taskIndex <= accumulatedTasks.size()) {
                Task oldTask = accumulatedTasks[taskIndex - 1];
                oldTask.original_date = accumulatedTasks[taskIndex - 1].original_date;
                oldTask.star_type = "Bronze";
                oldTask.priority = 1;

                currentTasks.push_back(oldTask);
                cout << "SUCCESS: '" << oldTask.name << "' added as a Bronze Task.\n";
            }
        }
    }
    cout << "--------------------------------------------------------\n";

    // [2] Reality Planning
    cout << "\n[2] Reality Planning:\n";
    cout << "How many hours are available for studying today? (Number): ";
    // Added input validation
    while (!(cin >> totalAvailableHours) || totalAvailableHours <= 0) {
        cout << "Invalid input. Please enter a number greater than 0: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cin.ignore();

    int numSubjects;
    cout << "How many NEW subjects are you planning to do today, " << currentUser.name << "? (Number): ";
    // Added input validation
    while (!(cin >> numSubjects) || numSubjects < 0) {
        cout << "Invalid input. Please enter a non-negative number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cin.ignore();

    bool realityCheckPassed = false;

    // Input loop with Reality Check (Fixing the Infinite Loop issue)
    while (!realityCheckPassed) {
        vector<Task> newTasks;
        totalTaskHours = 0;
        char taskMark = 'A';

        cout << "\nEnter tasks and details (Priority 10 is highest):\n";
        for (int i = 0; i < numSubjects; ++i) {
            Task t;
            cout << "--- Task " << i + 1 << " (" << taskMark << ") ---\n";
            cout << "Task Name: ";
            getline(cin, t.name);
            cout << "Time Estimate (hours): ";

            // Added input validation for duration
            while (!(cin >> t.duration) || t.duration <= 0) {
                cout << "Invalid input. Please enter a number greater than 0: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "Priority (1-10): ";

            // Added input validation for priority
            while (!(cin >> t.priority) || t.priority < 1 || t.priority > 10) {
                cout << "Invalid input. Priority must be between 1 and 10: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            cin.ignore();

            t.mark = taskMark++;
            t.original_date = currentDate;
            totalTaskHours += t.duration;
            newTasks.push_back(t);
        }

        // The Filter: Check Reality
        if (totalTaskHours > totalAvailableHours) {
            cout << "\n❌ ERROR: Your schedule is NOT realistic, " << currentUser.name << "!\n";
            cout << "Total task hours (" << totalTaskHours << "h) is more than available time (" << totalAvailableHours << "h).\n";
            cout << ">>> Please reduce the number of tasks. <<<\n";

            cout << "How many NEW subjects are you adding now?: ";
            // Added input validation for numSubjects inside the loop
            while (!(cin >> numSubjects) || numSubjects < 0) {
                cout << "Invalid input. Please enter a non-negative number: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            cin.ignore();
        } else {
            currentTasks.insert(currentTasks.end(), newTasks.begin(), newTasks.end());
            realityCheckPassed = true;
            cout << "\nGreat work, " << currentUser.name << "! Your schedule is realistic. Let's execute.\n";
        }
    }

    // Sort all tasks
    sortTasks(currentTasks);

    // Re-assign marks A, B, C... after sorting
    char taskMark = 'A';
    for (auto& task : currentTasks) {
        task.mark = taskMark++;
    }

    // [3] The Dashboard & [4] Chatbot Reaction
    string userInput;
    int completedTasks = 0;

    do {
        printDashboard(currentTasks);
        cout << "\nEnter the Code (letter) to complete a task, or 'DAYDONE' to finish the day: ";
        getline(cin, userInput);
        transform(userInput.begin(), userInput.end(), userInput.begin(), ::toupper);

        if (userInput.length() == 1 && userInput[0] >= 'A' && userInput[0] <= 'Z') {
            char mark = userInput[0];
            bool found = false;
            for (auto& task : currentTasks) {
                if (task.mark == mark && !task.is_done) {
                    task.is_done = true;
                    completedTasks++;
                    currentUser.xp += 10;

                    if (task.star_type == "Bronze") {
                         updateArchiveForSilverStar(task.original_date, task.name);
                         cout << "\n[NOTE]: Old task completed! Silver Star achieved for " << task.original_date << ".\n";
                    }

                    cout << "\n========================================================\n";
                    cout << getEncouragement() << endl; // Calling the new strong message function
                    cout << "Task '" << task.name << "' marked as [DONE].\n";
                    cout << "Current XP: " << currentUser.xp << " XP.\n";
                    cout << "========================================================\n";
                    found = true;
                    break;
                } else if (task.mark == mark && task.is_done) {
                    cout << "\n[NOTICE]: This task is already completed.\n";
                    found = true;
                    break;
                }
            }
            if (!found && userInput != "DAYDONE") {
                cout << "\n! Invalid Code. Please enter the correct task letter.\n";
            }
        }

    } while (userInput != "DAYDONE");

    // [5] End of Day Ritual (DayDone)
    cout << "\n========================================================\n";
    cout << "End of Day Ritual - DAYDONE\n";
    cout << "========================================================\n";

    float completionPercentage = currentTasks.empty() ? 0 : (float)completedTasks / currentTasks.size() * 100;

    if (completedTasks == currentTasks.size() && !currentTasks.empty()) {
        cout << "Congratulations! 100% completion today! You finished ALL your tasks, " << currentUser.name << "!\n";
    } else if (completedTasks > 0) {
        cout << "Good job! You completed " << completedTasks << " out of " << currentTasks.size() << " tasks (" << fixed << setprecision(0) << completionPercentage << "%).\n";
        cout << "Keep going, " << currentUser.name << ", tomorrow will be better!\n";
    } else {
        cout << "It's okay, " << currentUser.name << ". Starting is what matters. Tomorrow is a fresh start!\n";
    }

    // Satisfaction Question
    DailyStats todayStats;
    todayStats.date = currentDate;
    todayStats.total_tasks = currentTasks.size();
    todayStats.completed_tasks = completedTasks;
    todayStats.tasks_snapshot = currentTasks;

    cout << "\nHow satisfied are you with yourself today? (0-100%): ";
    cin >> todayStats.satisfaction_percentage;
    cin.ignore();

    // Message to Self
    cout << "What would you like to say to yourself today? (Message for the archive): ";
    getline(cin, todayStats.self_message);

    // Save Data
    saveDailyStats(todayStats);

    // [6] Analytics
    cout << "\nDo you want to see your historical data? (Type 'STATS'): ";
    string statsChoice;
    getline(cin, statsChoice);
    transform(statsChoice.begin(), statsChoice.end(), statsChoice.begin(), ::toupper);

    if (statsChoice == "STATS") {
        showAnalytics();
    }

    cout << "\nThank you for using DayDone, " << currentUser.name << ". See you tomorrow!\n";

    return 0;
}
