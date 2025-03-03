#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <sstream>
#include <unordered_map>

using namespace std;

struct Order {
	string dish_code;
	string dish_name;
	int quantity;
	double price;
	double subtotal;
	string order_date;
};

struct Dish {
	string dish_code;
	string dish_name;
	double quantity;
	double price;
	double sales;
};

typedef struct {
	int order_code;
	string order_date;
	string dish_code;
	int quantity_sold;
	double price;
	double subtotal;
	double prep_time;
}SALES;

SALES sales[200]; //assume max 200 orders per day

//function prototypes
void managerView(void);
void modifyMenu(void);
void deleteDish(void);
void addDish(void);
void displayStats(void);
void displayPrepTime(void);
void generateDailyOrderReport(void);
void calcAverageSales(void);
void displayDailySales(void);
void displayDailyDishStats(void);
void displayOverallDishSales(void);
void displayOrderCount(void);
void customerView(void);
void displayMenu(void);
void selectOrderItem(void);
void modifyOrderItem(void);
void browseOrder(void);
void recordStartTime(int orderCode);
void printInvoice(void);
int generateInvoiceNo(void);
void saveSales(void);
void chefView(void);
void changeOrderStatus(void);
void viewOrderStatus(void);
double calcTimeInterval(int orderCode, time_t endTime);
int getOrderCode(void);
void updateOrderCode(int newCode);
string getCurrentDate(void);

Dish menu;

//max number for orders, dishes and sales record
const int MAX_ORDERS = 200;
const int MAX_DISHES = 200;
const int MAX_SALES_RECORD = 200;

Dish dishSales[MAX_DISHES];
Dish dishes[MAX_DISHES];
SALES records[MAX_SALES_RECORD];

int main(void) {
	cout << "\t\t  -------------------------------Welcome to-------------------------------" << endl;
	cout << "\t\t  |                                                                      |" << endl;
	cout << "\t\t  |                Feast House Cafe Food Ordering System                 |" << endl;
	cout << "\t\t  |                                                                      |" << endl;
	cout << "\t\t  ------------------------------------------------------------------------" << endl;

	int option;
	cout << "\n\n\n\n\n\t\t\t\tAre you a manager, customer or chef?" << endl;
	cout << "\t\t\t\t------------------------------------------" << endl;
	cout << "\t\t\t\t|               1. Manager                |" << endl;
	cout << "\t\t\t\t|               2. Customer               |" << endl;
	cout << "\t\t\t\t|               3. Chef                   |" << endl;
	cout << "\t\t\t\t|               4. Exit                   |" << endl;
	cout << "\t\t\t\t------------------------------------------" << endl;
	cout << "\t\t\t\tEnter your choice (1-4): ";
	cin >> option;

	switch (option) {
	case 1:
		system("CLS");
		managerView();
		break;
	case 2:
		system("CLS");
		customerView();
		break;
	case 3:
		system("CLS");
		chefView();
		break;
	case 4:
		cout << "\n\n\t\t\t   .......................THANK YOU......................." << endl;
		return 0;
		break;
	default:
		system("CLS");
		cout << "Please select from the given options." << endl;
		main();
	}
	return 0;
}


/*
	Print out invoice
	Pre: All the files required are existed
		 Each line in "orders.txt" contains order details in the format: dish_code dish_name quantity price
		 All the required library are included
*/
void printInvoice(void) {
	system("CLS");
	ifstream in_file("orders.txt");
	if (!in_file) {
		cout << "Error opening file." << endl;
		return;
	}

	//formatting for invoice header
	cout << right << "\t\t\t\t\t\t" << "FEAST HOUSE CAFE - INVOICE" << endl;
	cout << right << setw(24) << setfill(' ') << "" << setw(1) << setfill(' ') << setw(77) << setfill('/') << " " << endl;
	int invoiceNo = generateInvoiceNo();
	int orderCode = getOrderCode();
	cout << "\t\t\tOrder Code: " << orderCode << endl;
	cout << "\t\t\tInvoice No: " << invoiceNo << endl;
	cout << "\t\t\tCashier: Admin" << endl;

	cout << right << setw(24) << setfill(' ') << "" << setw(1) << setfill(' ') << setw(77) << setfill('-') << " ";
	cout << right << "\t\t" << setw(1) << setfill(' ') << endl;

	cout << "\t\t\tDish Name\t\t\tQuantity\tUnit Price\tSubtotal(RM)\n";
	cout << right << setw(24) << setfill(' ') << "" << setw(1) << setfill(' ') << setw(77) << setfill('-') << " " << endl;
	double totalAmount = 0;

	Order order;
	string line;
	ofstream out_file("totalSales.txt", ios::app);
	if (!out_file)
		cout << "Erorr opening file." << endl;

	//read each line from orders.txt and process the order details
	while (in_file >> order.dish_code) {
		in_file >> ws; //skip whitespace
		getline(in_file, order.dish_name, '\t');
		in_file >> order.quantity >> order.price;  //read quantity and price

		order.subtotal = order.quantity * order.price;  //calculate subtotal for each dish
		totalAmount += order.subtotal;  //add to total amount

		cout << fixed << setprecision(2);
		cout << left;
		cout << "\t\t\t" << setw(20) << setfill(' ') << order.dish_name << "\t\t" << " " << order.quantity << "\t\t" << " " << order.price << "\t\t" << " " << order.subtotal << endl;
		in_file.ignore(numeric_limits<streamsize>::max(), '\n');  //ignore newline character and move to the next line
	}

	string currentDate = getCurrentDate();  //get current date
	out_file << currentDate << "\t" << totalAmount << endl;  //write order date and total amount into totalSales.txt to keep track of total sales amount in that day
	out_file.close();
	in_file.close();

	//formatting for invoice total and footer
	cout << right << setw(24) << setfill(' ') << "" << setw(1) << setfill(' ') << setw(77) << setfill('-') << " " << endl;
	cout << "\t\t\tTotal Amount: RM" << totalAmount << endl;
	cout << right << setw(24) << setfill(' ') << "" << setw(1) << setfill(' ') << setw(77) << setfill('-') << " " << endl;
	cout << right << setw(24) << setfill(' ') << "" << setw(1) << setfill(' ') << setw(77) << setfill('/') << " " << endl;

	ofstream dailyOrder_file("dailyOrder.txt", ios::app);

	if (!dailyOrder_file) {
		cout << "Error opening file." << endl;
		return;
	}

	dailyOrder_file << currentDate << "\t" << orderCode << endl;  //write current date and order code into dailyOrder.txt to keep track of number of orders in that day
	dailyOrder_file.close();

	cout << setfill(' '); //to disable the setfill('/')
	cout << "Press any key and hit enter to return to main menu..." << endl;

	char ans;
	cin >> ans;

	system("CLS");
	main();
}

