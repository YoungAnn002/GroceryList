#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <list>
#include <sstream>

#include "split.h"

using namespace std;

vector<string> Items;
vector<string> Customers;

class Payment{
  protected:
    double ammount;

  public:
    virtual ~Payment() {  };
    virtual string print_detail() const = 0;
};
class Credit : public Payment {
  string card_number;
  string experation;
  
  public:
    Credit (string card_number, string experation){
      this->card_number = card_number;
      this->experation = experation;
    };

    string print_detail() const{
      string all;
      all = "Paid by credit card " + card_number + ", exp. " + experation + "\n";
      return all;
    };
};
class PayPal : public Payment{
  string paypal_id;
  
public:
    PayPal (string paypal_id){
      this->paypal_id = paypal_id;
    };
    string print_detail() const{
      string all;
      all = "Paid by Paypal ID: " + paypal_id + "\n";
      return all;
    };
};
class WireTransfer : public Payment{
  string bank_id;
  string account_id;

  public:
  WireTransfer (string bank_id, string account_id){
    this->bank_id = bank_id;
    this->account_id = account_id;
  };
    string print_detail() const{
      string all;
      all = "Paid by Wire transfer from Bank ID " + bank_id + ", Account# " + account_id + "\n";
      return all;
    };
};


class Item{
  int item_id;
  string description;
  double price;

  public:
    int findItem(int itemID) {
  	 int index = 0;
  	
  	 for (int i = 0; i < Items.size(); i++) {
  		  if (Items.at(i) == to_string(itemID)) {
  			 index = i;
  			 return index;
  		  }
  	 }	
  	 if (itemID == 0) {
  		  return 0;
  	 }
  	 return -1;
    };
};
class Customer{
  int cust_id;
  string name;
  string street;
  string city;
  string state;
  string zip;
  string phone;
  string email;

  public:
  string print_detail(){
    string all;
    all = "Customer ID " + to_string(cust_id) + ": \n";
    all += name + ", ph. " + phone + ", email: " + email + "\n";
    all += street + "\n";
    all += city + ", " + state + " " + zip + "\n";
    return all;
  };
  void ReadInCustomer(int index) {
		cust_id = stoi(Customers.at(index));
		name = Customers.at(index + 1);
		street = Customers.at(index + 2);
		city = Customers.at(index + 3);
		state = Customers.at(index + 4);
		zip = Customers.at(index + 5);
		phone = Customers.at(index + 6);
		email = Customers.at(index + 7);
	};
  int findCustomer(int cusNum) {
  	int index = 0;
  	
  	for (int i = 0; i < Customers.size(); i++) {
  		if (Customers.at(i) == to_string(cusNum)) {
  			index = i;
  			return index;
  		}
  	}
  	return -1;
  };
};
class LineItem{
  int item_id;
  int qty;
  public:
    LineItem(string Order){
      string temp;
  		temp = Order;
  		int i = temp.find('-');
  		item_id = stoi(temp.substr(0, 5));
  		qty = stoi(temp.substr(6, 1));
    };
    double sub_total(){
      Item currItem;
      int index;
      double subTotal = 0.0;

      index = currItem.findItem(item_id) + 2;
      for (int i = 0; i < qty; i++){
        subTotal += stod(Items.at(index));
      }
      return subTotal;
    };
    string print_items(){
      Item currItem;
      int index;
      string all;
      index = currItem.findItem(item_id);
      all = "Item " + to_string(item_id) + ": " + "\"" + Items.at(index + 1) + "\", " + to_string(qty) + " @ " + Items.at(index + 2) + "\n";
      return all;
    };
    friend bool operator<(const LineItem& item1, const LineItem& item2) {
      return item1.item_id < item2.item_id;
    };
};
class Order{
  int order_id;
  string order_date;
  int cust_id;
  vector<LineItem> line_items;
  Payment* payment;
  public:
    Order(int order_id, string order_date, int cust_id, vector<LineItem> line_items, Payment* payment){
      this->order_id = order_id;
      this->order_date = order_date;
      this->cust_id = cust_id;
      this->line_items = line_items;
      this->payment = payment;
    };
    double total(){
      double finalTotal = 0;
      for (int i = 0; i < line_items.size(); i++){
        finalTotal += line_items.at(i).sub_total();
      }
      return finalTotal;
    };
    string print_order(){
      string all;
      stringstream temp;
      string temp2;
      temp << fixed << setprecision(2) << total();
      temp2 = temp.str();
      all = "======================\n";
      all += "Order #" + to_string(order_id) + ", Date: " + order_date + "\n";
      all += "Ammount: $" + temp2 + ", ";
      all += payment->print_detail();
      all += "\n";
      
      Customer thisCustomer;
      int find;
      find = thisCustomer.findCustomer(cust_id);
      thisCustomer.ReadInCustomer(find);
      all += thisCustomer.print_detail();
      
      sort(line_items.begin(), line_items.end());

      all += "Order Detail:\n";
      for (int i = 0; i < line_items.size(); i++){
        all += line_items.at(i).print_items();
      }
      return all;
    };
};

