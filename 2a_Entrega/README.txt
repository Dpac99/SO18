1) File and directories organization
.
|-- CircuitRouter-ParSolver
|-- CircuitRouter-SeqSolver
|-- README.txt
|-- doTest.sh
|-- inputs
|-- lib
`-- results

2) How to run and compile
    by doTest:
        i) make -C CircuitRouter-ParSolver
        ii) make -C CircuitRouter-SeqSolver
        iii) chmod +x doTest.sh
        iv) ./doTest.sh [THREADS] [PATH_TO_INPUTFILE]
        WARNING: Do not move ./doTest.sh. It will not work

    by commandline:
        i) make -C CircuitRouter-ParSolver
        ii) ./CircuitRouter-ParSolver -t [NUMTHREADS] [PATH_TO_INPUTFILE]

3) CPU info

model - modelname @ clockrate           : 142 - Intel(R) Core(TM) i7-7500U CPU @ 2.70GHz
cpu cores                                                    : 2

Linux mypchost 4.18.11-arch1-1-ARCH #1 SMP PREEMPT Sat Sep 29 21:01:35 UTC 2018 x86_64 GNU/Linux

