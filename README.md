# Monodomain solved with SPH
============================

Solves the monodomain equations with the Smoothed Particle Hydrodynamics method. The monodomain equation is defined as:

grad dot (sigma(grad(Vm))) = Im

where Vm is the transmembrane potential and Im is the transmembrane current, given by Im = beta(Cm(dVm/dt) + iion - stim).
beta is the surface-to-volume ratio, Cm is the membrane capacitance, iion is the ionic membrane current governed by the membrane model, and stim is an externally applied stimulus current.
  
The monodomain model can be used to test membrane models, before implementing them into the more realistic bidomain model.

The application also solves the monodomain equations by Finite Differences and the solution is used for comparisson purposes. For the Finite Differences solver, the work by user ["leongraham"](https://github.com/leongraham/monodomain) is used. The proposed code is based on said work, and uses the same model setup.

### Model setup

The parameters for the model are defined in the 'constants.h' file, such as membrane capacitance, surface-to-volume ratio, and membrane model parameters. The model setup is define in main.cc

### Installation

run 'make' from command line in the working directory:

```
make
```

this will produce an executable file named 'run', which can be ran from the command line with:

```
./run
```

The compiler flags in the Makefile can be edited if needed.

### Visualisation

matplotlib is used to display the potentials:

```
python3 plot.py Vm.out.sph.txt
```

To display the potentials for the finite difference solver:

```
python3 plot.py Vm.out.txt
```