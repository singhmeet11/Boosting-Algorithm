#include<iostream>
#include<vector>
#include<cstring>
#include<math.h>
#include<cmath>
#include<string>
#include <sstream>
#include<QuEST.h>
using namespace st

int b2d(int n){  // this function converts binary values to decimal ones
    	
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

int numConcat(int a, int b){  // for concatinating one number in front of the other thing mainly the addition of 0 or 1 to the circuits different parts
	
	
	ostringstream str1;
	ostringstream str2;
	
	str1<<a;
	string s1 = str1.str();
	str2<<b;
	string s2 = str2.str();

    // Concatenate both strings
    string s = s1 + s2;

    stringstream strtoint(s);
    int c = 0;
    strtoint>>c;
    return c;
}


void dec_to_bin(long unsigned int n, long unsigned int* binary, int* len){
	
	int binaryNum[32];
 
    int i = 0;
    while (n > 0) {
 
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
 	int bin = 0;
    for (int j = i - 1; j >= 1; j--){
        bin = bin + binaryNum[j];
        bin = bin*10;
	}
	bin = bin + binaryNum[0];
	
	*binary = bin;
	*len = i;
	
}

string decimalToBinary(long unsigned int n, int no_of_places){
	
	int num = no_of_places;
	
	long unsigned int binary = 0;
	
	int len = 0;
	
	dec_to_bin(n, &binary, &len);
	
	ostringstream bin_str;
    bin_str << binary;
    string binstr = bin_str.str();	

	int j=0;
	if(len != num){
		j = num-len;
		for(int k=0; k<j; k++){
			binstr = '0' + binstr;
		}	
	}
	return binstr;
}

void swapp(char &s1, char &s2){
    
    char temp;
    temp = s1;
    s1 = s2;
    s2 = temp;
}

// void swap(int &i1, int &i2){
//     int temp;
//     temp = i1;
//     i1 = i2; 
//     i2= temp;
// }

void flipper(vector<vector<char> > &matrix, int mc, int l, vector<vector<char> > &f_new_data){
    
    for(int i = 0; i <l ; i++)
    {   
        vector<char> vv;
        for(int j = mc-1; j >=0; j--)
        {
            vv.push_back(matrix[i][j]);
        }
        f_new_data.push_back(vv);
    }
}



void QEA_meas(int num_qubits,Qureg qr, int* meas_qubits, int num_ctrls, vector<int> data){
	cout << "{";

	// here - num_qubits refers to the number of qubits that you wanna measure, and meas_qubits is the exact index
	qreal outcomeProbs[int(pow(2,num_qubits))];
	calcProbOfAllOutcomes(outcomeProbs, qr , meas_qubits, num_qubits);

    int largest_dec = 0;
	string largest_str;
    float large = 0;
	for(int i=0; i<pow(2,num_qubits); i++){

        if(large < outcomeProbs[i]){
            large = outcomeProbs[i];
            largest_str = decimalToBinary(i, num_qubits);
            largest_dec = i;
            // cout<<"\nEigenvalue of the Grover operator: "<<largest_str;
            // cout<<" "<<large;
        }
        
    }
    cout<<"Eigenvalue of the Grover operator: "<<largest_str;
    float pi = 3.141592;
    float theta = (largest_dec/pow(2, num_ctrls))*pi;
    cout<<"\nTheta: "<<theta;

    int s = data.size();
    int N=4;
    // for(int k =0;k<s; k++){
    //     if(data[k]!=0){
    //         N++;
    //     }
    // }

    float a = pow(sin(theta),2);

    float M = N*a;

    cout<<"\nNumber of Solutions : "<<N-M<<" ";
    cout << "}" << "\n";
}

void prob_out_meas(int num_qubits,Qureg qr, int* meas_qubits){
	cout << "{";

	// here - num_qubits refers to the number of qubits that you wanna measure, and meas_qubits is the exact index
	qreal outcomeProbs[int(pow(2,num_qubits))];
	calcProbOfAllOutcomes(outcomeProbs, qr , meas_qubits, num_qubits);

	 
	for(int i=0; i<pow(2,num_qubits); i++){
		if(outcomeProbs[i]!=0){

            if(i==0){
		    	//if(outcomeProbs[i] != 0){
		    		cout << "\"" << 000<<"\"" ":" ;
		    		cout <<  "\"" << outcomeProbs[i] << "\""<< "," << "\n" ;
		    	//}
    
		    }
		    else{
		    	//if(outcomeProbs[i] != 0){
		        	cout << "\"" << decimalToBinary(i, num_qubits) <<"\"" ":";
		    		cout <<  "\"" << outcomeProbs[i] << "\"" << "," << "\n" ;
		    	//}
		    }
        }
			
	}
	cout << "}" << "\n";
}


void mem_data(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data, int num_ctrls){

    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;
	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
    int q_ctrl = 0;
    int q_a = 0+num_ctrls; // 0 to a-1
	int q_r = num_ctrls+a; // a to a+ 2^a -1
	int q_m = num_ctrls+a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = num_ctrls+a + m + r; // a+ 2^(a+1) to a+2^(a+1)
	// initalizing different quantum registers
    int len = data.size();

    for(int i=0; i<len; i++){

        if(data[i]>pow(2,mc)){
            cout<<"Error!! The value of the data to be stored is bigger then the 2**mc";
            return;
        }

    }

    string bin_data[len];

    for(int j=0; j<len; j++){
        bin_data[j] = decimalToBinary(data[j], mc);
    }
    vector<vector<char> > new_data;
    for(int x = 0; x<len; x++){
        vector<char> vv;
        for(int y=0 ; y<mc ;y++){
            vv.push_back(bin_data[x][y]);
        }
        new_data.push_back(vv);
    }

    ///Flipping done
    vector<vector<char> > f_new_data;
    flipper(new_data, mc, len, f_new_data);


    char new_data_row[mc*len];
    int c = 0;
    for(int x = 0; x<len; x++){
        for(int y=0 ; y<mc ;y++){
            new_data_row[c] = f_new_data[x][y];
            c++;
        }
    }
    int lll = sizeof(new_data_row)/sizeof(new_data_row[0]);

    for(int iio = 0; iio<lll; iio++){
        if(new_data_row[iio] == '1'){
            pauliX(qr, q_m + iio);
        }
    }
    
}

void meas_QRAM(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data, int num_ctrls){

    //QuESTEnv env = createQuESTEnv();
    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;

	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
    int q_ctrl = 0;
    int q_a = 0+num_ctrls; // 0 to a-1
	int q_r = num_ctrls+a; // a to a+ 2^a -1
	int q_m = num_ctrls+a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = num_ctrls+a + m + r; // a+ 2^(a+1) to a+2^(a+1)
	// initalizing different quantum registers

    //int len = *(&data + 1) - data;

    vector<int> div;
	div.push_back(1);
	div.push_back(0);

    int d = pow(2,a);

    vector<int> new_div;
	for(int i=0; i<d ; i++)
	    new_div.push_back('0');
	
	for(int i=0; i<a-1; i++){
	
		//appending
		div.insert(div.end(), div.begin(), div.end());
		int dim = div.size();
		
		for(int j = 0; j<dim/2; j++){
			new_div[j]=numConcat(0,div[j]);
		}
		for(int k = dim/2; k<dim; k++){
			new_div[k]=numConcat(1,div[k]);
		}

        for(int kk = 0; kk< dim; kk++){
            div.push_back(new_div[kk]);
        }

    }

    if(a == 1){
        vector<int> new_div;
        new_div.push_back(1);
        new_div.push_back(0);
    }

    for(int m=0; m<new_div.size(); m++){
        div[m] = int(new_div[m]);

    }
    vector<int> div_decimal;
    int leng = div.size();
    for(int ii = 0; ii<leng; ii++){
        div_decimal.push_back(b2d(div[ii]));
    }
    int l = div_decimal.size();
    int final_ord_mat[mc][l];
    for(int p=0; p<l; p++){
        int add = 0;
        for(int jj = 0; jj< mc; jj++){
            final_ord_mat[jj][p] = int(div_decimal[p])*int(mc);
            final_ord_mat[jj][p] = add+final_ord_mat[jj][p];
            add++;
    
        }

    }

    int final_ord[l*mc];
    int counter= 0;
    for(int p=0; p<l; p++){
        for(int jj = 0; jj< mc; jj++){
     
            final_ord[counter] = final_ord_mat[jj][p];
  
            counter++;
        }
    }

    
    int ss = mc*(div_decimal.size());
    int add_ord[ss];
    int j_last = 0;
    
    int add_ord_real[ss];
    int counts=0;
    for(int id = 0; id<l; id++){
        for(int idk = j_last; idk<j_last+mc; idk++){

            add_ord_real[counts] = id;
    
            counts++;
        }
    }
        
    vector<int> one_ord;
    for(int numb = 0; numb<mc; numb++)
    one_ord.push_back(numb);

    vector<int> bus_ord;

    for(int ik = 0; ik<r; ik++)
    bus_ord.insert(bus_ord.end(),one_ord.begin(), one_ord.end());


    for(int ix = 0; ix<m; ix++){
        int ctrls[] = {q_r+int(add_ord_real[ix]), q_m + int(final_ord[ix])};

        int targs[] = {q_b + int(bus_ord[ix])};

        multiControlledMultiQubitNot(qr, ctrls, 2, targs, 1); 
    }

}

void QRAM(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data, int num_ctrls){
    
    // QuESTEnv env = createQuESTEnv();
    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;
	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
    int q_ctrl = 0;
    int q_a = 0+num_ctrls; // 0 to a-1
	int q_r = num_ctrls+a; // a to a+ 2^a -1
	int q_m = num_ctrls+a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = num_ctrls+a + m + r; // a+ 2^(a+1) to a+2^(a+1)


    // Qureg qr = createQureg(q_b+mc,env);
	// initZeroState(qr);

//    // inputting add (For now its not generalized)
    for(int kk = 0; kk<a; kk++)
    hadamard(qr, q_a+kk);

    mem_data(qr,env,no_of_adress_qubits, no_of_bits_in_memory_cell,data, num_ctrls);
    //pauliX(qr, 1);
    // this part will have the branching basically the part where the routing qubits gets activated when the correct address is inputted into the structure....
    // 	setting the memory cells and the address register for accessing various memory cells

	// Branching starts from here 
	
	// We make the second qubit of the routing node equal to 1
	pauliX(qr, q_r + 1);
	
 	controlledNot(qr , q_a + 0 , q_r); 
	controlledNot(qr , q_r , q_r + 1);

    
	if(a!=1){
		for(int i=2; i<a+1; i++){ // here every 'i' represent the level of the binary tree
			for(int j=0; j<pow(2,(i-1)); j++){ 
				
				// trying to use another gate
			    ComplexMatrix2 x = {
    				.real={{0,1},{1,0}},
        			.imag={{0,0},{0,0}}
    			};
				int control[] = {q_a + (i-1), q_r + j};
				multiControlledUnitary(qr, control, 2, q_r +j + pow(2, i-1), x);////////// no prob
				controlledNot(qr, q_r + j+pow(2,(i-1)),q_r + j); //////////////
			}
		}	
	}
          
    meas_QRAM(qr, env, no_of_adress_qubits, no_of_bits_in_memory_cell,data,  num_ctrls);
    // //destroying the enviroment and the qubits now
	 
 

    
}



void mem_data_dag(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data){

    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;
	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
	int q_a = 0; // 0 to a-1
	int q_r = a; // a to a+ 2^a -1
	int q_m = a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = a + m + r; // a+ 2^(a+1) to a+2^(a+1)
	// initalizing different quantum registers
    int len = data.size();

    for(int i=0; i<len; i++){

        if(data[i]>pow(2,mc)){
            cout<<"Error!! The value of the data to be stored is bigger then the 2**mc";
            return;
        }

    }

    string bin_data[len];

    for(int j=0; j<len; j++){
        bin_data[j] = decimalToBinary(data[j], mc);
    }
    vector<vector<char> > new_data;
    for(int x = 0; x<len; x++){
        vector<char> vv;
        for(int y=0 ; y<mc ;y++){
            vv.push_back(bin_data[x][y]);
        }
        new_data.push_back(vv);
    }

    ///Flipping done
    vector<vector<char> > f_new_data;
    flipper(new_data, mc, len, f_new_data);


    char new_data_row[mc*len];
    int c = 0;
    for(int x = 0; x<len; x++){
        for(int y=0 ; y<mc ;y++){
            new_data_row[c] = f_new_data[x][y];
            c++;
        }
    }
    int lll = sizeof(new_data_row)/sizeof(new_data_row[0]);

    for(int iio = 0; iio<lll; iio++){
        if(new_data_row[iio] == '1'){
            pauliX(qr, q_m + iio);
        }
    }
}

void meas_QRAM_dag(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data){

    //QuESTEnv env = createQuESTEnv();
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
	// initalizing different quantum registers

    //int len = *(&data + 1) - data;

    vector<int> div;
	div.push_back(1);
	div.push_back(0);

    int d = pow(2,a);

    vector<int> new_div;
	for(int i=0; i<d ; i++)
	    new_div.push_back('0');
	
	for(int i=0; i<a-1; i++){
	
		//appending
		div.insert(div.end(), div.begin(), div.end());
		int dim = div.size();
		
		for(int j = 0; j<dim/2; j++){
			new_div[j]=numConcat(0,div[j]);
		}
		for(int k = dim/2; k<dim; k++){
			new_div[k]=numConcat(1,div[k]);
		}

        for(int kk = 0; kk< dim; kk++){
            div.push_back(new_div[kk]);
        }

    }

    if(a == 1){
        vector<int> new_div;
        new_div.push_back(1);
        new_div.push_back(0);
    }

    for(int m=0; m<new_div.size(); m++){
        div[m] = int(new_div[m]);

    }
    vector<int> div_decimal;
    int leng = div.size();
    for(int ii = 0; ii<leng; ii++){
        div_decimal.push_back(b2d(div[ii]));
    }
    int l = div_decimal.size();
    int final_ord_mat[mc][l];
    for(int p=0; p<l; p++){
        int add = 0;
        for(int jj = 0; jj< mc; jj++){
            final_ord_mat[jj][p] = int(div_decimal[p])*int(mc);
            final_ord_mat[jj][p] = add+final_ord_mat[jj][p];
            add++;
    
        }

    }

    int final_ord[l*mc];
    int counter= 0;
    for(int p=0; p<l; p++){
        for(int jj = 0; jj< mc; jj++){
     
            final_ord[counter] = final_ord_mat[jj][p];
  
            counter++;
        }
    }

    
    int ss = mc*(div_decimal.size());
    int add_ord[ss];
    int j_last = 0;
    
    int add_ord_real[ss];
    int counts=0;
    for(int id = 0; id<l; id++){
        for(int idk = j_last; idk<j_last+mc; idk++){

            add_ord_real[counts] = id;
    
            counts++;
        }
    }
        
    vector<int> one_ord;
    for(int numb = 0; numb<mc; numb++)
    one_ord.push_back(numb);

    vector<int> bus_ord;

    for(int ik = 0; ik<r; ik++)
    bus_ord.insert(bus_ord.end(),one_ord.begin(), one_ord.end());


    for(int ix = m-1; ix>=0; ix--){
        int ctrls[] = {q_r+int(add_ord_real[ix]), q_m + int(final_ord[ix])};

        int targs[] = {q_b + int(bus_ord[ix])};

        multiControlledMultiQubitNot(qr, ctrls, 2, targs, 1); ///////////////// correct
    }
}

void QRAM_dag(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data){
    
    //QuESTEnv env = createQuESTEnv();
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

    // Qureg qr = createQureg(q_b+mc,env);
	// initZeroState(qr);

//    // inputting add

    meas_QRAM_dag(qr, env, no_of_adress_qubits, no_of_bits_in_memory_cell,data);
    

    if(a!=1){
		for(int i=a; i>=2; i--){ // here every 'i' represent the level of the binary tree
			for(int j=pow(2,(i-1))-1; j>=0; j--){ 
				
				// trying to use another gate
			    ComplexMatrix2 x = {
    				.real={{0,1},{1,0}},
        			.imag={{0,0},{0,0}}
    			};
				int control[] = {q_a + (i-1), q_r + j};
                controlledNot(qr, q_r + j+pow(2,(i-1)),q_r + j); ////////////
				multiControlledUnitary(qr, control, 2, q_r +j + pow(2, i-1), x);/////////////no  prob
				
			}
		}	
	}

    controlledNot(qr , q_r , q_r + 1); ////////
    controlledNot(qr , q_a + 0 , q_r);//////////
	pauliX(qr, q_r + 1);

    mem_data_dag(qr,env,no_of_adress_qubits, no_of_bits_in_memory_cell,data);

    for(int kk = 0; kk<a; kk++)
    hadamard(qr, q_a+kk);

    
}


void c_mem_data(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data, int num_ctrls, int ctrl){

    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;
	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
	int q_ctrl = 0;
    int q_a = 0+num_ctrls; // 0 to a-1
	int q_r = num_ctrls+a; // a to a+ 2^a -1
	int q_m = num_ctrls+a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = num_ctrls+a + m + r; // a+ 2^(a+1) to a+2^(a+1)
	// initalizing different quantum registers
    int len = data.size();

    for(int i=0; i<len; i++){

        if(data[i]>pow(2,mc)){
            cout<<"Error!! The value of the data to be stored is bigger then the 2**mc";
            return;
        }

    }

    string bin_data[len];

    for(int j=0; j<len; j++){
        bin_data[j] = decimalToBinary(data[j], mc);
    }
    vector<vector<char> > new_data;
    for(int x = 0; x<len; x++){
        vector<char> vv;
        for(int y=0 ; y<mc ;y++){
            vv.push_back(bin_data[x][y]);
        }
        new_data.push_back(vv);
    }

    ///Flipping done
    vector<vector<char> > f_new_data;
    flipper(new_data, mc, len, f_new_data);


    char new_data_row[mc*len];
    int c = 0;
    for(int x = 0; x<len; x++){
        for(int y=0 ; y<mc ;y++){
            new_data_row[c] = f_new_data[x][y];
            c++;
        }
    }
    int lll = sizeof(new_data_row)/sizeof(new_data_row[0]);

    for(int iio = 0; iio<lll; iio++){
        if(new_data_row[iio] == '1'){
            controlledNot(qr, q_ctrl+ctrl, q_m + iio); ////////////
        }
    }
    
}

void c_meas_QRAM(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data,int num_ctrls, int ctrl){

    //QuESTEnv env = createQuESTEnv();
    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;

	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
	int q_ctrl = 0;
    int q_a = 0+num_ctrls; // 0 to a-1
	int q_r = num_ctrls+a; // a to a+ 2^a -1
	int q_m = num_ctrls+a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = num_ctrls+a + m + r; // a+ 2^(a+1) to a+2^(a+1)
	// initalizing different quantum registers

    //int len = *(&data + 1) - data;

    vector<int> div;
	div.push_back(1);
	div.push_back(0);

    int d = pow(2,a);

    vector<int> new_div;
	for(int i=0; i<d ; i++)
	    new_div.push_back('0');
	
	for(int i=0; i<a-1; i++){
	
		//appending
		div.insert(div.end(), div.begin(), div.end());
		int dim = div.size();
		
		for(int j = 0; j<dim/2; j++){
			new_div[j]=numConcat(0,div[j]);
		}
		for(int k = dim/2; k<dim; k++){
			new_div[k]=numConcat(1,div[k]);
		}

        for(int kk = 0; kk< dim; kk++){
            div.push_back(new_div[kk]);
        }

    }

    if(a == 1){
        vector<int> new_div;
        new_div.push_back(1);
        new_div.push_back(0);
    }

    for(int m=0; m<new_div.size(); m++){
        div[m] = int(new_div[m]);

    }
    vector<int> div_decimal;
    int leng = div.size();
    for(int ii = 0; ii<leng; ii++){
        div_decimal.push_back(b2d(div[ii]));
    }
    int l = div_decimal.size();
    int final_ord_mat[mc][l];
    for(int p=0; p<l; p++){
        int add = 0;
        for(int jj = 0; jj< mc; jj++){
            final_ord_mat[jj][p] = int(div_decimal[p])*int(mc);
            final_ord_mat[jj][p] = add+final_ord_mat[jj][p];
            add++;
    
        }

    }

    int final_ord[l*mc] ;
    int counter= 0;
    for(int p=0; p<l; p++){
        for(int jj = 0; jj< mc; jj++){
     
            final_ord[counter] = final_ord_mat[jj][p];
  
            counter++;
        }
    }

    
    int ss = mc*(div_decimal.size());
    int add_ord[ss];
    int j_last = 0;
    
    int add_ord_real[ss];
    int counts=0;
    for(int id = 0; id<l; id++){
        for(int idk = j_last; idk<j_last+mc; idk++){

            add_ord_real[counts] = id;
    
            counts++;
        }
    }
        
    vector<int> one_ord;
    for(int numb = 0; numb<mc; numb++)
    one_ord.push_back(numb);

    vector<int> bus_ord;

    for(int ik = 0; ik<r; ik++)
    bus_ord.insert(bus_ord.end(),one_ord.begin(), one_ord.end());


    for(int ix = 0; ix<m; ix++){
        int ctrls[] = {q_ctrl+ctrl, q_r+int(add_ord_real[ix]), q_m + int(final_ord[ix])};

        int targs[] = {q_b + int(bus_ord[ix])};

        multiControlledMultiQubitNot(qr, ctrls, 3, targs, 1); ///////////////correct
    }

}

void c_QRAM(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data, int num_ctrls, int ctrl){
    
    //QuESTEnv env = createQuESTEnv();
    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;
	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
    int q_ctrl = 0;
    int q_a = 0+num_ctrls; // 0 to a-1
	int q_r = num_ctrls+a; // a to a+ 2^a -1
	int q_m = num_ctrls+a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = num_ctrls+a + m + r; // a+ 2^(a+1) to a+2^(a+1)

    //Qureg qr = createQureg(q_b+mc,env);
	//initZeroState(qr);

//    // inputting add

    ComplexMatrix2 h = {
                .real={{1/sqrt(2),1/sqrt(2)},{1/sqrt(2),-1/sqrt(2)}},
                .imag={{0,0},{0,0}}
            };
    //int hctrl1[] = {q_ctrl+ctrl-1};

    //cout<<"\n ctrl : "<<q_ctrl+ctrl-1<<endl;
    controlledUnitary(qr, q_ctrl+ctrl, q_a, h); 
    controlledUnitary(qr, q_ctrl+ctrl, q_a+1, h); 

    // hadamard(qr, q_a);
    // hadamard(qr, q_a+1);

    c_mem_data(qr,env,no_of_adress_qubits, no_of_bits_in_memory_cell,data, num_ctrls, ctrl);
    //pauliX(qr, 1);
    // this part will have the branching basically the part where the routing qubits gets activated when the correct address is inputted into the structure....
    // 	setting the memory cells and the address register for accessing various memory cells

	// Branching starts from here 
	
	// We make the second qubit of the routing node equal to 1
	controlledNot(qr,q_ctrl+ctrl, q_r + 1);
	
    int ctrl1[] = {q_ctrl+ctrl, q_a};

    ComplexMatrix2 x = {
    				 .real={{0,1},{1,0}},
        			.imag={{0,0},{0,0}}
    			};

 	multiControlledUnitary(qr , ctrl1, 2, q_r,x);

    int ctrl2[] = {q_ctrl+ctrl, q_r};
	multiControlledUnitary(qr , ctrl2, 2 , q_r + 1, x);

    
	if(a!=1){
		for(int i=2; i<a+1; i++){ // here every 'i' represent the level of the binary tree
			for(int j=0; j<pow(2,(i-1)); j++){ 
				
				// trying to use another gate
				int control[] = {q_ctrl+ctrl, q_a + (i-1), q_r + j};
				multiControlledUnitary(qr, control, 3, q_r +j + pow(2, i-1), x);
                int ctrl3[] = {q_ctrl+ctrl,q_r + j+ pow(2,(i-1)) };
				multiControlledUnitary(qr,ctrl3, 2 ,q_r + j, x);
			}
		}	
	}
          
    c_meas_QRAM(qr, env, no_of_adress_qubits, no_of_bits_in_memory_cell,data, num_ctrls,ctrl);
}


void c_mem_data_dag(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data,int num_ctrls, int ctrl){

    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;
	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
    int q_ctrl = 0;
    int q_a = 0+num_ctrls; // 0 to a-1
	int q_r = num_ctrls+a; // a to a+ 2^a -1
	int q_m = num_ctrls+a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = num_ctrls+a + m + r; // a+ 2^(a+1) to a+2^(a+1)
    int len = data.size();

    for(int i=0; i<len; i++){

        if(data[i]>pow(2,mc)){
            cout<<"Error!! The value of the data to be stored is bigger then the 2**mc";
            return;
        }

    }

    string bin_data[len];

    for(int j=0; j<len; j++){
        bin_data[j] = decimalToBinary(data[j], mc);
    }
    vector<vector<char> > new_data;
    for(int x = 0; x<len; x++){
        vector<char> vv;
        for(int y=0 ; y<mc ;y++){
            vv.push_back(bin_data[x][y]);
        }
        new_data.push_back(vv);
    }

    ///Flipping done
    vector<vector<char> > f_new_data;
    flipper(new_data, mc, len, f_new_data);


    char new_data_row[mc*len];
    int c = 0;
    for(int x = 0; x<len; x++){
        for(int y=0 ; y<mc ;y++){
            new_data_row[c] = f_new_data[x][y];
            c++;
        }
    }
    int lll = sizeof(new_data_row)/sizeof(new_data_row[0]);

    for(int iio = lll-1; iio>=0; iio--){
        if(new_data_row[iio] == '1'){
            controlledNot(qr, q_ctrl+ctrl, q_m + iio);
        }
    }
}

void c_meas_QRAM_dag(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data, int num_ctrls, int ctrl){

    //QuESTEnv env = createQuESTEnv();
    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;

	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
    int q_ctrl = 0;
    int q_a = 0+num_ctrls; // 0 to a-1
	int q_r = num_ctrls+a; // a to a+ 2^a -1
	int q_m = num_ctrls+a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = num_ctrls+a + m + r; // a+ 2^(a+1) to a+2^(a+1)
	// initalizing different quantum registers

    //int len = *(&data + 1) - data;

    vector<int> div;
	div.push_back(1);
	div.push_back(0);

    int d = pow(2,a);

    vector<int> new_div;
	for(int i=0; i<d ; i++)
	    new_div.push_back('0');
	
	for(int i=0; i<a-1; i++){
	
		//appending
		div.insert(div.end(), div.begin(), div.end());
		int dim = div.size();
		
		for(int j = 0; j<dim/2; j++){
			new_div[j]=numConcat(0,div[j]);
		}
		for(int k = dim/2; k<dim; k++){
			new_div[k]=numConcat(1,div[k]);
		}

        for(int kk = 0; kk< dim; kk++){
            div.push_back(new_div[kk]);
        }

    }

    if(a == 1){
        vector<int> new_div;
        new_div.push_back(1);
        new_div.push_back(0);
    }

    for(int m=0; m<new_div.size(); m++){
        div[m] = int(new_div[m]);

    }
    vector<int> div_decimal;
    int leng = div.size();
    for(int ii = 0; ii<leng; ii++){
        div_decimal.push_back(b2d(div[ii]));
    }
    int l = div_decimal.size();
    int final_ord_mat[mc][l];
    for(int p=0; p<l; p++){
        int add = 0;
        for(int jj = 0; jj< mc; jj++){
            final_ord_mat[jj][p] = int(div_decimal[p])*int(mc);
            final_ord_mat[jj][p] = add+final_ord_mat[jj][p];
            add++;
    
        }

    }

    int final_ord[l*mc] ;
    int counter= 0;
    for(int p=0; p<l; p++){
        for(int jj = 0; jj< mc; jj++){
     
            final_ord[counter] = final_ord_mat[jj][p];
  
            counter++;
        }
    }

    
    int ss = mc*(div_decimal.size());
    int add_ord[ss];
    int j_last = 0;
    
    int add_ord_real[ss];
    int counts=0;
    for(int id = 0; id<l; id++){
        for(int idk = j_last; idk<j_last+mc; idk++){

            add_ord_real[counts] = id;
    
            counts++;
        }
    }
        
    vector<int> one_ord;
    for(int numb = 0; numb<mc; numb++)
    one_ord.push_back(numb);

    vector<int> bus_ord;

    for(int ik = 0; ik<r; ik++)
    bus_ord.insert(bus_ord.end(),one_ord.begin(), one_ord.end());


    for(int ix = m-1; ix>=0; ix--){
        int ctrls[] = {q_ctrl+ctrl, q_r+int(add_ord_real[ix]), q_m + int(final_ord[ix])};

        int targs[] = {q_b + int(bus_ord[ix])};

        multiControlledMultiQubitNot(qr, ctrls, 3, targs, 1); //////////////// correct
    }
}

void c_QRAM_dag(Qureg qr, QuESTEnv env, int no_of_adress_qubits, int no_of_bits_in_memory_cell, vector<int> data, int num_ctrls, int ctrl){
    
    //QuESTEnv env = createQuESTEnv();
    int a = no_of_adress_qubits;
    int mc = no_of_bits_in_memory_cell;
	int r = pow(2,a); // various types of quantum bits we will be using
	int m = pow(2,a)*mc;
	int bus = mc;

	// as QuEST can not have different quantum registers we will just have to have a bunch of varibales which define the starting of different quantum registers 
    int q_ctrl = 0;
    int q_a = 0+num_ctrls; // 0 to a-1
	int q_r = num_ctrls+a; // a to a+ 2^a -1
	int q_m = num_ctrls+a + r; // a+ 2^a to a+ 2^(a+1) -1
 	int q_b = num_ctrls+a + m + r; // a+ 2^(a+1) to a+2^(a+1)

//    // inputting add

    c_meas_QRAM_dag(qr, env, no_of_adress_qubits, no_of_bits_in_memory_cell,data,num_ctrls, ctrl);
    ComplexMatrix2 x = {
        .real={{0,1},{1,0}},
        .imag={{0,0},{0,0}}
        };

    if(a!=1){
		for(int i=a; i>=2; i--){ // here every 'i' represent the level of the binary tree
			for(int j=pow(2,(i-1))-1; j>=0; j--){ 
				
				// trying to use another gate

				int control[] = {q_ctrl+ctrl, q_a + (i-1), q_r + j};
                int control2[] = {q_ctrl+ctrl, q_r + j+pow(2,(i-1))};
                multiControlledUnitary(qr, control2, 2,q_r + j, x);
				multiControlledUnitary(qr, control, 3, q_r +j + pow(2, i-1), x);
				
			}
		}	
	}

    int cct[] ={q_ctrl+ctrl, q_r} ;
    multiControlledUnitary(qr , cct,2 , q_r + 1,x);
    int cct2[] ={q_ctrl+ctrl, q_a + 0 } ;
    multiControlledUnitary(qr , cct2, 2, q_r, x);
	controlledNot(qr, q_ctrl+ctrl, q_r + 1);

    c_mem_data_dag(qr,env,no_of_adress_qubits, no_of_bits_in_memory_cell,data, num_ctrls, ctrl);

    ComplexMatrix2 h = {
                .real={{1/sqrt(2),1/sqrt(2)},{1/sqrt(2),-1/sqrt(2)}},
                .imag={{0,0},{0,0}}
            };

    controlledUnitary(qr, q_ctrl+ctrl, q_a, h);
    controlledUnitary(qr, q_ctrl+ctrl, q_a+1, h);

}