//generate a random invoice no between range 10000 to 19999
int generateInvoiceNo() {
	//each time will generate different value
	srand(time(NULL));
	return rand() % 10000 + 10000;
}

/*
	Generate order code each time an order is placed
	Pre: All the files required are existed
*/
int getOrderCode() {
	ifstream in_file("order_code.txt");
	int orderCode = 0; //default order code

	// Read the order code from the file
	if (in_file.is_open()) {
		if (in_file.eof()) {
			in_file >> orderCode;  //write the default order code if the txt file is empty
			in_file.close();
		}
		else {
			in_file >> orderCode;
		}
		in_file.close();
	}
	return orderCode;
}

/*
	Updates the order code in the "order_code.txt" file
	Pre: all the files required are existed
*/
void updateOrderCode(int newCode) {
	ofstream codeFile("order_code.txt");

	if (codeFile.is_open()) {
		codeFile << newCode;  //write the new code into the txt file
		codeFile.close();
	}
	else
		cout << "Unable to update order code." << endl;
}


//show statistics related to prepratation time
void displayPrepTime() {
	cout << "Total Preparation Time" << endl;
	cout << "======================" << endl;
	ifstream in_file("prepTime.txt", ios::in);

	//initialization
	double totalPrepTime = 0;
	double dailyPrepTime = 0;
	double avrPrepTime = 0;
	int totalDays = 0;

	string line_1;
	string initialDate;
	if (getline(in_file, line_1)) {
		size_t pos = line_1.find("\t");
		if (pos != string::npos) {
			initialDate = line_1.substr(0, pos);
		}
	}
	in_file.clear();
	in_file.seekg(0, ios::beg);


	SALES record;
	//read records from prepTime.txt
	while (in_file >> record.order_date >> record.prep_time) {
		if (record.order_date != initialDate) {
			cout << initialDate << "\t" << dailyPrepTime << " min" << endl;
			totalDays++;
			dailyPrepTime = 0;
			initialDate = record.order_date;
		}
		dailyPrepTime += record.prep_time;
		totalPrepTime += record.prep_time;
	}

	if (!initialDate.empty()) {
		cout << initialDate << "\t" << dailyPrepTime << " min" << endl;
		totalDays++;
	}

	in_file.close();

	if (totalDays > 0)
		avrPrepTime = (double)totalPrepTime / totalDays;
	else
		avrPrepTime = 0;

	cout << "Average Daily Preperation Time" << endl;
	cout << "==============================" << endl;
	cout << "Average daily prep time: " << avrPrepTime << " min" << endl;

}

/*
	sequence of functions that allow customers to place orders, after that the time when the order is placed is recorded, the order code will be
	updated and an invoice will be printed out
*/
void customerView(void) {
	int orderCode = getOrderCode();
	selectOrderItem();
	recordStartTime(orderCode + 1);
	updateOrderCode(orderCode + 1);
	printInvoice();
}

/*
	Displays the manager view with several options provided
	Different actions are performed depending on the user's input
	user can choose to modify menu, display stats or exit to the main menu
	after each action, the user is prompted whether they want to perform another action
	Pre: All the functions in the option are defined
*/
void managerView() {
	int option;
	char option2;
	cout << "1. Modify Menu" << endl;
	cout << "2. Display Statistics" << endl;
	cout << "3. Exit to the main menu\n" << endl;
	cout << "Which action you wish to perform (1-3)?" << endl;
	cin >> option;

	switch (option) {
	case 1:
		system("CLS");
		modifyMenu();

		cout << "Do you still want to perform another action? (y/n)" << endl;
		cin >> option2;
		if (option2 == 'n' || option2 == 'N') {
			system("CLS");
			main();
		}
		else {
			system("CLS");
			managerView();
		}
		break;
	case 2:
		cout << endl;
		system("CLS");
		displayStats();
		cout << "Do you still want to perform another action? (y/n)" << endl;
		cin >> option2;
		if (option2 == 'n' || option2 == 'N') {
			system("CLS");
			main();
		}
		else {
			system("CLS");
			managerView();
		}
		break;
	case 3:
		system("CLS");
		main();
	default:
		system("CLS");
		cout << "Please select from the given options." << endl;
		managerView();
	}
}

