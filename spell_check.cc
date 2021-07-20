//Huihong Zheng
//csci335 Hw#4
//This programs will uses dictionary file as the hash table and document file 
//Helps to improve grammarly in sentences and suggest the closest word
//Using quadratic hashing
#include "quadratic_probing.h"
#include <iostream>
#include <fstream>
#include <sstream>    
#include <string> 
#include <cstring>    
#include <array>        
#include <cctype>     

using namespace std;



//insert all the dictionary words into the hash table
template <typename HashTableType>
void MakeDictionary(HashTableType &hash_table, const string &dictionary_filename)            
{

   hash_table.MakeEmpty();           
   hash_table.ToMakeEmptyCollision();          
   
   string dictionaryInsert;               
   ifstream DictionaryFile(dictionary_filename);        
   while (getline(DictionaryFile,dictionaryInsert))              
   {

   		hash_table.Insert(dictionaryInsert);            
   }    
}

//convert character to string
string Convert( char TheCharacter)        
{
	return string(1,TheCharacter);         
}     



//This case A will add one character in any possible position for check with dictionary word
template<typename HashTableType>
void CaseA( HashTableType &hash_table , string  DocumentWords)               
{
	int TheSizeOfWord = DocumentWords.size();              
	
	for (char ForEachAplha = 'a' ; ForEachAplha <= 'z'; ++ForEachAplha)
	{

		for( size_t LoopthroughWordSize = 0 ; TheSizeOfWord +1 > LoopthroughWordSize ; ++LoopthroughWordSize) 
		{	
			string Adder = " ";
			
			Adder = DocumentWords.insert(LoopthroughWordSize,Convert(ForEachAplha));              
			
			DocumentWords.erase(LoopthroughWordSize,1);            
			
			bool CaseAFound = hash_table.Contains(Adder);
			if (CaseAFound)      
			{
				cout<<"*** " <<DocumentWords << " -> "<<Adder<<" *** case A"<<endl;
			}   
		}    
			
	}     
}     

//Case B will be removing one character from the word and check with dictionary
template<typename HashTableType>
void CaseB( HashTableType &hash_table , string  DocumentWords)
{
	int TheSizeOfWord = DocumentWords.size();
	for ( size_t LoopthroughWordSize =0; TheSizeOfWord> LoopthroughWordSize; ++LoopthroughWordSize)
	{
		string Remover = DocumentWords;               
		
		Remover.erase(LoopthroughWordSize,1); 
		 
		// or
		// DocumentWords.erase(DocumentWords.begin() + LoopthroughWordSize);
		bool CaseBFound = hash_table.Contains(Remover);
		if(CaseBFound)            
		{
			cout<<"*** "<<DocumentWords<<" -> "<<Remover<<" *** case B"<<endl;
		}    
    }   
         
}     



//CaseC will be swapping adjacent characters in the word
//Be checked with the hash table (dictionary word)
template<typename HashTableType>
void CaseC( HashTableType &hash_table , string  DocumentWords)
{
        int TheSizeOfWord = DocumentWords.size();

        for ( size_t LoopthroughWordSize = 0 ; TheSizeOfWord- 1 >LoopthroughWordSize; ++LoopthroughWordSize)
        {
        	string Swapper = DocumentWords;
        	swap(Swapper[LoopthroughWordSize], Swapper[LoopthroughWordSize+1]);
        	
		bool CaseCFound = hash_table.Contains(Swapper);
		if(CaseCFound)       
		{
			cout<<"*** "<<DocumentWords<<" -> "<<Swapper<<" *** case C"<<endl;
		}   	
	}   

}


//convert all the words to lowercase
string LowerTheWordsCase(string TheWord)
{
     string GiveLowerCase ; 
     int counter = 0; 
     int SizeofWord = TheWord.size(); 
     while (SizeofWord> counter) 
     {

     	if(TheWord[counter] >= 'A' && TheWord[counter] <= 'Z') 
     	{
     		TheWord[counter] = TheWord[counter] +32; 
     	}
     	++counter;
     }
     GiveLowerCase = TheWord;
     return GiveLowerCase; 
}


//Removing all the non characters in the first index and last index of the word
string GetRemover(string TheWord)
{
        string StoreUpdate = TheWord;
        if ( StoreUpdate.size() < 0 ) 
        {
        	return StoreUpdate;
		}
        else
        {

        	if( (StoreUpdate.size()) == 1 && (!isalnum(StoreUpdate[0]))) 
        	{	
        		StoreUpdate.erase(0,1);       
        		return StoreUpdate;          
        	}
        	        	

        	else if( (!isalnum(StoreUpdate[0])) && (!isalnum(StoreUpdate[StoreUpdate.size()-1])))
        	{
        		StoreUpdate.erase(0,1); 
        		StoreUpdate.erase(StoreUpdate.size()-1,1); 
        		return StoreUpdate;
        	}

        	else if((StoreUpdate.size() > 1) &&  (!isalnum(StoreUpdate[StoreUpdate.size()-1])))
        	{
        		StoreUpdate.erase(StoreUpdate.size()-1,1); 
        		return StoreUpdate;
        	}
        	else
        	{
        		
        		return StoreUpdate; 
        	}
        }	        
}


//Convert all the words to lowercase before checking with the dictionary(because dictionary words are all lowercase)
//If the document word is not found in the hash table, it will uses the three cases to check a "Correct" word
//It will give the correct spelling suggestion according to three of the cases
template <typename HashTableType>
void SpellChecker(HashTableType &hash_table , const string &document_filename) 
{

     string doucment; 
     ifstream DocFile(document_filename.c_str()); 
     while(getline(DocFile, doucment)) 
     {
     	 stringstream docuWord(doucment);
     	 string WordFromDoc; 
     	 for (; docuWord >> WordFromDoc ;)
     	 {
     	 	string MakeThemLower = LowerTheWordsCase(WordFromDoc); 	 	
     	 	string PunctuatedString = GetRemover(MakeThemLower);
     	 	bool SearchWordFromDoc = hash_table.Contains(PunctuatedString);
     	 	if (SearchWordFromDoc)
     	 	{
     	 	   cout<<PunctuatedString<< " is CORRECT "<<endl;
     	 	}
     	 	else
     	 	{   
				cout<<PunctuatedString<<" is INCORRECT "<<endl;
     	 	   	CaseA(hash_table,PunctuatedString);  	 	    
     	 	  	CaseB(hash_table,PunctuatedString); 
     	 	  	CaseC(hash_table,PunctuatedString);    	 	 
     	 	}  	 		
     	 }
      }
}
	
//Will be using quadratic hashing as the declaration table
int testSpellingWrapper(int argument_count, char** argument_list) 
{
    
    const string document_filename(argument_list[1]);     
    const string dictionary_filename(argument_list[2]);   

    HashTable<string> dictionary;          
    MakeDictionary(dictionary, dictionary_filename);           
    SpellChecker(dictionary, document_filename);            

    return 0;
}

int main(int argc, char** argv) 
{
    if (argc != 3) 
    {
        cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>"
             << endl;
        return 0;
    }

    testSpellingWrapper(argc, argv);

    return 0;
}
