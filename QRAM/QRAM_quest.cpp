#include<iostream>
#include<vector>
#include<cstring>
#include<math.h>
#include<QuEST.h>
using namespace std;

int b2d(int n){  // this function converts bindary values to decimal ones
    	
    int num = n;
    int dec_value = 0;
	int base = 1;
	int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;
        base = base * 2;
    }
   return dec_value;
}

int countDigit(int n){ // 
    int count = 0;
    while (n != 0)
    {
        n = n / 10;
        ++count;
    }
    return count;
}
	
int numConcat(int num1, int num2){  // for concatinating one number in front of the other thing mainly the addition of 0 or 1 to the circuits different parts
	
    int digits = countDigit(num2);
    num1 = num1 * (pow(10,digits));
    num1 += num2;
    return num1;
}  
   
   
void QRAM(int a){
	// int a; // a = no. of address registerz
	QuESTEnv env = createQuESTEnv();
		
	int r = 2^a; // various types of quantum bits we will be using
	int m = 2^a;
	int bus = 1;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
	int q_a = 0; // 0 to a-1
	int q_r = a; // a to a+ 2^a -1
	int q_m = a+ pow(2,a); // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = a + pow(2,(a+1)); // a+ 2^(a+1)
	// initalizing different quantum registers
	
	Qureg qr = createQureg(q_b,env);
	initZeroState(qr);
	
	
	// setting the memory cells and the address register for accessing various memory cells 
	// pauliX(qr, 1]);
	// pauliX(qr, 1);
	
	// Branching starts from here 
	// 
	// We make the second qubit of the routing node equal to 1
	pauliX(qr, 1);
	
	// First Node Branches
	// Not sure if cx can be used on two different quantum registers this way controlledNot([Qureg* qr_a, Qureg* qr_r], qr_a,qr_a[0],qr_r[0]);
 	controlledNot(qr , 0 , q_r);
	controlledNot(qr , q_r , 1);

	
	// Maybe multiple q-registers can be accessed as a list of pointers
	// Maybe normally calling them        
	if(a!=1){
		for(int i=2; i<a+1; i++){ // here every 'i' represent the level of the binary tree
			for(int j=0; j<pow(2,(i-1)); j++){ 
				int control[] = {q_a + (i-1), q_r + j};
				int *ptr_c = control;
				int target[] = {q_r +j + pow(2, i-1)};
				int *ptr_t = target;
				multiControlledMultiQubitNot(qr, ptr_c , 2, ptr_t, 1); // here used '&' so as to control the problem of int to int*
				controlledNot(qr, q_r + j+pow(2,(i-1)),q_r + j);
			}
		}	
	}        

// now we will be adding how to couple the memory cells to the routing qubits

	//Memory Cells
	vector<int> div;
	div.push_back(1);
	div.push_back(0);
	
	int d = pow(2,a);
	
	vector<int> new_div;
	for(int i=0; i<d ; i++)
	    new_div.push_back(0);
	
	for(int i=0; i<a-1; i++){
	
		//appending
		div.insert(div.end(), div.begin(), div.end());
		for(int i=0; i<div.size(); i++){
		}
		cout<<endl;
		int dim = div.size();
		for(int j = 0; j<dim/2; j++){
			new_div[j]=numConcat(div[j], 0);
		}
		for(int k = 0; k<dim/2; k++){
			new_div[k]=numConcat(div[k], 1);
		}
		
		for(int m=0; m<dim; m++){
			div[m] = new_div[m];
		}
		cout<<endl;
	}   
	
	//Converting to decimal
	int l = div.size();
    int div_dec[l] = {0};
    for(int i=0; i<l; i++){
    	div_dec[i] = int(b2d(div[i]));
	}
	
	// once we know which ones to couple and how then we can apply ccx gate with routing and memory qubtis as control and the  bus as the target
	// ccx on memory cells
	for (int i=0; i<pow(2,a); i++){
		int control[] = {q_r + i , q_m + div_dec[i]};
		int target[] = {q_b};
		int *ptr_c = control;
		int *ptr_t = target;
		multiControlledMultiQubitNot(qr , ptr_c, 2 ,ptr_t, 1);
	}
	
	// now we will be measuring each qubit and seeing how it works properly 
	int prob = calcProbOfOutcome(qr, q_b, 1);
	//int outcome = measureWithStats(qr, q_b, &prob);
	printf("The memory cell 00 was measured and the outcome collapsed to 0 with probability %d\n", prob);


	// destroying the enviroment and the qubits now
	destroyQureg(qr, env);
	destroyQuESTEnv(env);

	  		
}

int main(){
	QRAM(2);
	return 0;
}