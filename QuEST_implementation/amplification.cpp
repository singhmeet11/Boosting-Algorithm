#include<iostream>
#include<vector>
#include<cstring>
#include<math.h>
#include<string>
#include <sstream>
#include<QuEST.h>
#include "qram_w_dag.h"
using namespace std;

void oracle(Qureg qr, int* qubit_ord ){


	ComplexMatrix2 z = {
		.real={{1,0},{0,-1}},
		.imag={{0,0},{0,0}}
	};
	int control[] = {qubit_ord[1],qubit_ord[2]};

    for(int i = 0 ; i<3; i++){
		// cout<<qubit_ord[i]<<"\n";
	}
// marking 3
	// pauliX(qr, qubit_ord[2]);
	// // pauliX(qr, qubit_ord[3]);		
	// // hadamard(qr, qubit_ord[0]);
	// multiControlledUnitary(qr, control, 2, qubit_ord[0], z);
	// // hadamard(qr, qubit_ord[0]);
	// pauliX(qr, qubit_ord[2]);

	// for 1
	pauliX(qr, qubit_ord[0]);
	// pauliX(qr, qubit_ord[3]);		
	// hadamard(qr, qubit_ord[0]);
	multiControlledUnitary(qr, control, 2, qubit_ord[0], z);
	// hadamard(qr, qubit_ord[0]);
	pauliX(qr, qubit_ord[0]);
	// pauliX(qr, qubit_ord[3]);		
	// pauliX(qr,qubit_ord[1]);
// // marking 0110
// 	pauliX(qr, qubit_ord[0]);
// 	pauliX(qr, qubit_ord[3]);		
// 	// hadamard(qr, qubit_ord[0]);
// 	multiControlledUnitary(qr, control, 3, qubit_ord[0], z);
// 	// hadamard(qr, qubit_ord[0]);
// 	pauliX(qr, qubit_ord[0]);
// 	pauliX(qr, qubit_ord[3]);		

// // marking 1001

// 	pauliX(qr, qubit_ord[2]);
// 	pauliX(qr, qubit_ord[1]);		
// 	multiControlledUnitary(qr, control, 3, qubit_ord[0], z);
// 	pauliX(qr, qubit_ord[2]);
// 	pauliX(qr, qubit_ord[1]);		


}



void S_0(Qureg qr,vector<int> qubit_ord){

	ComplexMatrix2 z = {
		.real={{1,0},{0,-1}},
		.imag={{0,0},{0,0}}
	};
	// int control[] = {qubit_ord[4],qubit_ord[3],qubit_ord[2],qubit_ord[1]};

	int len = qubit_ord.size();
	// cout<<len;
	int control[len - 1];
	
    for(int i = 0 ; i<len-1; i++){
		// cout<<qubit_ord[i]<<"\n";
		control[i] = qubit_ord[i];
		
	}
    for(int i = 0 ; i<len-1; i++){
		pauliX(qr,control[i]);
	}
	pauliX(qr, qubit_ord[len-1]);
	// pauliX(qr, qubit_ord[2]);
	// pauliX(qr, qubit_ord[1]);
	// pauliX(qr, qubit_ord[0]);
	// pauliX(qr, qubit_ord[3]);
	// pauliX(qr, qubit_ord[4]);
	// pauliX(qr, qubit_ord[3]);
	multiControlledUnitary(qr, control, len-1, qubit_ord[len-1], z);
	// pauliX(qr, qubit_ord[2]);
	// pauliX(qr, qubit_ord[1]);
	// pauliX(qr, qubit_ord[0]);
	// pauliX(qr, qubit_ord[3]);
	// pauliX(qr, qubit_ord[4]);
    for(int i = 0 ; i<len-1; i++){
		pauliX(qr,control[i]);
	}
	pauliX(qr, qubit_ord[len-1]);
	// // pauliX(qr, qubit_ord[3]);
}

void amplification(int reps){
	// int a; // a = no. of address registerz
    int no_of_adress_qubits=2;
    int no_of_bits_in_memory_cell=3;

    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;
	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
	int q_a = 0; // 0 to a-1
	int q_r = a; // a to a+ 2^a -1
	int q_m = a+ r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = a + m+r; // a+ 2^(a+1) to a+2^(a+1)
    vector<int> data;
    data.push_back(6);
    data.push_back(3);
    data.push_back(4);
    data.push_back(2);
//    data.push_back(0);
//    data.push_back(4);
//    data.push_back(5);
//    data.push_back(11);
//    data.push_back(12);
//    data.push_back(13);
    
    QuESTEnv env = createQuESTEnv();
    Qureg qr = createQureg(q_b+mc,env);
	initZeroState(qr);

	// preparation of initial state
    QRAM(qr,env,no_of_adress_qubits, no_of_bits_in_memory_cell, data);

	// application of iterations
	vector<int> diffuser_arr;

    for(int i = 0 ; i<q_b +mc; i++){
        diffuser_arr.push_back(q_a+ i);
        // cout<<diffuser_arr[i]<<"\n";
    }
	int len = sizeof(diffuser_arr)/sizeof(diffuser_arr[0]);
	// cout<<len;
	// application of oracle 
    int bus_qubits[mc];
    for(int i = 0 ; i<mc; i++){
        bus_qubits[i] = q_b+ i;
        // cout<<bus_qubits[i]<<"\n";
    }
    




	for(int i=0;i<reps;i++){
		oracle(qr, bus_qubits);
    	QRAM_dag(qr,env,no_of_adress_qubits, no_of_bits_in_memory_cell, data);
		S_0(qr, diffuser_arr);
		QRAM(qr,env,no_of_adress_qubits, no_of_bits_in_memory_cell, data);
		// 
	}





    prob_out_meas(mc, qr, bus_qubits);
    
    destroyQureg(qr, env);
	destroyQuESTEnv(env);

	

    


	// this for calculation of probability of measuring each state



    ///
	// char a = decToBinary(5);
	// cout << a ;


    //printf("decimal to binary" , decToBinary(10));


// thisdict = {
//   "000": 0.2,
//   "001": 0.3,
//   "010": 0.4
// }

    
	  		
}
int main(){

	amplification(1);
    
    return 0;
}

