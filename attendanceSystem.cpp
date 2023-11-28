#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;
using namespace cv;

int main() {
    // Load the pre-trained face detection model
    String face_cascade_path = "haarcascade_frontalface_default.xml";
    CascadeClassifier face_cascade;

    if (!face_cascade.load(face_cascade_path)) {
        cerr << "Error loading face cascade. Check the file path." << endl;
        return -1;
    }

    // Open the video camera
    VideoCapture cap(0); // 0 corresponds to the default camera, change it if you have multiple cameras

    if (!cap.isOpened()) {
        cerr << "Error opening video capture." << endl;
        return -1;
    }

    int attendanceCount = 0;  // Variable to count attendance for the day

    while (true) {
        Mat frame;
        cap >> frame;

        if (frame.empty()) {
            cerr << "Error capturing frame." << endl;
            break;
        }

        // Convert the frame to grayscale for face detection
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        // Detect faces
        vector<Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));

        // If a face is detected, mark attendance and update the count
        if (!faces.empty()) {
            // Check if attendance is already marked for today
            time_t now = time(0);
            tm currentDate;
            localtime_s(&currentDate, &now);

            string logFile = "attendance_log.txt";
            ifstream log(logFile);
            string line;
            bool present = false;

            while (getline(log, line)) {
                size_t pos = line.find(to_string(currentDate.tm_mday) + "-" +
                    to_string(currentDate.tm_mon + 1) + "-" +
                    to_string(currentDate.tm_year + 1900));
                if (pos != string::npos) {
                    present = true;
                    break;
                }
            }

            log.close();

            // If attendance is not already marked for today, mark it and update the count
            if (!present) {
                ofstream log(logFile, ios::app);
                log << currentDate.tm_mday << "-" << currentDate.tm_mon + 1 << "-"
                    << currentDate.tm_year + 1900 << " " << ++attendanceCount << endl;
                log.close();

                cout << "Attendance marked for today. Count: " << attendanceCount << endl;
            }
        }

        // Draw rectangles around the detected faces
        for (const Rect& face : faces) {
            rectangle(frame, face, Scalar(0, 255, 0), 2);
        }

        // Display the result
        imshow("Attendance System", frame);

        // Break the loop if the user presses the 'ESC' key
        if (waitKey(1) == 27) {
            break;
        }
    }

    // Release the camera and close the OpenCV windows
    cap.release();
    destroyAllWindows();

    return 0;
}
