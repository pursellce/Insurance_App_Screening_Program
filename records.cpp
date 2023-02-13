#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include "records.h"

using namespace std;

//PRINT FUNCTION
void records::printVectors(){
	if(incomplete.size() != 0){
		cout<<"\nIncomplete Records:\n";
		for(int i = 0; i<incomplete.size(); i++){
			cout<<"Submission ID: " <<incomplete[i].ID<<" - "<<"Agency Name: "<<incomplete[i].agency<<" - Named Insured: ";
			cout<<incomplete[i].insured<<" - Policy Limit: "<<incomplete[i].limit<<" - State: "<<incomplete[i].state;
			cout<<" - Number of Locations: "<<incomplete[i].locationNum<<" - SIC Code: " <<incomplete[i].sicCodeString;
			cout<<endl;
		}
	}
	if(accept.size() != 0){
		cout<<"\nAccepted Records:\n";
		for(int i = 0; i<accept.size(); i++){
			cout<<"Submission ID: " <<accept[i].ID<<" - "<<"Agency Name: "<<accept[i].agency<<" - Named Insured: ";
			cout<<accept[i].insured<<" - Policy Limit: "<<accept[i].limit<<" - State: "<<accept[i].state<<" - Number of Locations: ";
			cout<<accept[i].locationNum<<" - SIC Code: " <<accept[i].sicCodeString<<endl;
		}	
	}
	if(reject.size() != 0){
		cout<<"\nRejected Records:\n";
		for(int i = 0; i<reject.size(); i++){
			cout<<"Submission ID: " <<reject[i].ID<<" - "<<"Agency Name: "<<reject[i].agency<<" - Named Insured: ";
			cout<<reject[i].insured<<" - Policy Limit: "<<reject[i].limit<<" - State: "<<reject[i].state<<" - Number of Locations: ";
			cout<<reject[i].locationNum<<" - SIC Code: " <<reject[i].sicCodeString<<endl;
		}
	}
	if(uwReview.size() != 0){
		cout<<"\nUnderwriter Review Records:\n";
		for(int i = 0; i<uwReview.size(); i++){
			cout<<"Submission ID: " <<uwReview[i].ID<<" - "<<"Agency Name: "<<uwReview[i].agency<<" - Named Insured: ";
			cout<<uwReview[i].insured<<" - Policy Limit: "<<uwReview[i].limit<<" - State: "<<uwReview[i].state;
			cout<<" - Number of Locations: "<<uwReview[i].locationNum<<" - SIC Code: " <<uwReview[i].sicCodeString<<endl;
		}	
	}
};
	

//function to assign submission ID number to application for internal records
void records::assignID (){
	int num = 1000000;
	for(int i = 0; i < dqrecords.size(); i++){
		dqrecords[i].ID = num+i;
	}
};

//function to make sure all fields within application have been submitted, 
//if not complete write to 'incomplete' vector and delete from dqrecords.
void records::checkComplete (){
	for(int i = 0; i<dqrecords.size(); i++){
		if(dqrecords[i].agency == "NULL" || dqrecords[i].insured == "NULL" 
			|| dqrecords[i].limit == 0 || dqrecords[i].state == "NULL" 
			|| dqrecords[i].locationNum == 0 || dqrecords[i].sicCodeInt == 0){
				dqrecords[i].flag = "incomplete";
				incomplete.push_back(dqrecords[i]);
				dqrecords.erase(dqrecords.begin()+i);
				i--;
		}
	}
}
//Function to check policy limit against rules. If limit>100000 send to underwriter, if <25000, reject, if between those two accept
void records::checkPolicyLimit(){
	for(int i = 0; i<dqrecords.size(); i++){
		if(dqrecords[i].flag != "uwReview"){
			if(dqrecords[i].limit <=100000 && dqrecords[i].limit >=25000){
				dqrecords[i].flag = "accept";
			}
		}			
		if(dqrecords[i].limit < 25000){
			dqrecords[i].flag = "reject";
			reject.push_back(dqrecords[i]);
			dqrecords.erase(dqrecords.begin()+i);
			i--;
		}	
		if(dqrecords[i].limit >100000){
			dqrecords[i].flag = "uwReview";
		}

	}
};
//Function to check state against list of auto rejects and auto accepts.	
void records::checkState(){
	for(int i = 0; i<dqrecords.size(); i++){
		if((dqrecords[i].state == "TX" || dqrecords[i].state == "OK" || dqrecords[i].state == "LA" 
			|| dqrecords[i].state == "MO" || dqrecords[i].state == "CO") && (dqrecords[i].flag != "uwReview")){
				dqrecords[i].flag = "accept";
		}
		else{
			dqrecords[i].flag = "uwReview";
		}
		if(dqrecords[i].state == "CA" || dqrecords[i].state == "NM" || dqrecords[i].state == "WA" 
			|| dqrecords[i].state == "OR" || dqrecords[i].state == "NY"){
				dqrecords[i].flag = "reject";
				reject.push_back(dqrecords[i]);
				dqrecords.erase(dqrecords.begin()+i);
				i--;
		}		
	}
};					

