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

The build of `mainNoDevCall.cpp` also fails with seperable compilation disabled despite the fact that it does not make any direct calls to device functions.

```
Consolidate compiler generated dependencies of target mainNoDevCall
gmake[2]: Leaving directory `/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildApp'
/usr/bin/gmake  -f CMakeFiles/mainNoDevCall.dir/build.make CMakeFiles/mainNoDevCall.dir/build
gmake[2]: Entering directory `/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildApp'
[ 20%] Building CUDA object CMakeFiles/mainNoDevCall.dir/mainNoDevCall.cpp.o
/usr/local/cuda-10.2/bin/nvcc -forward-unknown-to-host-compiler  -I/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/app -isystem=/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildV3/install/include --generate-code=arch=compute_75,code=[compute_75,sm_75] -std=c++14 -MD -MT CMakeFiles/mainNoDevCall.dir/mainNoDevCall.cpp.o -MF CMakeFiles/mainNoDevCall.dir/mainNoDevCall.cpp.o.d -x cu -c /users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/app/mainNoDevCall.cpp -o CMakeFiles/mainNoDevCall.dir/mainNoDevCall.cpp.o
[ 40%] Linking CUDA executable mainNoDevCall
/opt/scorec/cmake/3.20.0/bin/cmake -E cmake_link_script CMakeFiles/mainNoDevCall.dir/link.txt --verbose=1
/opt/scorec/spack/v0154_2/install/linux-rhel7-x86_64/gcc-6.5.0/gcc-7.4.0-75ljprtmtjoqfmlhrnbkrvlnsvhppv2y/bin/g++ CMakeFiles/mainNoDevCall.dir/mainNoDevCall.cpp.o -o mainNoDevCall  /users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildV3/install/lib64/libv3.a -lcudadevrt -lcudart_static -lrt -lpthread -ldl  -L"/usr/local/cuda-10.2/targets/x86_64-linux/lib/stubs" -L"/usr/local/cuda-10.2/targets/x86_64-linux/lib"
/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildV3/install/lib64/libv3.a(v3.cpp.o): In function `__sti____cudaRegisterAll()':
tmpxft_000060ab_00000000-5_v3.cudafe1.cpp:(.text+0x60e): undefined reference to `__cudaRegisterLinkedBinary_37_tmpxft_000060ab_00000000_6_v3_cpp1_ii_ec982148'
collect2: error: ld returned 1 exit status
gmake[2]: *** [mainNoDevCall] Error 1
gmake[2]: Leaving directory `/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildApp'
gmake[1]: *** [CMakeFiles/mainNoDevCall.dir/all] Error 2
gmake[1]: Leaving directory `/users/cwsmith/ThrowAwayCode/cudaLibrary/code-samples/posts/separate-compilation-linking/buildApp'
gmake: *** [all] Error 2
```

This error appears to be explained in the `nvcc` docs here:

https://docs.nvidia.com/cuda/cuda-compiler-driver-nvcc/index.html#implicit-cuda-host-code

| 6.6.3. Implicit CUDA Host Code
| 
| A file like b.cu above only contains CUDA device code, so one might think that
| the b.o object doesn't need to be passed to the host linker. But actually there
| is implicit host code generated whenever a device symbol can be accessed from
| the host side, either via a launch or an API call like cudaGetSymbolAddress().
| This implicit host code is put into b.o, and needs to be passed to the host
| linker. Plus, for JIT linking to work all device code must be passed to the host
| linker, else the host executable will not contain device code needed for the JIT
| link. So a general rule is that the device linker and host linker must see the
| same host object files (if the object files have any device references in
| them—if a file is pure host then the device linker doesn't need to see it). If
| an object file containing device code is not passed to the host linker, then you
| will see an error message about the function __cudaRegisterLinkedBinary_name
| calling an undefined or unresolved symbol __fatbinwrap_name.

