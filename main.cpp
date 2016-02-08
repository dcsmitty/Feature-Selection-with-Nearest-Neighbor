#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

struct Percent
{
    double count;
    double total;
};

struct Neighbor
{
    int pos;
    double total;
};

bool compareByLength(const Neighbor &a, const Neighbor &b)
{
    return a.total < b.total;
}

//declare global variables
int alg_Select;
double input_array[2048][64];
int feature_amount;
int row_amount;


void normalize(double max, double min)
{
    //normalize values to between -100 and 100
    double range = max/2 - min;
    for(int i = 0; i < row_amount; i++)
    {
        for(int j = 1; j < feature_amount; j++)
        {
            double temp = input_array[i][j] - min;
            input_array[i][j] = temp/range;
            input_array[i][j] -= 1;
        }
    }
    
}
void get_User_Input()
{
    string user_string;
    
    cout << "Feature Selection Algorithm" << endl;
    
    //take in file for input
    Check: cout << "Type in the name of the file to test : ";
    cin >> user_string;
    
    ifstream fin;
    fin.open(user_string.c_str());
    if(!fin){
        cerr << "Enter a valid input file!!!" << endl; 
        goto Check;
    }
    
    double fixed_val;
    int temp_counter = 0;
    int row_counter = 0;
    int feature_check = 0;
    bool start_check = false;
    
    double max = 0;
    double min = 1000000;
    
    //input data from file into matrix
    while(!fin.eof())
    {
        fin >> fixed_val;
        if((fixed_val == 1 || fixed_val == 2) && start_check == true )
        {
            row_counter++;
            feature_check = temp_counter;
            temp_counter = 0;
        }
        start_check = true;
        //max and min val for normalization
        if(fixed_val > max && temp_counter != 0)
        {
            max = fixed_val;
        }
        if(fixed_val < min && temp_counter != 0)
        {
            min = fixed_val;
        }
        input_array[row_counter][temp_counter] = fixed_val;
        temp_counter++;
    }
    row_counter++;
    fin.close();
    
    row_amount = row_counter;
    feature_amount = feature_check;
    
    normalize(max, min);
    
    //get algorithm you want to use
    int temp_int;
    Check2: cout << "Type the number of the algorithm you want to run." << endl;
    
    cout << "\t1) Foward Selection" << endl;
    cout << "\t2) Backward Elimination" << endl;
    cout << "\t3) Smith's Algorithm" << endl;
    
    cin >> temp_int;
    if(temp_int < 1 || temp_int > 3)
    {
        cerr << "Choose a valid algorithm!!!" << endl; 
        goto Check2;
    }
    
    alg_Select = temp_int;
    
    cout << "This dataset has " << feature_amount-1 << " features" << 
    ", with " << row_amount << " instances!!!" << endl;
}

Percent nearest_neighbor(vector<int> list, int knn)
{
    int count = 0;
    int total_amount = 0;
    
    //loop through each element(row)
    for(int i = 0; i < row_amount; i++)
    {
        double total = 0;
        vector<Neighbor> total_array;
        vector<double> temp;
        //create temp list that stores all the selected features at current row
        for(unsigned int z = 0; z < list.size(); z++)
        {
            temp.push_back(input_array[i][list.at(z)]);
        }
        //loop through all rows except the same one being tested
        for(int k = 0; k < row_amount; k++)
        {
            if(i != k)
            {
                total = 0;
                vector<double> temp2;
                //create temp list that stores all the selected features at each row
                for(unsigned int z = 0; z < list.size(); z++)
                {
                    temp2.push_back(input_array[k][list.at(z)]);
                }
                //calculation for distance between specified row and all other rows
                for(unsigned int x = 0; x < temp.size(); x++)
                {
                    total += pow(temp.at(x)-temp2.at(x), 2) * i;
                }
                total = sqrt(total);
                
                //make a list of all totals
                Neighbor t_bor;
                t_bor.total = total;
                t_bor.pos = k;
                total_array.push_back(t_bor);
            }
        }
        //sort the list of totals
        sort(total_array.begin(), total_array.end(), compareByLength);
        int count1 = 0;
        int count2 = 0;
        //used for k neareset neighbor so it only checks k closest neighbors
        for(int z = 0; z < knn; z++)
        {
            if(input_array[total_array.at(z).pos][0] == 1)
            {
                count1++;
            }
            else if(input_array[total_array.at(z).pos][0] == 2)
            {
                count2++;
            }
            else
            {
                cout << "broken" << endl;
            }
        }
        //increase count for percenatage
        if(count1 >= count2)
        {
            if(input_array[i][0] == 1)
            {
                count++;
            }
        }
        else
        {
            if(input_array[i][0] == 2)
            {
                count++;
            }
        }
            
    
        total_amount++;
    }
    
    //get the count and total used for best percentage
    Percent percentage;
    percentage.count = double(count);
    percentage.total = double(total_amount);
    
    
    return percentage;
}



