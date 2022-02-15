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

the build fails with:

```
[ 50%] Building CUDA object CMakeFiles/main.dir/main.cpp.o
/usr/local/cuda-10.2/bin/nvcc -forward-unknown-to-host-compiler  -I/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/app -isystem=/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildV3/install/include --generate-code=arch=compute_75,code=[compute_75,sm_75] -std=c++14 -MD -MT CMakeFiles/main.dir/main.cpp.o -MF CMakeFiles/main.dir/main.cpp.o.d -x cu -c /users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/app/main.cpp -o CMakeFiles/main.dir/main.cpp.o
ptxas fatal   : Unresolved extern function '_ZN2v38scrambleEv'
gmake[2]: *** [CMakeFiles/main.dir/main.cpp.o] Error 255
gmake[2]: Leaving directory `/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildApp'
gmake[1]: *** [CMakeFiles/main.dir/all] Error 2
gmake[1]: Leaving directory `/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildApp'
gmake: *** [all] Error 2
```

Why can't the compiler see the `v3::scramble` function without enabling `CUDA_SEPARABLE_COMPILATION` (see commented line in `app/CMakeLists.txt`) for the executable?  The verbose build sequence with separable compilation enabled is pasted below.

It seems that the separable compilation property is transitive if a device function needs to be used from the library; all code linking against a library built with seperable compilation needs to enable separable compilation (`-dc`).  Is that correct?

```
[ 33%] Building CUDA object CMakeFiles/main.dir/main.cpp.o
/usr/local/cuda-10.2/bin/nvcc -forward-unknown-to-host-compiler  -I/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/app -isystem=/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildV3/install/include --generate-code=arch=compute_75,code=[compute_75,sm_75] -std=c++14 -MD -MT CMakeFiles/main.dir/main.cpp.o -MF CMakeFiles/main.dir/main.cpp.o.d -x cu -dc /users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/app/main.cpp -o CMakeFiles/main.dir/main.cpp.o
[ 66%] Linking CUDA device code CMakeFiles/main.dir/cmake_device_link.o
/opt/scorec/cmake/3.20.0/bin/cmake -E cmake_link_script CMakeFiles/main.dir/dlink.txt --verbose=1
/usr/local/cuda-10.2/bin/nvcc -forward-unknown-to-host-compiler  --generate-code=arch=compute_75,code=[compute_75,sm_75] -Xcompiler=-fPIC -Wno-deprecated-gpu-targets -shared -dlink CMakeFiles/main.dir/main.cpp.o -o CMakeFiles/main.dir/cmake_device_link.o  /users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildV3/install/lib64/libv3.a -lcudadevrt -lcudart_static -lrt -lpthread -ldl 
[100%] Linking CUDA executable main
/opt/scorec/cmake/3.20.0/bin/cmake -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=1
/opt/scorec/spack/v0154_2/install/linux-rhel7-x86_64/gcc-6.5.0/gcc-7.4.0-75ljprtmtjoqfmlhrnbkrvlnsvhppv2y/bin/g++ CMakeFiles/main.dir/main.cpp.o CMakeFiles/main.dir/cmake_device_link.o -o main  /users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildV3/install/lib64/libv3.a -lcudadevrt -lcudart_static -lrt -lpthread -ldl  -L"/usr/local/cuda-10.2/targets/x86_64-linux/lib/stubs" -L"/usr/local/cuda-10.2/targets/x86_64-linux/lib"
gmake[2]: Leaving directory `/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildApp'
[100%] Built target main
gmake[1]: Leaving directory `/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildApp'
/opt/scorec/cmake/3.20.0/bin/cmake -E cmake_progress_start /users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildApp/CMakeFiles 0
```