/*
	show option for manager to add or delete dish
*/
void modifyMenu(void) {
	cout << "Do you wish to: " << endl;
	cout << "1. Add dish" << endl;
	cout << "2. Delete dish" << endl;
	int choice;
	cout << "Enter you choice (1-2): ";
	cin >> choice;

	switch (choice) {
	case 1:
		system("CLS");
		cout << "This is the current menu: " << endl;
		displayMenu();
		addDish();
		break;
	case 2:
		system("CLS");
		cout << "This is the current menu: " << endl;
		displayMenu();
		deleteDish();
		break;
	default:
		system("CLS");
		cout << "Invalid choice, please enter again!\n" << endl;
		managerView();
	}
}

/*
	Provides a menu of options for the chef to perform, such as changing order status or viewing order status
*/
void chefView(void) {
	cout << "Which action you want to perform?" << endl;
	cout << "1. Change Order Status" << endl;
	cout << "2. View Order Status" << endl;
	cout << "3. Exit to main menu" << endl;

	int ans;
	char option;
	cin >> ans;
	switch (ans) {
	case 1:
		changeOrderStatus();
		cout << "Do you still want to perform another action? (y/n)" << endl;
		cin >> option;
		if (option == 'n' || option == 'N') {
			system("CLS");
			main();
		}
		else {
			cout << endl;
			chefView();
		}
		break;
	case 2:
		viewOrderStatus();
		cout << "Do you still want to perform another action? (y/n)" << endl;
		cin >> option;
		if (option == 'n' || option == 'N') {
			system("CLS");
			main();
		}
		else {
			cout << endl;
			chefView();
		}
		break;
	case 3:
		system("CLS");
		main();
	default:
		system("CLS");
		cout << "Please select from the given options." << endl;
		chefView();
	}
}

/*
	Display various statistics including daily orders, daily dish sales,
	overall dish sales, average order count, total daily orders, preparation time,
	total daily sales and average daily sales
*/
void displayStats(void) {
	generateDailyOrderReport();
	displayDailyDishStats();

	cout << endl;
	displayOverallDishSales();
	cout << endl;
	cout << endl;

	displayOrderCount();
	cout << endl;
	cout << endl;

	displayPrepTime();
	cout << endl;
	cout << endl;

	calcAverageSales();
	displayDailySales();
	cout << "\n\n\n";
}

/*
	Displays menu items stored in the "menu.txt" file in a formatted table
	Pre: The file exists and contains valid menu data
		 The structure in the txt file is correct
*/
void displayMenu() {
	ifstream in_file("menu.txt");

	//formatting for heading
	cout << "DishCode" << "|\t" << "DishName" << "\t\t\t\t|" << right << setw(7) << fixed << setprecision(2) << "Price" << endl;
	cout << "--------|-----------------------------------------------|--------" << endl;

	//read the txt file and get menu 
	while (in_file >> menu.dish_code >> ws) {
		getline(in_file, menu.dish_name, '\t');
		in_file >> menu.price;
		cout << left << menu.dish_code << "\t|\t" << setw(30) << menu.dish_name << "\t\t|" << setw(20) << setfill(' ');
		cout << right << setw(7) << fixed << setprecision(2) << menu.price << endl;
	}
	in_file.close();
}

/*
	Adds a new dish to the menu stored in the "menu.txt" file
	If the entered dish code does not elready exist in the menu:
	- prompts the user to nter new dish name and price
	- append that information into the txt file
	- displays added successful message
	If the entered dish code already exist in the menu:
	- displays an error message, indicating that addition is unsuccessful
	Pre: the txt file exists and contains valid menu data

*/
void addDish() {
	ifstream in_file("menu.txt");
	ofstream out_file("temp_menu.txt");
	string newDishCode;
	bool codeExists = false;

	cout << "\nEnter new dish code: ";
	cin >> newDishCode;
	//change the first char to uppercase
	newDishCode[0] = toupper(newDishCode[0]);

	// Check if the new dish code already exists
	while (in_file >> menu.dish_code >> ws) {
		getline(in_file, menu.dish_name, '\t');
		in_file >> menu.price;
		if (menu.dish_code == newDishCode) {
			codeExists = true;
			break;
		}
		out_file << menu.dish_code << "\t" << menu.dish_name << "\t" << menu.price << endl;
		in_file.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the newline character from the buffer
	}

	in_file.close();
	out_file.close();

	// If the dish code already exists, display a message
	if (codeExists) {
		cout << "\n\nAddition unsuccessful. The dish code already exists." << endl;
	}
	else {
		//if the dish code is not existed, user can continue fill out the dish name and price per dish
		out_file.open("menu.txt", ios::app);
		cout << "Enter new dish name: ";
		cin.ignore(); // Clear input buffer
		getline(cin, menu.dish_name);
		if (!menu.dish_name.empty()) {
			menu.dish_name[0] = toupper(menu.dish_name[0]);
		}

		cout << "Enter new dish price: ";
		double price;
		string input;
		while (true) {
			getline(cin, input);
			stringstream ss(input);
			if (ss >> price && ss.eof()) {
				// Valid price entered
				menu.price = price;
				break;
			}
			cout << "Invalid input. Please enter a valid price." << endl;
		}
		out_file << newDishCode << "\t" << menu.dish_name << "\t" << menu.price << endl;
		out_file.close();
		//show a message if the dish is added successfully
		cout << "\n\nNew dish added successfully!" << endl;
	}
}

