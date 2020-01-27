#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>




using namespace std;

class pairElem1{
	int numEl;
	double quantity;

public:

	pairElem1(int numEl, double quantity){
		this->numEl = numEl;
		this->quantity = quantity;
	}

	int getNum(){
		return numEl;
	}

	double getQuant(){
		return quantity;
	}

};

class pairElem2{
	int numEl;
	vector<bool> binCode;

public:

	pairElem2(){
		numEl = 0;
		binCode = {};
	}

	int getNum(){
		return numEl;
	}

	vector<bool> getBin(){
		return binCode;
	}

	bool getBinElement(int i){
		return binCode[i];
	}

	int getSize(){
		return binCode.size();
	}



	void add1(){
		binCode.push_back(1);
	}

	void add0(){
		binCode.push_back(0);
	}

	void setNum(int numEl){
		this->numEl = numEl;
	}

	void printBin(){
		cout << " binCode = ";
		for (int i = 0; i < binCode.size(); i++)
		{
			cout << binCode[i];
		}
		cout << endl;
	}
	
};

string enterFileName(){                         //создаем функцию для записи имени обрабатываемого документа
	string fileName;
	cout << "Enter the path to file with extension.\n";
	cin >> fileName;
	return fileName;
}

string makeDocName(string startName){                     //создаем функцию для создания наименования результирующего документа
	string expansion;                                       //переменная "расширение"
	for (int i = 0; i < int(startName.size() + 1); i++)      //когда доходим до точки, применяем функцию substr которая возвращает расширение
	{
		if (startName[i] == '.'){

			expansion = startName.substr(0, i);
		}
	}
	expansion = expansion + ".S_F";                  // к расширению добавляем номер нашего варианта и расширения будующего документа
	return expansion;
}



void readFile(string fileName, vector<pairElem1>& frequencyArr, double& entropy, long int& byteQuantity, string& firstName){

	ifstream ourFile(fileName);
	if (!ourFile.is_open())                                                       //проверка на открытие файла
	{
		cout << "File was not found!" << endl;
	}
	else
	{
		cout << "File is opened." << endl;                                         //когда файл открыт
		
		ourFile >> firstName;
		ourFile >> byteQuantity;

	
		int numElement = 0;
		double quantity = 0;
		while (!ourFile.eof())
		{
			numElement = 0;
			quantity = 0;
			ourFile >> numElement;
			ourFile >> quantity;
			pairElem1 pair(numElement, quantity);
			frequencyArr.push_back(pair); 
			
		}


		entropy = frequencyArr[frequencyArr.size() - 1].getQuant() + frequencyArr[frequencyArr.size() - 1].getNum();
		frequencyArr.pop_back();
	}

	ourFile.close();                                                                      //закрываем открытый файл
}



double quantSum( vector<pairElem1>& frequencyArr, int begin1, int end1){
	int iterator = 0;
	double sumQuant = 0;
	if (begin1 <= end1)
	{
		for (int i = begin1; i <= end1 && end1 < frequencyArr.size(); i++)
		{
			sumQuant += frequencyArr[i].getQuant();
		}
	}
	else{
		cout << "Begin off arr was smallest then end." << endl;
	}
	return sumQuant;
}

int findIt( vector<pairElem1>& frequencyArr, int begin1, int end1 ){
	int iterator;
	double sum1 = 0;
	double sum2 = 0;
	double mod = 1;
	for (int i = begin1; i < end1; i++)
	{
		sum1 = quantSum(frequencyArr, begin1, i);
		sum2 = quantSum(frequencyArr, i + 1, end1);
		if ( abs(sum1 - sum2) < mod){
			mod = abs(sum1 - sum2);
			iterator = i;
		}
	}



	return iterator;
}




void synchronization(vector<pairElem1>& frequencyArr, vector<pairElem2>& resultArr){

	for (int i = 0; i < frequencyArr.size(); i++)
	{
		pairElem2 pair;
		resultArr.push_back(pair);
		resultArr[i].setNum( frequencyArr[i].getNum() );
	}


}

void shannonFano(vector<pairElem1>& frequencyArr, vector<pairElem2>& resultArr, int begin1, int end1){
	if (begin1 < end1){

		 int iterator = findIt(frequencyArr, begin1, end1);
		 for (int i = begin1; i <= end1; i++)
		 {
			 if (i <= iterator){ resultArr[i].add1(); }
			 else { resultArr[i].add0(); }
		 }
		 shannonFano(frequencyArr, resultArr, begin1, iterator);
		 shannonFano(frequencyArr, resultArr, iterator + 1, end1);


	}

}

double averageCodeLength(vector<pairElem1>& frequencyArr, vector<pairElem2>& resultArr){
	double result = 0;
	for (int i = 0; i < frequencyArr.size(); i++)
	{
		result += (frequencyArr[i].getQuant() * resultArr[i].getSize());
	}
	return result;
}



void saveResult(string pathName, string firstName, long int byteQuantity, vector<pairElem2>& resultArr, double entropy, double averCode){
	string docPathName = makeDocName(pathName);           
	
	ofstream ourFile(docPathName);                                         //открываем файл для записи(создаем)
	ourFile << firstName << "\n";                                             //записываем имя обрабатываемого файла 
	ourFile << byteQuantity << "\n\n";                                        //записываем общее количество 
	for (int i = 0; i < resultArr.size(); i++)
	{
		ourFile << resultArr[i].getNum() << "\t";
		for (int j = 0; j < resultArr[i].getSize(); j++)
		{
			ourFile << resultArr[i].getBinElement(j);
		}
		ourFile << "\n";
	}
	ourFile << "\n" << averCode;
	ourFile << "\n" << entropy;                                                  //записываем энтропию
	ourFile.close();                                                                    //закрываем файл
}





int main(){
	vector<pairElem1> frequencyArr = {};
	vector<pairElem2> resultArr = {};
	string fileName = enterFileName();
	string firstName;
	long int byteQuantity = 0;
	double entropy = 0;
	double averCode = 0;
	
	readFile(fileName, frequencyArr, entropy, byteQuantity, firstName);
	
	synchronization(frequencyArr, resultArr);

	

	

	shannonFano(frequencyArr, resultArr, 0, resultArr.size() -1);
	averCode = averageCodeLength(frequencyArr, resultArr);
	saveResult(fileName, firstName, byteQuantity, resultArr, entropy, averCode);


	system("pause");
	return 0;
}