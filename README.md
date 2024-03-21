# :sheep: FPMAS Prey Predator model

Implementation of a basic prey predator model using FPMAS. The model
demonstrates FPMAS features, and allows the selection of various synchronization
modes and load balancing algorithms.

## Build

Ensures that the [FPMAS](https://github.com/FPMAS/FPMAS) platform is properly
installed. An additional `-DCMAKE_PREFIX_PATH=custom/installation/path/` might
be specified to `cmake` if `FPMAS` is not installed in a standard directory
(the installation path corresponds to the `-DCMAKE_INSTALL_PREFIX` eventually
specified when building `FPMAS`).

```
git clone --recurse-submodules https://github.com/FPMAS/fpmas-classic-prey-predator
cd fpmas-classic-prey-predator
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Configuration

Model parameters are described in the [config.yml](blob/master/config.yml) file.

## Run

```
mpiexec -n <N> ./fpmas-prey-predator <config_file>
```
- `N`: processes count
- `config_file`: a `.yml` configuration file

## Output

The generated `model.csv` file contains the following fields:

- `time`: Time step.
- `grass`: Count of grown grasses.
- `prey`: Count of preys.
- `predator`: Count of predators.

If the `lb_output` option is set to true, a `lb.<proc index>.csv` file is
generated on each process with the following fields:

- `time`: Time step
- `agent_workload`: Total count of prey and predators agents on the process.
- `cell_workload`: Total count of cells agents on the process.
- `perception_communication`: Total count of PERCEPTION edges between a local
  prey/predator agent and an agent on an other process.
- `cell_communication`: Total count of CELL edges between a local
  cell and a cell on an other process.

# Contact

For more information about this model or its implementation, please contact:
- Paul Breugnot (developer): paul.breugnot@univ-fcomte.fr
- Laurent Philippe (supervisor): laurent.philippe@univ-fcomte.fr
- Bénédicte Herrmann (supervisor): benedicte.herrmann@univ-fcomte.fr
- Christophe Lang (supervisor): christophe.lang@univ-fcomte.fr