void records::checkLocationNum(){
	for(int i = 0; i<dqrecords.size(); i++){
		if(dqrecords[i].locationNum > 5){
			dqrecords[i].flag = "uwReview";
		}
		if((dqrecords[i].locationNum <= 5) && (dqrecords[i].locationNum >=1) && (dqrecords[i].flag != "uwReview")){
			dqrecords[i].flag = "accept";
		}
		if(dqrecords[i].locationNum > 50 ){
			dqrecords[i].flag = "reject";
			reject.push_back(dqrecords[i]);	
			dqrecords.erase(dqrecords.begin()+i);
			i--;
		}		
	}
};	
		
//Function to check SIC codes against list of auto accepts and rejects
void records::checkSICCode(){
	for(int i = 0; i<dqrecords.size(); i++){
		if((dqrecords[i].sicCodeInt == 7011 || dqrecords[i].sicCodeInt == 5812 || dqrecords[i].sicCodeInt == 7542 
			|| dqrecords[i].sicCodeInt == 5999 || dqrecords[i].sicCodeInt == 5411 || dqrecords[i].sicCodeInt == 8611 
			|| dqrecords[i].sicCodeInt == 7231 || dqrecords[i].sicCodeInt == 0752) && (dqrecords[i].flag != "uwReview")){
			dqrecords[i].flag = "accept";
		}
		else{
			dqrecords[i].flag = "uwReview";			
		}
		if(dqrecords[i].sicCodeInt == 200 || dqrecords[i].sicCodeInt == 0100 || dqrecords[i].sicCodeInt == 1000 
			|| dqrecords[i].sicCodeInt == 1389 || dqrecords[i].sicCodeInt == 7532 || dqrecords[i].sicCodeInt == 2899 
			|| dqrecords[i].sicCodeInt == 8011 || dqrecords[i].sicCodeInt == 7299){
			dqrecords[i].flag = "reject";
			reject.push_back(dqrecords[i]);	
			dqrecords.erase(dqrecords.begin()+i);
			i--;
		}				

	}	
};
/*
void records::checkSICCode(){
	for(int i = 0; i<dqrecords.size(); i++){
		if((dqrecords[i].sicCode == "7011"|| dqrecords[i].sicCode == "5812"
			|| dqrecords[i].sicCode == "7542" || dqrecords[i].sicCode == "5999"
			|| dqrecords[i].sicCode == "5411" || dqrecords[i].sicCode == "8611"
			|| dqrecords[i].sicCode == "7231" || dqrecords[i].sicCode == "0752") && (dqrecords[i].flag != "uwReview")){
				dqrecords[i].flag = "accept";
		}
		else{
			dqrecords[i].flag = "uwReview";			
		}
		if(dqrecords[i].sicCode == "0200" || dqrecords[i].sicCode == "0100" || dqrecords[i].sicCode == "1000"
			|| dqrecords[i].sicCode == "1389" || dqrecords[i].sicCode == "7532" || dqrecords[i].sicCode == "2899" 
			|| dqrecords[i].sicCode == "8011" || dqrecords[i].sicCode == "7299"){
			dqrecords[i].flag = "reject";
			reject.push_back(dqrecords[i]);	
			dqrecords.erase(dqrecords.begin()+i);
			i--;
		}		
	}	
};
*/
//Function to move accepted and underwriter review records into appropriate vectors after processing
void records::moveRecords(){
	for(int i = 0; i<dqrecords.size(); i++){
		if(dqrecords[i].flag == "accept"){
			accept.push_back(dqrecords[i]);	
			dqrecords.erase(dqrecords.begin()+i);
			i--;
		}
		else if(dqrecords[i].flag == "uwReview"){
			uwReview.push_back(dqrecords[i]);
			dqrecords.erase(dqrecords.begin()+i);
			i--;
		}
	}
};

