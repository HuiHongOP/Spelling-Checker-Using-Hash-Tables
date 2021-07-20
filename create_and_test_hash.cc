//Huihong Zheng
//csci335 Hw#4
//This program will use different type of hashing methods : quadratic , linear, and double 
//It will use hash table to store the words in wordsEnglish.txt and uses query_words.txt (It can be sentences) as the finder of words to the words in the hash table
//It will gives the output whether if the words in query_words.txt is "Found" or "Not_Found"
//If "Found" , then states how many probing it took
//Program also gives table size(hash table),  load factor, number of elements in the table, avg coll ,and  total coll
#include "quadratic_probing.h"
#include "linear_probing.h"    
#include "double_hashing.h"     
#include <iostream>
#include <fstream> 
#include <string> 
#include <sstream>    

using namespace std;

//This function will take in the words in queryfile/query.text and try to find it in the hash table
//It will display whether if it's found( how many probing it took) or not found
template <typename HashTableType>
void CheckerQuery(HashTableType &hash_table, const string &query_filename)        
{
  
  
  string querywords;     
  ifstream ReadQueryWords(query_filename);     
  if(ReadQueryWords.is_open())   
  {
     while(getline(ReadQueryWords,querywords))        
    {
    	stringstream ReaderOfOneLine(querywords);        
    	string HolderOfQueryWords;     
    	for(; ReaderOfOneLine >> HolderOfQueryWords;)
    	{
  	    	bool FoundInsideHashTable = hash_table.Contains(HolderOfQueryWords);
  		    if (FoundInsideHashTable)            
  		    {
  		    	cout<<HolderOfQueryWords <<" Found "<<hash_table.TotalCollisionsProbing()+1<<endl;
  		    	hash_table.ToMakeEmptyCollision();      
  		    }   
  		    else          
  	    	{
  	    		cout<<HolderOfQueryWords <<" Not_Found "<<hash_table.TotalCollisionsProbing()+1<<endl;
  		    	hash_table.ToMakeEmptyCollision();       
  	    	}   
    	}    
    }   
  }
  else{
      cout<<"The text/file can not be opened"<<endl;
  }
}   

//Function will create hash table using words.txt or wordsEnglish.txt 
//Insert the words into hash table
//@ TheNumberOfElement get the total words/ dictionary words in the hash table
//@ TheSizeHashTable get the hash table size
//@LoadFactor = (total words/ dictionary words in hash table ) divide by hash table size
//@GetTotalCollision = Total collision in order to insert all of the words into the hash table
//@AverageCollisions = total collision divides by total inserted words into the hash table
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
  hash_table.MakeEmpty();      
  hash_table.ToMakeEmptyCollision();          
  
  
  string WordInText;
  ifstream ReadingWordFile(words_filename);
  while(getline(ReadingWordFile,WordInText))
  {
  	hash_table.Insert(WordInText);           
  }   

  int TheNumberOfElement = hash_table.CurrentElementSize();
  
  int TheSizeHashTable = hash_table.TheHashTableSize();
  
  double LoadFactor = (hash_table.CurrentElementSize() /hash_table.TheHashTableSize());
  
  int GetTotalCollision = hash_table.TotalCollisionsProbing();
  
  double AverageCollisions = (hash_table.TotalCollisionsProbing()/ TheNumberOfElement);

  cout << "number_of_elements: " << TheNumberOfElement<<endl;  
  cout << "size_of_table: " << TheSizeHashTable<<endl;           
  cout << "load_factor: " << LoadFactor<<endl;        
  cout << "average_collisions: " << AverageCollisions<<endl;           
  cout << "total_collisions: " << GetTotalCollision <<endl<<endl;        
  hash_table.ToMakeEmptyCollision();        
  CheckerQuery(hash_table,query_filename);               
  
}  


int testHashingWrapper(int argument_count, char **argument_list) {
    const string words_filename(argument_list[1]);
    const string query_filename(argument_list[2]);
    const string param_flag(argument_list[3]);
    int R = 31;      
    if (argument_count == 5) {
        const string rvalue(argument_list[4]);
        R = stoi(rvalue);
    }

    if (param_flag == "linear")     
    {
        HashTableLinear<string> linear_probing_table;      
        TestFunctionForHashTable(linear_probing_table, words_filename,
                                 query_filename);                  
    } else if (param_flag == "quadratic")     
    {
        HashTable<string> quadratic_probing_table;     
        TestFunctionForHashTable(quadratic_probing_table, words_filename,
                                 query_filename);                  
    } else if (param_flag == "double") {     
         cout << "r_value: " << R <<endl;            
         HashTableDouble<string> double_probing_table;       
         double_probing_table.SetRValue(R);                   
         TestFunctionForHashTable(double_probing_table, words_filename,
                                 query_filename);                 
    } else 
    {
        cout << "Unknown hash type " << param_flag
             << " (User should provide linear, quadratic, or double)" << endl;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 4 && argc != 5) {
        cout << "Usage: " << argv[0]
             << " <wordsfilename> <queryfilename> <hashing type: double, quadratic, linear>" << endl;
        cout << "or Usage: " << argv[0]
             << " <wordsfilename> <queryfilename> <hashing type: double, quadratic, linear> <rvalue>" << endl;
        return 0;
    }

    testHashingWrapper(argc, argv);
    return 0;
}