list<Order> orders;

void read_customers(string customerFile){
  string hold;
	ifstream inFS;
	vector<string> customerInfo;
	Customer firstCustomer;

	inFS.open(customerFile);
	if (!inFS.is_open()) {
		cout << "you messed up" << endl;
		return;
  }

  while (!inFS.eof()) {
		getline(inFS, hold);

		customerInfo = split(hold, ',');
		
		for (int i = 0; i < customerInfo.size(); i++) {
			Customers.push_back(customerInfo.at(i));
		}
	}
  inFS.close();
}

void read_items(string itemsFile){
  string hold;
	ifstream inFS;
	vector<string> itemsInfo;

	inFS.open(itemsFile);
	if (!inFS.is_open()) {
		cout << "you messed up" << endl;
		return;
	}

	while (!inFS.eof()) {
		getline(inFS, hold);

		itemsInfo = split(hold, ',');

		for (int i = 0; i < itemsInfo.size(); i++) {
			Items.push_back(itemsInfo.at(i));
		}
  
  }
  inFS.close();
}
void read_orders(string orderFile){
  string hold;
  ifstream inFS;
  int index = 0;
  int find;
  vector<string> orderInfo;
  

  Customer currCustomer;
  vector<LineItem> currItems;
  //temp
  int orderNum = 1;
  string date;
  int payType;
  int custID;

  inFS.open(orderFile);
	if (!inFS.is_open()) {
		cout << "you messed up" << endl;
		return;
	}

  getline(inFS, hold);

  while (!inFS.eof()) {
    
		orderInfo = split(hold, ',');

    find = currCustomer.findCustomer(stoi(orderInfo.at(index)));
    currCustomer.ReadInCustomer(find);
    custID = stoi(orderInfo.at(index));

    orderNum = stoi(orderInfo.at(index + 1));
    date = orderInfo.at(index + 2);
    
    for (int i = 0; i < orderInfo.size()-3; i++){
      currItems.push_back(orderInfo.at(i + 3));
    }
    //Payment
    index = 0;
    getline(inFS, hold);
		orderInfo = split(hold, ',');
    payType = stoi(orderInfo.at(index));
    
    if(payType == 1){
      string cardNum = orderInfo.at(index + 1);
      string experation = orderInfo.at(index + 2);

      Credit* card = new Credit(cardNum, experation);
      Order currOrder(orderNum, date, custID, currItems, card);
      orders.push_back(currOrder);
      
    }
    else if(payType == 2){
      string payPalID = orderInfo.at(index + 1);
      
      PayPal* card = new PayPal(payPalID);
      Order currOrder(orderNum, date, custID, currItems, card);
      orders.push_back(currOrder);
    }
    else{
      string bankID = orderInfo.at(index + 1);
      string accountID = orderInfo.at(index + 2);
      
      WireTransfer* card = new WireTransfer(bankID, accountID);
      Order currOrder(orderNum, date, custID, currItems, card);
      orders.push_back(currOrder);
      
    }

    currItems.clear();
    getline(inFS, hold);
  }
  

  inFS.close();
}

int main() {
  read_customers("customers.txt");
  read_items("items.txt");
  read_orders("orders.txt");
  

  ofstream ofs("order_report.txt");
  for (auto& order: orders)
  ofs << order.print_order() << endl;

  return 0;
}