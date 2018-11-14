# Liquid Cellular Automata

Implements the density classification task in a liquid cellular
automaton.

## Dependencies

* [SFML](https://www.sfml-dev.org/) is used for the visualization and
  must be installed to compile.
* pthreads

The code has been built and tested with the viz on Ubuntu 16.05,
18.04, FreeBSD 11.2. It has been built on OSX as well.

## Build
```bash
$ cd liquid-ca
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release .. && make
```

To run tests do `make test`

## Experiments
In general every experiment is its own executable they take the
following standard options:

| Option                      | Model parameter                  |
| --------------------------- | -------------------------------- |
| `--num-agents <N>`          | N                                |
| `--arena-size <L>`          | L                                |
| `--communication-range <r>` | r                                |
| `--max-time <T>`            | T                                |
| `--correlated <sigma>`      | use CRW with sigma std. dev.     |
| `--iterations <n>`          | replicas to run for each density |
| `--seed <seed>`             | random seed                      |

Some experiments take additional options.

### Velocity experiment
Basic experiment that evaluates the performance of the LCA for initial
densities in the range [0,1].

`$ ./velocity_experiment <speed> [options listed above]`

Outputs the fraction of correctly classified initial conditions for
each initial density.

### Visualization
Currently will output a png of the viz every 10 time steps (sorry, I
should make that optional).

`./sim_viz <speed> [optons from above]`

Addtionally takes the following options

* `--frame-rate <frames per second>`
* `--initial-density <rho>`