void list_disp(vector<int> list)
{
    //gets rid of extra code cuts back on size (called by display)
    for(unsigned int k = 0; k < list.size()-1; k++)
    {
        cout << list.at(k) << ",";
    }
    cout << list.at(list.size()-1);
}

void display(Percent P, vector<int> list, int key)
{
    double per = P.count/P.total;
    per *= 100;
    
    sort(list.begin(), list.end());
    //different display messages depending on where they are used
    if(key == 1)
    {
        cout << "Using feature(s) {";
        for(unsigned int k = 0; k < list.size(); k++){
            if(k != list.size()-1){
                cout << list.at(k) << ",";
            }
            else{
                cout << list.at(k);
            }
        }
        cout << "} accuracy is " << fixed << setprecision(1) << per << "%" << endl;
    }
    else if(key == 2)
    {
        cout << endl << "Feature set {"; 
        list_disp(list);
        cout << "} was best, with accuracy of " << fixed << setprecision(1) << per << "%" << endl << endl;
    }
    else if(key == 3)
    {
        cout << endl << "(Warning, Accuracy has decreased! Continuing search in case of local maxima) "
        << "Feature set {";
        list_disp(list);
        cout << "} was best, with accuracy of " << fixed << setprecision(1) << per << "%" << endl << endl;
    }
    
    else if(key == 4)
    {
        cout << "Finished search!! The best feature subset is {";
        list_disp(list);
        cout << "}, which has an accuracy of  " << fixed << setprecision(1) << per << "%"<< endl;
    }
}

