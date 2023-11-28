#include <iostream>
#include <fstream>
#include <ctime>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void markStudentAttendance(const string& name, const string& rollNumber);
void markTeacherAttendance(const string& name, const string& regNumber, int shiftNumber);
void viewStudentAttendance();
void viewTeacherAttendance();

int main() {
    int choice;

    do {
        cout << "1) Mark Student Attendance\n";
        cout << "2) Mark Teacher Attendance\n";
        cout << "3) View Student Attendance\n";
        cout << "4) View Teacher Attendance\n";
        cout << "5) Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer

        switch (choice) {
        case 1: {
            string name, rollNumber;
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Roll Number: ";
            getline(cin, rollNumber);
            markStudentAttendance(name, rollNumber);
            break;
        }
        case 2: {
            string name, regNumber;
            int shiftNumber;
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Registration Number: ";
            getline(cin, regNumber);
            cout << "Enter Shift Number: ";
            cin >> shiftNumber;
            markTeacherAttendance(name, regNumber, shiftNumber);
            break;
        }
        case 3:
            viewStudentAttendance();
            break;
        case 4:
            viewTeacherAttendance();
            break;
        case 5:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}

void markStudentAttendance(const string& name, const string& rollNumber) {
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cerr << "Error opening video capture." << endl;
        return;
    }

    Mat frame;
    namedWindow("Face Detection", WINDOW_NORMAL);
    resizeWindow("Face Detection", 640, 480);

    String face_cascade_path = "haarcascade_frontalface_default.xml";
    CascadeClassifier face_cascade;

    if (!face_cascade.load(face_cascade_path)) {
        cerr << "Error loading face cascade. Check the file path." << endl;
        return;
    }

    time_t start_time = time(0);
    while (time(0) - start_time < 3) {
        cap >> frame;

        if (frame.empty()) {
            cerr << "Error capturing frame." << endl;
            break;
        }

        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        vector<Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));

        for (const Rect& face : faces) {
            rectangle(frame, face, Scalar(0, 255, 0), 2);
        }

        imshow("Face Detection", frame);

        if (waitKey(1) == 27) {
            destroyAllWindows();
            return;
        }
    }

    destroyAllWindows();

    time_t now = time(0);
    tm currentDate;
    localtime_s(&currentDate, &now);

    ofstream log("studentattendance_log.txt", ios::app);
    log << currentDate.tm_mday << "-" << currentDate.tm_mon + 1 << "-"
        << currentDate.tm_year + 1900 << " " << name << " " << rollNumber << endl;
    log.close();

    cout << "Student Attendance marked for today: " << name << " " << rollNumber << endl;
}

void markTeacherAttendance(const string& name, const string& regNumber, int shiftNumber) {
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cerr << "Error opening video capture." << endl;
        return;
    }

    Mat frame;
    namedWindow("Face Detection", WINDOW_NORMAL);
    resizeWindow("Face Detection", 640, 480);

    String face_cascade_path = "haarcascade_frontalface_default.xml";
    CascadeClassifier face_cascade;

    if (!face_cascade.load(face_cascade_path)) {
        cerr << "Error loading face cascade. Check the file path." << endl;
        return;
    }

    time_t start_time = time(0);
    while (time(0) - start_time < 3) {
        cap >> frame;

        if (frame.empty()) {
            cerr << "Error capturing frame." << endl;
            break;
        }

        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        vector<Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));

        for (const Rect& face : faces) {
            rectangle(frame, face, Scalar(0, 255, 0), 2);
        }

        imshow("Face Detection", frame);

        if (waitKey(1) == 27) {
            destroyAllWindows();
            return;
        }
    }

    destroyAllWindows();

    time_t now = time(0);
    tm currentDate;
    localtime_s(&currentDate, &now);

    ofstream log("teacherattendance_log.txt", ios::app);
    log << currentDate.tm_mday << "-" << currentDate.tm_mon + 1 << "-"
        << currentDate.tm_year + 1900 << " " << name << " " << regNumber << " " << shiftNumber << endl;
    log.close();

    cout << "Teacher Attendance marked for today: " << name << " " << regNumber << " " << shiftNumber << endl;
}

void viewStudentAttendance() {
    ifstream log("studentattendance_log.txt");

    if (!log.is_open()) {
        cerr << "Error opening student attendance log file." << endl;
        return;
    }

    cout << "Student Attendance Log:\n";
    string line;

    while (getline(log, line)) {
        cout << line << endl;
    }

    log.close();
}

void viewTeacherAttendance() {
    ifstream log("teacherattendance_log.txt");

    if (!log.is_open()) {
        cerr << "Error opening teacher attendance log file." << endl;
        return;
    }

    cout << "Teacher Attendance Log:\n";
    string line;

    while (getline(log, line)) {
        cout << line << endl;
    }

    log.close();
}
