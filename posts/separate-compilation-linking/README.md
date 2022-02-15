example env on SCOREC RHEL7 - need gcc, cuda, and cmake

```
# Set Environment for Rhel-7 systems, gcc 7.4
module unuse /opt/scorec/spack/lmod/linux-rhel7-x86_64/Core 
module use /opt/scorec/spack/v0154_2/lmod/linux-rhel7-x86_64/Core 
module load \
gcc/7.4.0 \
cmake/3.20.0 \
cuda/10.2
```

build v3 library

```
cmake -S v3 -B buildV3 -DCMAKE_CUDA_ARCHITECTURES=75 -DCMAKE_INSTALL_PREFIX=$PWD/buildV3/install
cmake --build buildV3 --target install
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$PWD/buildV3/install
```

build app

```
cmake -S app -B buildApp -DCMAKE_CUDA_ARCHITECTURES=75
cmake --build buildApp --verbose
```

This fails with:

```
ptxas fatal   : Unresolved extern function '_ZN8particle7advanceEf'
```