void Search()
{
    cout << endl << "Beginning Search..." << endl << endl;
    
    vector<int> row_list;
    vector<int> best_list;
    int value;
    int bad_value;
    Percent Total_Best;
    Total_Best.count = 0;
    Total_Best.total = 0;
    
    //Foward Selection
    if(alg_Select == 1)
    {
        //loop through all the features until every feature is in list
        while(int(row_list.size()) < feature_amount-1)
        {
            Percent Local_Best;
            double best_val = 0;
            //for all features
            for(int i = 1; i < feature_amount; i++)
            {
                bool flag = true;
                //check to make sure feature is not in feature list
                for(unsigned int j = 0; j < row_list.size(); j++)
                {
                    if(i == row_list.at(j))
                        flag = false;
                }
                if(flag)
                {
                    vector<int>temp = row_list;
                    temp.push_back(i);
                    int knn;
                    if(feature_amount >= 40){
                        knn = feature_amount/2;
                    }
                    else if(feature_amount < 40 && feature_amount >= 10){
                        knn = feature_amount/2;
                    }
                    else{
                        knn = 1;
                    }
                    //nearest neighbor call
                    Percent P = nearest_neighbor(temp, 5);
                    display(P, temp, 1);
                    temp.clear();
                    //check for best feature to be added
                    if(P.count/P.total > best_val)
                    {
                        best_val = P.count/P.total;
                        Local_Best.count = P.count;
                        Local_Best.total = P.total;
                        value = i;
                    }
                }
            }
            
            //push best local feature on list
            row_list.push_back(value);
            
            //checks to see total best feature list for all cases
            if(Local_Best.count/Local_Best.total > Total_Best.count/Total_Best.total && Total_Best.total != 0)
            {
                display(Local_Best, row_list, 2);
                best_list = row_list;
                Total_Best.count = Local_Best.count;
                Total_Best.total = Local_Best.total;
            }
            else if(Local_Best.count/Local_Best.total <= Total_Best.count/Total_Best.total && Total_Best.total != 0)
            {
                display(Local_Best, row_list, 3);
            }
            else if(Total_Best.total == 0)
            {
                display(Local_Best, row_list, 2);
                best_list = row_list;
                Total_Best.count = Local_Best.count;
                Total_Best.total = Local_Best.total;
            }
        }
        display(Total_Best, best_list, 4);
    }
    
    //Backwards Elimination
    else if(alg_Select == 2)
    {
        bool start = true;
        //fill up the feature list with every feature
        for(int i = 1; i < feature_amount; i++)
        {
            row_list.push_back(i);
        }
        //while there are some features loop
        while(int(row_list.size()) > 1)
        {
            Percent Local_Best;
            double best_val = 0;
            //check if first iteration
            if(start == true)
            {
                vector<int>temp = row_list;
                Percent P = nearest_neighbor(temp, 5);
                display(P, temp, 1);
                if(P.count/P.total > best_val)
                {
                    best_val = P.count/P.total;
                    Local_Best.count = P.count;
                    Local_Best.total = P.total;
                }
            }
            else
            {
                //loop through the features backwards
                for(int i = feature_amount-1; i > 0; i--)
                {
                    bool flag = true;
                    //check to see if feature is on list
                    for(unsigned int j = 0; j < row_list.size(); j++)
                    {
                        if(i == row_list.at(j))
                        {
                            flag = false;
                        }
                    }
                    if(!flag)
                    {
                        vector<int>temp = row_list;
                        temp.erase(std::remove(temp.begin(), temp.end(), i), temp.end());
                        //call nearest neighbor
                        Percent P = nearest_neighbor(temp, 5);
                        display(P, temp, 1);
                        if(P.count/P.total > best_val)
                        {
                            best_val = P.count/P.total;
                            Local_Best.count = P.count;
                            Local_Best.total = P.total;
                            value = i;
                        }
                    }
                }
                //pop element
                row_list.erase(remove(row_list.begin(), row_list.end(), value), row_list.end());
            }
            
            start = false;
            //check for total best element
            if(Local_Best.count/Local_Best.total >= Total_Best.count/Total_Best.total && Total_Best.total != 0)
            {
                display(Local_Best, row_list, 2);
                best_list = row_list;
                Total_Best.count = Local_Best.count;
                Total_Best.total = Local_Best.total;
            }
            else if(Local_Best.count/Local_Best.total < Total_Best.count/Total_Best.total && Total_Best.total != 0)
            {
                display(Local_Best, row_list, 3);
            }
            else if(Total_Best.total == 0)
            {
                display(Local_Best, row_list, 2);
                best_list = row_list;
                Total_Best.count = Local_Best.count;
                Total_Best.total = Local_Best.total;
            }
        }
        display(Total_Best, best_list, 4);
    }
    else if(alg_Select == 3)
    {
        vector<int> banned_pos;
        //loop until row list has half the features
        while(int(row_list.size()) < (feature_amount)/2)
        {
            Percent Local_Best;
            Percent Local_Worst;
            double best_val = -1;
            double worst_val = 1000000;
            for(int i = 1; i < feature_amount; i++)
            {
                bool flag = true;
                bool flag2 = true;
                //check to make sure feature isnt in feature list
                for(unsigned int j = 0; j < row_list.size(); j++)
                {
                    if(i == row_list.at(j))
                        flag = false;
                }
                //check to make sure feature isnt in banned list
                for(unsigned int j = 0; j < banned_pos.size(); j++)
                {
                    if(i == banned_pos.at(j))
                        flag2 = false;
                }
                if(flag && flag2)
                {
                    vector<int>temp = row_list;
                    temp.push_back(i);
                    int knn;
                    if(row_amount >= 100){
                        knn = row_amount/20;
                    }
                    else if(row_amount < 100 && row_amount >= 10){
                        knn = 3;
                    }
                    else{
                        knn = 1;
                    }
                    //call nearest neighbor
                    Percent P = nearest_neighbor(temp, 5);
                    display(P, temp, 1);
                    temp.clear();
                    //find best val
                    if(P.count/P.total > best_val)
                    {
                        best_val = P.count/P.total;
                        Local_Best.count = P.count;
                        Local_Best.total = P.total;
                        value = i;
                    }
                    //find worst val
                    if(P.count/P.total < worst_val)
                    {
                        worst_val = P.count/P.total;
                        Local_Worst.count = P.count;
                        Local_Worst.total = P.total;
                        bad_value = i;
                    }
                }
            }
            //add onto row list and banned list
            row_list.push_back(value);
            banned_pos.push_back(bad_value);
            
            //update best total accuracy
            if(Local_Best.count/Local_Best.total > Total_Best.count/Total_Best.total && Total_Best.total != 0)
            {
                display(Local_Best, row_list, 2);
                best_list = row_list;
                Total_Best.count = Local_Best.count;
                Total_Best.total = Local_Best.total;
            }
            else if(Local_Best.count/Local_Best.total <= Total_Best.count/Total_Best.total && Total_Best.total != 0)
            {
                display(Local_Best, row_list, 3);
            }
            else if(Total_Best.total == 0)
            {
                display(Local_Best, row_list, 2);
                best_list = row_list;
                Total_Best.count = Local_Best.count;
                Total_Best.total = Local_Best.total;
            }
            
        }
        display(Total_Best, best_list, 4);
    }
}

int main()
{
    get_User_Input();
    Search();
    
    return 0;
}