//Helper function for sorting records by policy limit
void records::heapify(vector<record>& vector, int n, int i){
	int smallest = i;  		//store root in var smallest
	int left = 2*i+1;		 //index of left child
	int right = (2*i)+2; 	//index of right child
	
	//if left child is smaller than root, set smallest to left
	if((left < n) && (vector[left].limit<vector[smallest].limit)){
		smallest = left;
	}
	//if right child is smaller than smallest, set smallest to right
	if((right < n) && (vector[right].limit<vector[smallest].limit)){
		smallest = right;
	}
	//if smallest is not the root, swap root and smallest, recursively call heapify
	if(smallest != i){
		swap(vector[i], vector[smallest]);
		heapify(vector, n, smallest);
	}
};

//Function to sort records by policy limit
void records::heapsortRecords(vector<record>& vector){
	int i;
	for(i = (vector.size()/2)-1; i>=0; i--){
		heapify(vector, vector.size(), i);
	}
	for(i = vector.size()-1; i>0; i--){
		swap(vector[0], vector[i]);
		heapify(vector, i, 0);
	}
};	
	

//function to print report of all automatically rejected applications
void records::printReject (){
	ofstream fout;
	fout.open("RejectedApplications.txt");
	if(fout.is_open()){
		fout<<"Incomplete Applications: \n";
		if(incomplete.size() > 0){
			for(int i=0; i<incomplete.size(); i++){
				fout<<"Submission ID: " <<incomplete[i].ID<<" - ";		
				fout<<"Agency Name: "<<incomplete[i].agency<<" - Named Insured: "<<incomplete[i].insured;
				fout<<" - Policy Limit: "<<incomplete[i].limit<<" - State: "<<incomplete[i].state<<" - Number of Locations: ";
				fout<<incomplete[i].locationNum<<" - SIC Code: " <<incomplete[i].sicCodeString<<endl;
			}
			fout<<endl;
		}
		else{
			fout<<"None\n";
		}
		fout<<"\nRejected Applications:\n";
		if(reject.size()>0){	
			for(int i=0; i<reject.size(); i++){
				fout<<"Submission ID: " <<reject[i].ID<<" - ";		
				fout<<"Agency Name: "<<reject[i].agency<<" - Named Insured: "<<reject[i].insured;
				fout<<" - Policy Limit: "<<reject[i].limit<<" - State: "<<reject[i].state<<" - Number of Locations: ";
				fout<<reject[i].locationNum<<" - SIC Code: " <<reject[i].sicCodeString<<endl;
			}
		}
		else{
			fout<<"None\n";
		}
	}
	else{
		cout<<"Unable to create report\n";
	}
};

//function to print report of all applications to be sent for underwriter review	
void records::printUWReview (){
	ofstream fout;
	fout.open("UWReviewApplications.txt");
	if(fout.is_open()){
		if(uwReview.size() > 0){
			fout<<"Underwriter Review Needed Applications: \n";
			for(int i=0; i<uwReview.size(); i++){
				fout<<"Submission ID: " <<uwReview[i].ID<<" - ";		
				fout<<"Agency Name: "<<uwReview[i].agency<<" - Named Insured: "<<uwReview[i].insured<<" - Policy Limit: ";
				fout<<uwReview[i].limit<<" - State: "<<uwReview[i].state<<" - Number of Locations: "<<uwReview[i].locationNum;
				fout<<" - SIC Code: " <<uwReview[i].sicCodeString<<endl;
			}
		}
		else{
			fout<<"None\n";
		}
	}
	else{
		cout<<"Unable to create report\n";
	}
	
};

//function to print report of all automatically accepted applications					
void records::printAccept(){
	ofstream fout;
	fout.open("Accept.txt");
	if(fout.is_open()){
		fout<<"Accepted Auto-rate Applications: \n";
		if(accept.size() > 0){
			for(int i=0; i<accept.size(); i++){
				fout<<"Submission ID: " <<accept[i].ID<<" - ";		
				fout<<"Agency Name: "<<accept[i].agency<<" - Named Insured: "<<accept[i].insured<<" - Policy Limit: ";
				fout<<accept[i].limit<<" - State: "<<accept[i].state<<" - Number of Locations: "<<accept[i].locationNum;
				fout<<" - SIC Code: " <<accept[i].sicCodeString<<endl;
			}
		}
		else{
			fout<<"None\n";
		}
	}
	else{
		cout<<"Unable to create report\n";
	}

}
