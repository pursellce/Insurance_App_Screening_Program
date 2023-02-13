#ifndef RECORDS_H
#define RECORDS_H
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stdlib.h>

using namespace std;

class record{
	public:
		int ID = 0; 					//submission ID number assigned by program
		string agency = "None"; 		//Agency name
		string insured = "None";		//named insured
		int limit = 0;					//policy limit
		string state = "XX";			//state in which applicant is based
		int locationNum = 0;			//number of locations
		int sicCodeInt = 0;				//standard industry code as integer value
		string sicCodeString = "0000";	//standard industry code as string value
		string flag = "tbd";			//reject/accept/underwriter review/incomplete
};


class records{
	public:
		deque<record> dqrecords;			//deque to hold records during processing
		vector<record> incomplete;			//vector to hold incomplete records
		vector<record> reject;				//vector to hold auto reject records
		vector<record> uwReview;			//vector to hold records needing underwriter review
		vector<record> accept;				//vector to hold auto accept records

		//Functions for testing
		void printVectors();				//testing
		
		//functions to process the records and set appropriate flags
		void assignID (); 					//function to assign submission ID number to application for internal records
		void checkComplete (); 				//function to make sure all fields within application have been submitted
		void checkPolicyLimit();			//function to check policy limit field
		void checkState();					//function to check state fields
		void checkLocationNum();			//function to check number of locations field
		void checkSICCode();				//function to check SIC code field
		
		//functions to sort and print records after processing
		void moveRecords ();								//Function to move records to appropriate vector after processing complete
		void heapify(vector<record>& vector, int n, int i);	//Helper function for sorting function
		void heapsortRecords (vector<record>& vector);		//Function using heap sort to sort records from highest policy limit to lowest
		void printReject ();								//function to print report of all automatically rejected&incomplete applications
		void printUWReview ();								//function to print report of all applications to be sent for underwriter review
		void printAccept ();								//function to print report of all automatically accepted applications
		

};
#endif