/*
	Deletes a dish from the "menu.txt" file
	If the entered dish code exists in the txt file:
	- deletes the selected dish from the txt file
	- show delete successful message
	If the entered dish code  does not exist in the txt file:
	- displays an error message indicating that the dish code was not found
	Pre: The txt file exists and contains valid menu data
*/
void deleteDish() {
	string dishCodeToDelete;
	cout << "\nEnter the dish code to delete: ";
	cin >> dishCodeToDelete;

	//change the first char to uppercase
	dishCodeToDelete[0] = toupper(dishCodeToDelete[0]);


	ifstream in_file("menu.txt");
	ofstream out_file("temp_menu.txt");

	bool dishDeleted = false;

	// Read each menu item from the original file
	while (in_file >> menu.dish_code >> ws) {
		getline(in_file, menu.dish_name, '\t');
		in_file >> menu.price;

		// Check if the current menu item matches the dish code to delete
		if (menu.dish_code == dishCodeToDelete) {
			dishDeleted = true; // Mark that a dish has been deleted
			continue; // Skip writing this dish to the new file
		}
		// Write the menu item to the new file
		out_file << menu.dish_code << "\t" << menu.dish_name << "\t" << menu.price << endl;
	}

	in_file.close();
	out_file.close();

	if (!dishDeleted) {
		cout << "\n\nDish code not found." << endl;
		remove("temp_menu.txt"); // Remove the temporary file if no dish is deleted
	}
	else {
		// Replace the original file with the temporary file
		remove("menu.txt");
		//rename the file back
		rename("temp_menu.txt", "menu.txt");
		//show deletion successful message
		cout << "\n\nDish deleted successfully!" << endl;
	}
}

