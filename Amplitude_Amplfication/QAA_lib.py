#!/usr/bin/env python
# coding: utf-8

# In[1]:


import qiskit
from qiskit import QuantumCircuit, transpile, Aer, BasicAer, IBMQ, execute, QuantumRegister, ClassicalRegister
from qiskit.visualization import plot_histogram
from qiskit.tools.jupyter import *
from qiskit.visualization import *
from qiskit.visualization import plot_bloch_multivector
import pandas as pd
#import matplotlib as mpl
from math import pi
import numpy as np
from qiskit import QuantumCircuit, transpile

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


# IBMQ.save_account('0c5f09afe94bb9159f65f3ae983bdad4fd4fe1ef0fe97a1f9167ffc44c733c5ccb21d848987d3c1796fc3a40e2143fc5f902ddb2526a3df548c3b0ea53cba38f')
# provider = IBMQ.load_account()


# ## Diffuser
# 
# After the oracle the next thing that comes is the diffuser circuit. The overall operation can be written as 
# $$
# -A.S_0.A^{\\dag}
# $$
# where A is the matrix that produces the initial state. And $A^{\\dag}$ is the inverse. FUrtherore $S_0$ is the unitary which flips |11...11> to -|11....11>. Together they flip all the states about mean of the amplitudes. 
# 
# This cures our original problem of having weird stuff happen when the solution was not present in the data set. 
# 
# **New Porblem** - sometimes changing the no. of iterations actually changes the results. If you extract '2' out of 0,1,2,3 superpostion. No. of reps = 1 and 4 works fine but no. of reps = 2 and 3 gives us the equa; superposition state. 
# 

# In[16]:


def Inverse_A(A):
    
    # no need to change this automaticlly becomes A_dag
    qc = A
    
      
    qc_g = qc.to_gate(label = 'Adag')
    AAd = qc_g.inverse()
    AAd.name = '$A^{\\dag}$'
    
    return AAd


# In[5]:


def S_0(n):
    
    ## one big probem in rohans code was that he has not initialise theqc so 
    ## it was taking values from other circuits

    qc = QuantumCircuit(n)
    ctr = []
    for k in range(1,n):
        ctr.append(k)


    qc.barrier()
    qc.x(list(range(n)))
    qc.h(0)
    qc.mct(ctr,0)
    qc.h(0)
    qc.x(list(range(n)))

    qc.barrier()

    
    
    return qc


# In[69]:


# S_0(19).draw('mpl')


# In[57]:


def Amplitude_Amplification(A, oracle, reps):
    '''
    This will be a generalized function for running QAA using A and Oracle 
    Oracle and A are both quantum circuits
    
    '''
    
    if A.width() != oracle.width():
        raise Exception("The width of initial circuit and oracle does not match!!")
        
        
    n = A.width()
    qc = QuantumCircuit(n)
    qc  = qc.compose(A)

    
    # calculating the inverse of A
    Ad = Inverse_A(A)
    
    for rep in range(reps):
        qc.barrier()
        
        qc.append(oracle, list(range(n)))
        qc.barrier()
        qc.append(Ad,list(range(n)))
        qc  = qc.compose(S_0(n))
        qc  = qc.compose(A)

    return qc


# In[68]:


# qc11.measure_all()
# backend = BasicAer.get_backend('qasm_simulator')
# job = execute(qc11,backend,shots = 1000)
# counts = job.result().get_counts()
# # plot_histogram(counts)


# In[ ]:




