
# Parser and Stamping of Netlist And LU Solver

This project is to write a parser which can be suitable for circuit description inputs. After parsing the input file, it generates a MNA matrix presentation of the circuit. Perform LU factorization using the Gauss's method. In part a) the parser and stamping functionality is explained and part b) explains the LU solver.
## a)
In this part I write a parser that can read a circuit specification in terms of a simple “language” that is described. The language is quite limited and restrictive, and represents the bare minimum that will be needed for subsequent part. The parser is not case-sensitive, just like netlists in Spice softwares like HSPICE. and it interprets any contiguous sequence of spaces or tabs as equivalent to a single space. 
* Every line of the input file is describe a single circuit element,
*  The description of every circuit element is given wholly within a single input line.
*   The order of lines in the input file is immaterial and any characters following a % in an input line is considered as comments and ignored. 
* Circuit node names are non-negative integers, from the set {0,1,2,...}, and the node name 0 is reserved and used for the ground or reference node.


#### The accepted circuit elements and their specifications are given below.

In this, the symbol <node.*>, where * can be any single alphanumeric character, denotes a node name. 
Specifically: 

<node.+> denotes the node that is the positive voltage reference point for the element and <node.-> denotes the negative reference node. The positive direction of current in any element is assumed to be from `<node.+>` to `<node.->`. The symbol `<int>` denotes a non-negative integer, and `[<value>]`denotes a non-negative real number. The `[<value>]` given for a circuit parameter, like resistance or capacitance, is in the standard units: Volt, Ampere, Ohm, Farad, or Henry, but it is not include the corresponding unit. Finally, anything inside brackets, such as `[G2]` or `[<value>]` is an optional field.
    
* ### Voltage source:
    Only independent DC voltage sources are allowed, specified as: 
    `V<int> <node.+> <node.-> <value>`
* ### Current source: 
    Only independent DC current sources are allowed, specified as:
    ` I<int> <node.+> <node.-> <value>`
* ### Resistor: 
    Only linear resistors are allowed, specified as:
   `R<int> <node.+> <node.-> <value> [G2 % this is a group 2 element]
`
    * ### Capacitor: 
    Only linear capacitors are allowed, specified as:
   
    `C<int> <node.+> <node.-> <value>`
* ### Inductor: 
    Only linear inductors are allowed, and they should be specified as:      
    `L<int> <node.+> <node.-> <value>`
* ### Diode:
    The diode model, and its parameter values, is not part of the input description. Instead, the model will be built into any subsequent simulation code that you will write and only the terminals should be specified here. Optionally, a scale factor can also be included so as to allow the specification of diodes that are larger than minimum size. The specification is:

        D<int> <node.+> <node.-> [<value>]
    
* ### BJT:
    Similar to the diode model, only the terminals and an optional scale factor are given. Let QN denote an npn device and QP denote pnp; the specification is:

        QN<int> <node.C> <node.B> <node.E> [<value>]
    
        QP<int> <node.C> <node.B> <node.E> [<value>]

    where the nodes represent the collector, base, and emitter terminals, respectively.
* ### MOSFET:
    Similar to the above, we give only the terminals and an optional scale factor, and the body terminal is to be ignored:

     MN<int> <node.D> <node.G> <node.S> [<value>]
    MP<int> <node.D> <node.G> <node.S> [<value>]

    where MN denotes an n-channel device and MP is p-channel, and the nodes     represent the drain, gate, and source terminals, respectively.

    The parser creates a data structure, as a linked list of records,         where each record describes a separate circuit element, including its       terminals and parameter values. 
    
## b)
Using the parser developed previously in (a) as a front-end, this program is written to accept a description of any resistive linear circuit, with no controlled sources, and constructs the corresponding MNA system using element stamps. In other words, the program accepts any network of linear resistors, independent current sources, and independent voltage sources. The program makes use of the linked-list data structure created by the parser. It interpret the optional field [G2] introduced earlier, in the specification of the parser, as indicating that an element belongs to group 2. The [G2] flag is not required for membership in group 2.

* With the circuit description file given in input_circuitDiscription.txt, the correct solution is given in Fig. 2 and an example of the corresponding output format are given in: 
   - output_mnaMatrix.txt
    - output_xVector.txt
    - output_rhs.txt.

    
    
    #### Fig1: MNA Matrix representation example

    ![fig1](https://i.imgur.com/CmVHIYC.png)

    
     #### output_mnaMatrix.txt for the above example
    ![Fig2](https://i.imgur.com/Vc4ZUE3.png)
    
    
![fig3](https://i.imgur.com/Ehw3JMG.png)

   ## Part 2
    
    
In this part of theproject, a LU solver is implemented that can solve any linear resistive circuit with no controlled sources using modified nodal analysis (MNA). it uses part 1 to do the parser and stamping functions.
### LU Solver
    
The linear solver uses Gauss’s method for LU-factorization, partial (row) pivoting, and using an in-place computation so that L and U simply over-write the system matrix. Partial pivoting (using row-exchange) is performed to find the best pivot for accuracy, and not only to avoid a zero pivot. but also to ignore issues of sparsity.
The implementation is general, in the sense that it accepts any linear circuit description consisting of any combination of linear resistors and independent voltage and current sources. 


The parser as well as LU fonction can be used on any circuits meeting the criterias mentioned above.
    
Thanks a lot for reading. 
## Resources

[1] V. Litovski and M. Zwolinski, “VLSI Circuit Simulation and
Optimization,” Chapman & Hall, 1997.
[2] L.T. Pillage, R.A. Rohrer, and C. Visweswariah, “Electronic Circuit
and System Simulation Methods,” McGraw-Hill, 1995.
[3] J. Ogrodzki, “Circuit Simulation Methods and Algorithms,” CRC,
1994.
[3] W. J. McCalla, “Fundamentals of Computer-Aided Circuit
Simulation,” Kluwer, 1988.
[4] L. O. Chua and P.-M. Lin, “Computer-Aided Analysis of Electronic
Circuits,” Prentice-Hall, 1975.
    
#### Circuit theory
• O. Wing, “Classical circuit Theory,” Springer, 2008.
• L. O. Chua, “Linear and Nonlinear Circuits,” McGraw-Hill, 1987.
Circuit Simulation Administrative Matters Yu-Min Lee 11
    
####  Frequency domain simulation
• K.S. Kundert, J. K. White, and A. Sangiovanni-Vincentelli, “Steadystate Methods for Simulating Analog and Microwave Circuits,”
Kluwer, 1990.
    
#### Equivalent Circuit
• T. Ytterdal, Y. Cheng, and T. A. Fjeldly, “Device Modeling for Analog
and RF CMOS Circuit Design,” Wiley, 2004.
    
#### Cross domain simulation
• G. F. Carey, W. B. Richardson, C. S. Reed, and B. J. Mulvaney,
“Circuit, Device and Process Simulation – Mathematical and
Numerical Aspects,” Wiley, 1996.
    
#### Electromechanical inverse problem
• N.V. Korovkin, V.L. Chechurin, and M. Hayakawa, “Inverse
Problems in Electric Circuits and Electromagnetics,” Springer, 2006.
Circuit Simulation Administrative Matters Yu-Min Lee 12

 
#### Numerical Methods
• R. L. Burden and J. D. Faires, “Numerical Analysis,” 8th Ed.,
Thomson, 2005.
• R. S. Varga, “Matrix Iterative Analysis,” Springer, 2001.
• G. H. Golub and C. F. Van Loan, “Matrix Computations,” 2nd Ed.,
The Johns Hopkins University Press, 1989
    
