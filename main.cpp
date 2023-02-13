/*
Author: Connor Pursell
Date: 5/7/22
*/

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "records.h"

using namespace std;

int main (){

//load in file and store in records
	records Records;
	record rtemp;
	vector<string> row;
	string line, cell, temp;
	ifstream fin;
	
	
	fin.open("Input.csv");
	if(fin.fail()){
		cout << "Failed to find file\n";
		exit(1);
	}
	else{
		cout<<"File read in successfully\n";
	}
	
	do{
        row.clear();
		getline(fin, line);
		stringstream s(line);
		while (getline(s, cell, ',')) {
            row.push_back(cell);
        }
		
		rtemp.agency = row[0];
		rtemp.insured = row[1];
		stringstream l(row[2]);
		l >> rtemp.limit;		
		rtemp.state = row[3];
		stringstream ln(row[4]);
		ln >> rtemp.locationNum;
		stringstream ss(row[5]);		
		ss >> rtemp.sicCodeInt;
		rtemp.sicCodeString = row[5];
		Records.dqrecords.push_back(rtemp);
 
	}while (!fin.eof());
	
	Records.assignID();
	cout<<"Submission IDs assigned successfully.\n";
	Records.checkComplete();
	cout<<"Check for completion successful\n";
	Records.checkPolicyLimit();
	cout<<"Check of policy limits successful\n";	
	Records.checkState();
	cout<<"Check of states successful\n";
	Records.checkLocationNum();
	cout<<"Check of number of locations successful\n";
	Records.checkSICCode();
	cout<<"Check of SIC codes successful\n";
	
	Records.moveRecords();	
	Records.heapsortRecords(Records.accept);
	Records.heapsortRecords(Records.uwReview);
	Records.printVectors();
	Records.printReject();
	Records.printUWReview();
	Records.printAccept();
	cout<<"Files for Auto accepted, auto rejected/incomplete, and underwriter review applications created\n";
		
	
	return 0;
}
	
	
	

