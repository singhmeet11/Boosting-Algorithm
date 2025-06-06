# Boosting-Algorithm
Developing boosting algorithms in order to make Quantum Linear Classifiers more stron.



## Approach
We will be using QISKIT and QuEST in order to do the implementation of the boosting algorithm. QuEST will be the main language for the development of this project and will be used as it provides excellent sclabaility for simulation of quantum systems over different hardwares such as GPUs and HPC systems. The reson we will be using QISKIT too is that it is much easier use and we both are much more proficeint with QISKIT and python in general. Furthermore we are also facing isses with running QuEST in our systems(although we can run it we have to compile the file each time using cmake and make commands due to which it takes more time). 

## Parts 

### QRAM
The bucket brigade Quantum RAM[1] has been implemeneted as a generalized function which is dependant on the number of input registers. We can adjust the address registers as well as the memory cells to see how the states change and hwo we are able to get to get to the correct memory cell. The circuit has been mainly motivated from[2] and for the case of two address qubits is identical to [3]. Furthermore [3] also takes about making this a read/write memory but not really sure if this will be necessary for our use so have not added that feautre. 

##### Some important things
1.We have verified the results by setting the address qubits to '01' which points to m_1 memory cell, and then we have made the memory cell 1 and 0 and along with that the output changes, we also played around and found good results. 

2.Different functions control different parts of the QRAM code

##### Limitations
1.We are only able to store the classical data and nothing quantum 

2.There are a lot of further limitiations including noise issues mentioned in [2]

##### Conversion from QISKIT to QuEST
1.The main difference is that in qiskit we can initialize the various qubit registers and apply multi qubit gates instead but in QuEST multiqubit gates can not be applied in between multiple qubits. Due to which while converting between them make variables which mark the starting points of the Qubit regsiters.

2.Tried using MultiControlledMultiNot gate in QuEST for creating Toffoli but that did not work due to which used the MultiControlledUnitary and defined unitary as x gate's pauli matrix.   

### QSearch - Ampltitude Amplification, Amplitude Estimation
Grover's Algorithm lets us search through an unsorted database with a quadratic speedup over classical methods, i.e database of N elements can be searched through by order of N^(1/2) queries, use of an Oracle is involved. If we have N elements, represented by bitstrings of n length, (N=2^n), and the number of solutions, i.e. Good States, is known to be M, then Quantum Amplitude Amplification can be applied to amplify the probability of finding the Good State (Solution states) and thus reduce the Amplitude and probability of finding the Bad state (i.e not solutions). How it's done is; the initial state is prepared (as an equal superposition of 2^n states in our example), then the Oracle 'marks' the Good States. Then this whole state undergoes a reflection about a state that is orthogonal to the superposition of good states (this Orthogonal state is made by taking a superposition of all states left after removing the good states). By now our state is in a phase opposite to the bad state and Amplitude of the bad state has been reduced. Then the state undergoes another reflection about itself. This way the the state comes closer to the Good State by a certain angle, now if the number of solutions is known, then the process of the two Reflections can be repeated by a suitable number of times to reach closer and closer to the good state.

But when the number of solutions, M, is not known and has to be counted, Quantum Amplitude Estimation comes into play. The two reflections are combined into 1 'Grover Operator' and its eigenvalues give us the angle by which it rotates any particular state. The eigenvalues of the operator are estimated by the Quantum Phase Estimation process. The estimated angle then tells us about the number of solutions, and then the number of iterations of the Amplitude amplification process required to locate the Solution states too.
The complete Estimation-Amplification process is a useful subroutine for Quantum Machine Learning algorithms.


## References
1.[Quantum random access memory](https://arxiv.org/pdf/0708.1879.pdf)

2.[On the robustness of bucket brigade quantum RAM](https://arxiv.org/pdf/1502.03450.pdf) 

3.[IMPLEMENTING QRAM IN QISKIT WITH CODE(a blogpost)](https://quantumcomputinguk.org/tutorials/implementing-qram-in-qiskit-with-code)

4.[Quantum Amplitude Amplification and Estimation](https://arxiv.org/abs/quant-ph/0005055)
