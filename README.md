# G4_HPGe

This project is a simulation of High Purity Germanium (HPGe) detectors using Geant4.

## Table of Contents
- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## Introduction
G4_HPGe is a Geant4 simulation designed to model the behavior of simple HPGe detectors. These detectors are widely used in nuclear physics experiments for their excellent energy resolution.

## Installation
To install and set up the project, follow these steps:

1. **Clone the repository:**
    ```sh
    git clone https://github.com/yourusername/G4_HPGe.git
    cd G4_HPGe
    ```

2. **Install dependencies:**
    Ensure you have Geant4 installed. Follow the [Geant4 installation guide](https://geant4.web.cern.ch/support/download). Alternatively, you can use the provided ```environment.yml``` file to install the conda environment.

3. **Build the project:**
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage
To start the GUI, execute the following command in the build directory:
```sh
./G4_HPGe
```
The usual Geant4 interface with the HPGe detector should appear.
For examples of launching several simulations, refer to ```Run.ipynb``` notebook in analysis directory. Additionally, other notebooks in the same directory can give an example on how to handle a simple analysis of the simulations.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.