# Boosting-Algorithm
Developing boosting algorithms in order to make Quantum Linear Classifiers more strong


## Approach
We will be using QISKIT and QuEST in order to do the implementation of the boosting algorithm. QuEST will be the main language for the development of this project and will be used as it provides excellent sclabaility for simulation of quantum systems over different hardwares such as GPUs and HPC systems. The reson we will be using QISKIT too is that it is much easier use and we both are much more proficeint with QISKIT and python in general. Furthermore we are also facing isses with running QuEST in our systems(although we can run it we have to compile the file each time using cmake and make commands due to which it takes more time). 

## Parts 
We will be working on different parts of the project one by one as guided by Sagnik sir.

### QRAM
The bucket brigade Quantum RAM[1] has been implemeneted as a generalized function which is dependant on the number of input registers. We can adjust the address registers as well as the memory cells to see how the states change and hwo we are able to get to get to the correct memory cell. The circuit has been mainly motivated from[2] and for the case of two address qubits is identical to [3]. Furthermore [3] also takes about making this a read/write memory but not really sure if this will be necessary for our use so have not added that feautre. 

##### Limitations
1.We are only able to store the classical data and nothing quantum 
2.There are a lot of further limitiations including noise issues mentioned in [2]


## References
1.[Quantum random access memory](https://arxiv.org/pdf/0708.1879.pdf)

2.[On the robustness of bucket brigade quantum RAM](https://arxiv.org/pdf/1502.03450.pdf) 

3.[IMPLEMENTING QRAM IN QISKIT WITH CODE(a blogpost)](https://quantumcomputinguk.org/tutorials/implementing-qram-in-qiskit-with-code)
