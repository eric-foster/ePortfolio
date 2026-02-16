//============================================================================
// Name        : Project Two
// Author      : Eric Foster
// Version     : 1.0
// Description: Final project for CS300, utilizing a Binary Search Tree Data Structure implementation.
//============================================================================


#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <time.h>

using namespace std;

struct Course {
    string courseId;
    string title;
    vector <string> prereq;

    Course() {
        this->courseId = "";
        this->title = "";
        this->prereq = {};
    }
};
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);


public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseId);

    void DestroyBST(Node* node);
    int size;
};

BinarySearchTree::BinarySearchTree() {

    //root is equal to nullptr
    root = nullptr;
    size = 0;
}

void BinarySearchTree::DestroyBST(Node* node)
{
    if (node != nullptr) {
        DestroyBST(node->right);
        DestroyBST(node->left);
        delete node;
    }
}

BinarySearchTree::~BinarySearchTree() {
    // recursively destory all nodes
    DestroyBST(root);

}

void BinarySearchTree::Insert(Course course) {

    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(course);

        // increment size
        size++;
    }

    // else
    else {
        // add Node root and bid
        this->addNode(root, course);

        // increment size
        size++;
    }

}

void BinarySearchTree::addNode(Node* node, Course course) {
    /* Iterate through tree algorithm*/ // Saves memory and faster than recursion when sifting through larger data sets
    Node* curr = node;

    while (curr != nullptr) {
        // left subtree
        if (course.courseId.compare(curr->course.courseId) < 0) {
            if (curr->left == nullptr) {
                curr->left = new Node(course);
                curr = nullptr;
            }
            else {
                curr = curr->left;
            }
        }
        // right subtree
        else {
            if (curr->right == nullptr) {
                curr->right = new Node(course);
                curr = nullptr;
            }
            else {
                curr = curr->right;
            }
        }
    }


}

void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    inOrder(root);
}

void BinarySearchTree::inOrder(Node* node) {
    int itr = 0;
    if (node != nullptr) {
        //InOrder left
        inOrder(node->left);
        //output courseId, title, and prerequisites 
        cout << node->course.courseId << ": " << node->course.title << " | ";
        if (node->course.prereq.empty()) {
            cout << "No prerequisites" << endl;
        }
        else {
            for (auto i = node->course.prereq.begin(); i < node->course.prereq.end(); i++) {
                cout << node->course.prereq[itr] << " ";
                itr++;
            }
            cout << endl;
        }
        //InOder right
        inOrder(node->right);
    }
}

Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* curr = root;
    // keep looping downwards until bottom reached or matching bidId found
    while (curr != nullptr) {
        // if match found, return current bid
        if (curr->course.courseId.compare(courseId) == 0) return curr->course;
        // if bid is smaller than current node then traverse left
        else if (courseId.compare(curr->course.courseId) < 0) {
            curr = curr->left;
        }
        else {
            // else larger so traverse right
            curr = curr->right;
        }

    }
    Course course;
    return course;
}

void parseFile(string fileName, BinarySearchTree* bst)
{
    Course course;
    // loading course into bst
    ifstream inputFile;
    inputFile.open(fileName);

    if (!inputFile.is_open()) {
        cout << "ERROR: File could not be opened. Try adding to CWD." << endl;

    }
    else {
        string line = "";
        while (getline(inputFile, line)) {
            // Declare local variables to store information for the course
            string courseId = "";
            string title = "";
            string temp;
            vector <string> prereq;

            stringstream inputString(line);
            getline(inputString, courseId, ',');
            course.courseId = courseId;
            getline(inputString, title, ',');
            course.title = title;

            if (courseId.compare("") == 0 || title.compare("") == 0) {
                cout << endl << line << " imporper format less than two values. Not added to data structure." << endl << endl;
                continue;
            }
            // Get rest of the line to the EOL
            while (getline(inputString, temp, '\n')) {
                // Make the stream only concern the prerequisites 
                stringstream ss(temp);
                // Seperate temp by the ',' delimiter
                while (getline(ss, temp, ',')) {
                    // add it to prereq vector
                    course.prereq.push_back(temp);
                }

            }


            // Add the course to bst
            bst->Insert(course);

            //reset variables
            line = "";
            course.prereq.clear();
        }
    }
    inputFile.close();
}

void displayCourse(Course course) {
    int itr = 0;
    cout << course.courseId << ", " << course.title << endl;
    cout << "Prerequisites: ";
    if (course.prereq.empty()) {
        cout << "No prerequisites";
    }
    else {
        for (auto i = course.prereq.begin(); i < course.prereq.end(); i++) {
            cout << course.prereq[itr] << " ";
            itr++;
        }
    }
    cout << endl << endl;
}

void displayMenu(int argc, char* argv, BinarySearchTree* bst, Course course)
{
    // Set exception mask for cin stream
    cin.exceptions(ios::failbit);
    clock_t ticks;
    string csvPath, courseId;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
    }


    cout << "Welcome to Course Planner." << endl;
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Print Schedule" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter selection: ";

        // Verifies if input is a char or string - which is not valid 
        try {
            cin >> choice;
            // checking if input is valid
            if ((choice < 0 || choice > 3) && choice != 9) {
                cout << choice << " is not a valid menu option try again." << endl;
            }

            switch (choice) {
            case 1:

                // Initialize a timer variable before loading bids
                ticks = clock();

                // Complete the method call to load the courses
                parseFile(csvPath, bst);

                cout << bst->size << " courses read" << endl;

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;

            case 2:
                cout << "Here is a sample schedule: " << endl << endl;
                bst->InOrder();
                break;

            case 3:
                string input;
                ticks = clock();
                cout << "What course do you want to know about?" << endl;
                cin >> input;

                courseId = input;
                // converting lowercase values to uppercase 
                for (int i = 0; i < courseId.length(); i++) {
                    courseId[i] = toupper(courseId[i]);
                }
                course = bst->Search(courseId);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                if (!course.courseId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "Course Id " << courseId << " not found." << endl;
                }

                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;

            }
        }
        catch (ios_base::failure& e) {
            cout << "Not a valid input. Try a number 1-3 or 9" << endl;
            cin.clear();
            cin.ignore(1234, '\n');
        }
    }
    cout << endl << "Thank you for using the course planner. See you soon." << endl;
}

int main(int argc, char* argv) {
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    // process command line arguments
    displayMenu(argc, argv, bst, course);
    return 0;
}