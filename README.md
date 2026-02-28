# PETDataAnalysis
The project enables users to generate basic data distributions from measurements taken with a small PET setup composed of two MaPMT detectors and LHCb RICH readout electronics (MAROC and Chimaera2 boards). 

## Installation
This program depends on several ROOT libraries, so ROOT must be installed and accessible on your system.

After installing ROOT, configure your environment by running:

```source <path-to-ROOT>/thisroot.sh```

Now clone the repository and compile the code:

```
git clone https://github.com/hannah794/PETDataAnalysis.git 
cd PETDataAnalysis 
mkdir build 
cd build 
cmake ..
make
cd ..
```

## Analysis data 
Create directory for storing the MaPMT data:
```
mkdir data
```
Place the ROOT files generated from the converted MaPMT data in this directory.

Use the following naming convention and file organisation structure:
```
data/Run<run-no>/out-feb-<chimaera2-no>-000<run-no>.root 
```
where \<run-no\> is a 3-digit-long run number (support for longer run numbers should be implemented in the future) and \<chimaera2-no\> is the serial number of the used chimaera2 board.

## Additional info
Parameter quadrant used in the code refers to a slot on the MaPMT baseboard and should not be confused with the mapping used in the .mdf -> .root file conversion. 