/*
	Allows the user to select items from the menu and place an order
	Displays the menu to the user
	Prompt user to enter dish code, quantity to order
	Show added success if enter valid data, else show error message
	Prompts the user whether if they wish to modify their order
	Continue this process until user chooses to exit by entering 'Q' or 'q'
	Pre: the "menu.txt" file exists and contains valid menu data
*/
void selectOrderItem() {
	Order order;
	Dish menu;

	system("CLS");
	cout << "Ordering food...\n" << endl;

	string dishCode;
	bool codeExists = false;

	displayMenu();
	ofstream out_file("orders.txt");

	do {  //continue this loop as long as user did not press Q or q to exit ordering
		ifstream in_file("menu.txt");
		codeExists = false;
		cout << "\nEnter dish code (press Q to exit): ";
		cin >> dishCode;
		if (dishCode == "Q" || dishCode == "q") {   //exit the loop
			cout << endl;
			break;
		}
		else {
			//change the first character to uppercase
			dishCode[0] = toupper(dishCode[0]);
		}
		while (in_file >> menu.dish_code >> ws) {
			getline(in_file, menu.dish_name, '\t');
			in_file >> menu.price;
			if (menu.dish_code == dishCode) {
				codeExists = true;

				string currentDate = getCurrentDate();
				out_file << menu.dish_code << "\t" << menu.dish_name << "\t";

				int quantity;
				do {
					cout << "Enter quantity: ";
					cin >> quantity;
					if (quantity <= 0 || cin.fail())
					{
						cout << "Invalid quantity. Please try again.\n" << endl;  //print a message if user enter invalid quantity number
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}
					else {
						out_file << quantity << "\t";
						out_file << menu.price << endl;

						cout << "Added successfully!\n\n" << endl;
						break; // Exit the loop if valid quantity is entered
					}
				} while (true);
			}
			in_file.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		in_file.close();

		if (!codeExists)
			cout << "Dish code doesn't exist. Please try again!" << endl;  //show a message if user enter a not existed dish code
	} while (dishCode != "Q" && dishCode != "q");
	cout << endl;

	out_file.close();

	browseOrder();

	char ans;
	do {
		//ask if user wish to modify the order
		cout << "\nDo you want to modify your order?" << endl;
		cout << "Y: yes, i want to modify my order." << endl;
		cout << "N: no, proceed to checkout." << endl;

		cin >> ans;
		if (ans == 'n' || ans == 'N') {
			saveSales();
			return;
		}
		else if (ans == 'y' || ans == 'Y') {
			system("CLS");
			modifyOrderItem();
			saveSales();
		}
		else {
			cout << "Invalid choice. Please try again." << endl;
		}
	} while (ans != 'n' && ans != 'N' && ans != 'y' && ans != 'Y');
}

/*
	Allows user to modify items in the order
	Display current order, prompt the user which dish to modify
	If entered a valid dish code, prompts user the quantity to delete,
	else display error message
	Continue this process until the user chooses to exit by entering 'Q' or 'q'
	Pre: The "orders.txt" file exists and contains valid order data
*/
void modifyOrderItem() {
	Order order;
	string dishCodeToModify;
	int deleteQuantity;

	char choice;
	do {
		browseOrder();
		cout << "Enter dish code to modify (press Q to exit): ";
		cin >> dishCodeToModify;
		dishCodeToModify[0] = toupper(dishCodeToModify[0]);
		if (dishCodeToModify == "Q" || dishCodeToModify == "q")
			return;

		ifstream in_file("orders.txt");
		ofstream temp_file("temp.txt");

		bool found = false;
		string currentDate;
		string dishCode, dishName, line;
		int quantity;
		double price;
		while (getline(in_file, dishCode, '\t') && getline(in_file, dishName, '\t') && in_file >> quantity >> price) {
			if (dishCode == dishCodeToModify)
			{
				found = true;
				cout << "Enter quantity to delete (Enter the exact ordered amount to unselect the dish) *to add, use negative integer: ";
				cin >> deleteQuantity;
				if (cin.fail())
					cout << "Reenter quantity to delete: ";
				// Update quantity
				quantity -= deleteQuantity;
				if (quantity > 0)
					//only write to temp file if the quantity > 0
					temp_file << dishCode << "\t" << dishName << "\t" << quantity << "\t" << price << endl;
				cout << "Successfully Modified!" << endl;
			}
			else {
				temp_file << dishCode << "\t" << dishName << "\t" << quantity << "\t" << price << endl;
			}
			// Skip to the next line
			in_file.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		in_file.close();
		temp_file.close();

		// Rename temp file to orders.txt
		remove("orders.txt");
		rename("temp.txt", "orders.txt");

		if (!found)
			cout << "Dish code not found in the order." << endl;

		cout << "Do you want to continue modifying orders? (Y/N): ";
		cin >> choice;
	} while (toupper(choice) == 'Y');
	cout << "Order Placed! An invoice will be generated." << endl;
}
/*
	Displays the current order for browsing
	Pre: The "orders.txt" file exists and contains valid order data
*/
void browseOrder() {
	ifstream in_file("orders.txt");
	string line;
	if (!in_file) {
		cout << "Error opening file.";
		return;
	}
	else {
		cout << "This is your current order: " << endl;
		cout << "Dish Code\tDish Name\tQuantity\tPrice Per Dish" << endl;
		cout << "---------------------------------------------------------------" << endl;
		while (getline(in_file, line)) {
			if (line.empty()) {
				continue; // Skip empty lines
			}
			stringstream ss(line);
			string dish_code, dish_name, quantity_str, price_str;
			int quantity;
			double price;

			ss >> dish_code >> ws; // Read dish code
			getline(ss, dish_name, '\t'); // Read dish name
			ss >> quantity_str >> price_str; // Read quantity and price as strings
			quantity = stoi(quantity_str); // Convert string to integer
			price = stod(price_str); // Convert string to double

			cout << dish_code << "\t\t" << dish_name << "\t" << setw(10) << setfill(' ') << quantity << "\t\t" << fixed << setprecision(2) << price << endl; // Adjusted output formatting
		}
	}
}

/*
	Records the start time of an order identified by its order code
	Pre: ctime function is included to get the current time
		 The order code is provided as a parameter
*/
void recordStartTime(int orderCode) {
	ofstream out_file("order_times.txt", ios::app);
	if (!out_file) {
		cout << "Error opening file." << endl;
		return;
	}

	time_t currentTime = time(nullptr);
	out_file << orderCode << "\t" << currentTime << endl;
	out_file.close();
}

/*
	Calculates the time interval between the start time of an order and the end time
	Returns the time interval in minutes
	If the order code's start time is not found, returns -1 and error message
	Pre: The "order_times.txt" file exists and can be opened for reading
		 The difftime function is available to calculate time differences
*/
double calcTimeInterval(int orderCode, time_t endTime) {
	ifstream in_file("order_times.txt", ios::in);
	if (!in_file) {
		cout << "Error opening file for reading." << endl;
		return -1;
	}

	string line;
	int startCode;
	time_t startTime;
	bool found = false;

	// Find the start time corresponding to the given order code
	while (getline(in_file, line)) {
		istringstream iss(line);
		if (iss >> startCode >> startTime) {
			if (startCode == orderCode) {
				found = true;
				break;
			}
		}
	}
	in_file.close();

	//check if the start time was found
	if (!found) {
		cout << "Start time not found for order code: " << orderCode << endl;
		return -1;
	}

	// Calculate the time interval between start and end times
	double intervalSeconds = difftime(endTime, startTime);
	double intervalMinutes = intervalSeconds / 60.0; // Convert seconds to minutes
	return intervalMinutes;
}

/*
	Display the status of all orders, including processing and completed orders
	Pre: All files required are existed
		 Library fstream is included
*/
void viewOrderStatus() {
	ifstream in_file("order_times.txt", ios::in);

	if (!in_file) {
		cout << "Error opening file\n";
		return;
	}

	int orderCode;
	int count = 0;
	long long timestamp;

	//display processing orders
	cout << "Orders in processing:\n";
	while (in_file >> orderCode >> timestamp) {
		cout << setw(10) << orderCode;
		++count;
		if (count % 4 == 0)  //print 4 order code in a line
			cout << endl;
	}
	in_file.close();
	cout << endl;

	//open the file containing completed orders
	ifstream in_file2("completedOrder.txt", ios::in);
	if (!in_file2) {
		cout << "Error opening file\n";
		return;
	}

	string completedOrderCode;
	int count2 = 0;

	//display completed orders
	cout << "Completed orders:\n";
	while (in_file2 >> completedOrderCode) {
		cout << setw(17) << completedOrderCode;
		++count2;
		if (count2 % 4 == 0)  //print 4 order codes in a line
			cout << endl;
	}
	in_file2.close();
	cout << endl;
}



/*
	Allows the manager to change the status of an order from processing to completed
	Error message is displayed if the entered order code is invalid
	Manager if prompted to view all order stauts or exit
	Pre: All the txt file exist and working fine
*/
void changeOrderStatus() {
	int orderCode;
	time_t currentTime;
	bool hasOrders = false;
	int count = 0;

	// Read order codes from file
	ifstream in_file("order_times.txt", ios::in);
	if (!in_file) {
		cout << "Error opening file for reading." << endl;
		return;
	}

	//display available order codes
	cout << "These are the available order codes:\n";
	while (in_file >> orderCode >> currentTime) {
		hasOrders = true;
		cout << setw(10) << orderCode;
		count++;
		if (count % 4 == 0)  //show 4 processing order codes in one line
			cout << endl;
	}
	cout << endl;
	in_file.close();

	//check if there are any processing orders
	if (!hasOrders) {
		system("CLS");
		cout << "Currently no processing order. Please wait for customers to place their orders." << endl;
		return;
	}

	// Select which order to update
	int updateCode;
	char answer;
	bool isValidCode = false;

	do {
		isValidCode = false;    // Reset bool for each loop
		cout << "Which order you want to update: \n";
		cin >> updateCode;

		// Check if the entered code exists in the file
		ifstream check_file("order_times.txt");
		ofstream temp_file("temp.txt");
		bool found = false;
		time_t startTime;

		while (check_file >> orderCode >> startTime) {
			if (updateCode == orderCode) {
				time_t endTime = time(nullptr);
				double prepTime = calcTimeInterval(updateCode, endTime);
				cout << "Order status updated successfully!" << endl;
				found = true;

				// Write completed order to completedOrder.txt
				ofstream completed_file("completedOrder.txt", ios::app);
				completed_file << updateCode << "(" << prepTime << "min)" << endl;
				completed_file.close();

				// Write preparation time to prepTime.txt
				string date = getCurrentDate();
				ofstream out_file("prepTime.txt", ios::app);
				if (!out_file)
					cout << "Error opening outfile file\n";
				else
					out_file << date << "\t" << prepTime << endl;
			}
			else {
				// Write processing order to temp file
				temp_file << orderCode << "\t" << startTime << endl;
			}
		}
		check_file.close();
		temp_file.close();

		if (!found)
			cout << "Invalid order code. Please try again.\n";
		else {
			// Replace the original file with the temp file
			remove("order_times.txt");
			rename("temp.txt", "order_times.txt");
			isValidCode = true;

			cout << "Do you still want to update another order code?(y/n):\n";
			cin >> answer;
		}
	} while (!isValidCode || answer == 'y' || answer == 'Y');

	// Prompt for viewing all order status or exiting
	int a = 1; //act as a flag
	while (a == 1)
	{
		char ans;
		cout << "View all order status? (y/n):\n";
		cin >> ans;
		if (ans == 'y' || ans == 'Y') {
			viewOrderStatus();
			a = 0;
		}
		else if (ans == 'n' || ans == 'N') {
			cout << "Thanks for using the system!" << endl;
			a = 0;
		}
		else {
			cout << "Invalid input." << endl;
			a = 1;
		}
	}
}


/*
	Generates a daily order report by calculating the toal quantity sold for each dish
	Pre: All the txt files exist and can be opened for reading
*/
void generateDailyOrderReport(void) {
	int numRecords = 0;
	int numDishes = 0;


	// Read menu.txt data
	ifstream menuFile("menu.txt");
	if (!menuFile) {
		cout << "Error opening file" << endl;
		return;
	}
	while (numDishes < MAX_DISHES && menuFile >> dishes[numDishes].dish_code >> ws) {
		getline(menuFile, dishes[numDishes].dish_name, '\t');
		menuFile >> dishes[numDishes].price;
		numDishes++;
	}
	menuFile.close();

	// Read SalesRecords.txt data
	ifstream salesFile("salesRecord.txt");
	if (!salesFile) {
		cout << "Error opening file" << endl;
		return;
	}
	while (numRecords < MAX_SALES_RECORD && salesFile >> records[numRecords].order_date >> records[numRecords].dish_code >> records[numRecords].quantity_sold >> records[numRecords].price) {
		numRecords++; // Increment the number of records read
	}
	salesFile.close();

	// Open dailyDishStats.txt for writing
	ofstream outFile("dailyDishStats.txt");
	if (!outFile) {
		cout << "Error opening file" << endl;
		return;
	}

	// Iterate through sales records and aggregate quantities for each dish on each date
	string currentDate = ""; // Variable to store the current date
	for (int i = 0; i < numRecords; ++i) {
		if (i == 0 || records[i].order_date != records[i - 1].order_date) {
			// Start of a new date, reset dish quantities
			int dishQuantities[MAX_DISHES] = { 0 }; // Initialize dish quantities to 0
			for (int j = 0; j < numDishes; ++j) {
				outFile << records[i].order_date << "\t" << dishes[j].dish_code << "\t";
				bool foundRecord = false;
				for (int k = i; k < numRecords; ++k) {
					if (records[k].order_date == records[i].order_date && records[k].dish_code == dishes[j].dish_code) {
						dishQuantities[j] += records[k].quantity_sold;
						foundRecord = true;
					}
				}
				if (foundRecord) {
					outFile << dishQuantities[j] << "\tRM " << dishQuantities[j] * dishes[j].price << endl;
				}
				else {
					outFile << "0\tRM  0.00" << endl;
				}
			}
			// Update the current date
			currentDate = records[i].order_date;
		}
	}

	// Close the output file
	outFile.close();
}



/*
	Retrive the current system date and returns in a string format "dd/mm/yyyy"
	use for marking each sales's / order's transaction date
*/
string getCurrentDate() {
	// Get current system time
	auto now = chrono::system_clock::now();

	// Convert to time_t (seconds since epoch)
	time_t now_time_t = chrono::system_clock::to_time_t(now);

	// Convert time_t to struct tm
	tm now_tm = *localtime(&now_time_t);

	// Extract day, month, and year
	int day = now_tm.tm_mday;
	int month = now_tm.tm_mon + 1;  // tm_mon is 0-based
	int year = now_tm.tm_year + 1900; // tm_year is years since 1900

	// Create a string representing the date in "dd/mm/yyyy" format
	string currentDate = to_string(day) + "/" + to_string(month) + "/" + to_string(year);

	return currentDate;
}

/*
	display the total number of orders for each day and calculate daily order count
	the result is displayed on the screen
*/
void displayOrderCount(void) {
	//print heading
	cout << "Total Order" << endl;
	cout << "===========" << endl;

	ifstream in_file("dailyOrder.txt");

	//initialization
	int dailyOrder = 0;
	int totalOrders = 0;
	double avrOrderCount = 0;
	int totalDays = 0;

	//read the first line to determine the initial date
	string line_1;
	string initialDate;
	if (getline(in_file, line_1)) {
		size_t pos = line_1.find("\t");
		if (pos != string::npos) {
			initialDate = line_1.substr(0, pos);
		}
	}
	// Reset file flags and set the file pointer to the beginning of the file
	in_file.clear();
	in_file.seekg(0, ios::beg);

	SALES record;

	//read sales records from the file
	while (in_file >> record.order_date >> record.order_code) {
		//check if the order date changes
		if (record.order_date != initialDate) {
			//print daily order count
			cout << initialDate << "\t" << dailyOrder << " orders" << endl;
			totalDays++;
			dailyOrder = 0;
			initialDate = record.order_date;
		}
		dailyOrder++;
		totalOrders++; // Increment total orders
	}

	if (!initialDate.empty()) {
		cout << initialDate << "\t" << dailyOrder << " orders" << endl;
		totalDays++;
	}
	in_file.close();

	//calculate and display average daily order count
	if (totalDays > 0)
		avrOrderCount = (double)totalOrders / totalDays;
	else
		avrOrderCount = 0;

	//print average daily order count
	cout << "Average Daily Order" << endl;
	cout << "===================" << endl;
	cout << "Average daily order: " << avrOrderCount << " orders" << endl;
}

/*
	calculates the average daily total sales based on the data stored in the "totalSales.txt"
	for each day, it calculates the total sales by summing up the subtotals of all orders made on that day.
	it then writes the date and the corresponding total sales for each day into the "dailyTotalSales.txt" file.
	after processing all the sales records, it computes the average daily total sales by dividing the total sales by the number of days.
	the result is displayed on the screen
*/
void calcAverageSales() {
	double dailySales = 0.0;
	double totalSales = 0.0;
	int totalDays = 0;

	//declare a struct to store sales record
	SALES record;

	ifstream sales_file("totalSales.txt");
	ofstream daily_file("dailyTotalSales.txt", ios::trunc);
	if (!sales_file) {
		cout << "Error opening file" << endl;
		return;
	}

	// Read the initial date from the first line of the file
	string line_1;
	string initialDate;
	if (getline(sales_file, line_1)) {
		size_t pos = line_1.find("\t");
		if (pos != string::npos) {
			initialDate = line_1.substr(0, pos);
		}
	}
	sales_file.clear();
	sales_file.seekg(0, ios::beg);

	// Read sales records from the file
	while (sales_file >> record.order_date >> record.subtotal) {
		// Check if the order date changes
		if (record.order_date != initialDate) {
			daily_file << initialDate << "\t" << dailySales << endl;   // Write daily total sales to the output file
			totalDays++;   // Increment total days
			totalSales += dailySales; // Add daily sales to total sales
			dailySales = 0.0; // Reset daily sales
			initialDate = record.order_date;   //update initial date
		}
		dailySales += record.subtotal;
	}
	// Write the average for the last day
	daily_file << initialDate << "\t" << dailySales << endl;
	totalDays++;   // Increment total days for the last day
	totalSales += dailySales; // Add daily sales to total sales for the last day

	sales_file.close();
	daily_file.close();

	// Calculate average daily total sales
	double avrSales = totalSales / totalDays;
	cout << "Average Daily Total Sales" << endl;
	cout << "=========================" << endl;
	cout << "Average daily total sales: RM" << fixed << setprecision(2) << avrSales << endl;
}

/*
	display the daily total sales.
	It opens the "dailyTotalSales.txt" file for reading and reads each line, which contains the date and the corresponding total sales.
	For each line, it outputs the date and total sales to the console, formatted with proper alignment and precision.
*/
void displayDailySales() {
	cout << "Total Sales" << endl;
	cout << "===========" << endl;
	ifstream in_file("dailyTotalSales.txt");
	string date;
	double totalSales;
	while (in_file >> date >> totalSales) {
		cout << setw(12) << left << date << " RM" << fixed << setprecision(2) << totalSales << endl;
	}
	in_file.close();
}

/*
	display the daily dish sales
	It opens the "dailyDishStats.txt" file for reading and reads each line.
	For each line, it extracts the date, dish code, quantity sold, and sales information.
	Then, it prints this information to the console in a formatted manner, with proper alignment and precision.
*/
void displayDailyDishStats(void) {
	cout << "Daily Dish Sales" << endl;
	cout << "================" << endl;
	cout << "Date      Dish Code     Quantity        Sales" << endl;
	cout << "---------------------------------------------------" << endl;
	ifstream in_file("dailyDishStats.txt");
	if (!in_file) {
		cout << "Error opening file" << endl;
		return;
	}

	// Read each line in the file and display the daily dish sales
	string line;

	string prevDate, orderDate, dishCode, currency;
	int quantity;
	double sales;
	bool firstLine = true;

	while (getline(in_file, line)) {
		// Extract data from the line using string stream
		istringstream iss(line);
		iss >> orderDate >> dishCode >> quantity;

		// Skip the "RM" part
		iss >> currency;

		// Read the sales as a double
		iss >> sales;

		// Print the data with left alignment
		if (orderDate != prevDate)
		{
			cout << left << setw(12) << orderDate << " ";
			cout << setw(12) << dishCode << " ";
			cout << setw(10) << quantity << " ";
			cout << "RM " << fixed << setprecision(2) << sales << endl;
			prevDate = orderDate;
			firstLine = true; // Reset the flag for the next date
		}
		else
		{
			cout << setw(13) << ""; // Skip printing date,// dishCode, quantity
			cout << setw(12) << dishCode << " ";
			cout << setw(10) << quantity << " ";
			cout << "RM " << fixed << setprecision(2) << sales << endl;
			firstLine = false;
		}
	}

	in_file.close();
}

/*
calculates and displays the overall dish sales
It reads the daily dish sales data from the "dailyDishStats.txt" file.
For each dish code, it accumulates the total quantity sold and total sales across all days.
Then, it calculates the average quantity and sales per day for each dish code.
Finally, it prints the overall quantity and sales per day for each dish code to the console in a formatted manner.
The function ensures that the quantity and sales are divided by the total number of days to get the average per day.
It also closes the input file after reading the data.
*/
void displayOverallDishSales(void) {
	ifstream in_file("dailyDishStats.txt");

	//read the first line to determine the initial date
	string line_1;
	string initialDate;
	if (getline(in_file, line_1)) {
		// Find the position of the first tab character to extract the date
		size_t pos = line_1.find("\t");
		if (pos != string::npos) {
			initialDate = line_1.substr(0, pos);
		}
	}

	//reset file flags and set the file pointer to the beginning of the file
	in_file.clear();
	in_file.seekg(0, ios::beg);

	//initialize the day counter
	int day = 1;

	int numDishes = 0;

	//read each line in the file
	string line;
	while (getline(in_file, line)) {
		istringstream iss(line);
		string orderDate, dishCode;
		int quantity;
		double sales;
		iss >> orderDate >> dishCode >> quantity;

		// Skip the "RM" part
		string currency;
		iss >> currency;

		// Read the sales as a double
		iss >> sales;

		//check if the order date differs from the initial date
		if (orderDate != initialDate) {
			//increment the day counter and update the initial date
			day++;
			initialDate = orderDate;
		}

		//check if the dish code already exists in the array
		bool found = false;
		for (int i = 0; i < numDishes; ++i) {
			if (dishSales[i].dish_code == dishCode) {
				//update the quantity and sales of the existing dish
				dishSales[i].quantity += quantity;
				dishSales[i].sales += sales;
				found = true;
				break;
			}
		}

		// If the dish code is not found, add it to the array
		if (!found) {
			dishSales[numDishes].dish_code = dishCode;
			dishSales[numDishes].quantity = quantity;
			dishSales[numDishes].sales = sales;
			numDishes++;
		}
	}
	in_file.close();

	// Calculate overall quantity and sales per day
	for (int i = 0; i < numDishes; ++i) {
		dishSales[i].quantity /= day;
		dishSales[i].sales /= day;
	}

	// Print the overall quantity and sales per day for each dish code
	cout << endl;
	cout << "Daily Average Dish Sales" << endl;
	cout << "========================" << endl;
	cout << "Dish Code      Quantity          Sales" << endl;
	cout << "-----------------------------------------" << endl;
	for (int i = 0; i < numDishes; ++i) {
		cout << dishSales[i].dish_code << "\t\t" << dishSales[i].quantity << "\t\tRM " << fixed << setprecision(2) << dishSales[i].sales << endl;
	}
}

/*
	Saves orders item in orders.txt into salesRecord.txt
*/
void saveSales(void) {
	ifstream in_file("orders.txt");
	ofstream out_file("salesRecord.txt", ios::app);

	if (!in_file || !out_file) {
		cout << "Error opening files." << endl;
		return;
	}

	//declare to store order details
	string dishCode, dishName;
	int quantity;
	double price;
	//get current date
	string currentDate = getCurrentDate();

	//read orders from orders.txt and save them to salesRecord.txt
	while (in_file >> dishCode >> ws && getline(in_file, dishName, '\t') >> quantity >> price) {
		out_file << currentDate << "\t" << dishCode << "\t" << quantity << "\t" << price << endl;
	}

	in_file.close();
	out_file.close();
}