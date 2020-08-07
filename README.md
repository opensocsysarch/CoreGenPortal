# CoreGen Portal Graphical User Interface

## Getting Started

## Prerequisites
* C++11 Compiler (LLVM/Clang and GCC are tested)
* CMake 3.4.3+
* CoreGen (and it's constituent dependencies)

## Building

The majority of the CoreGen Portal library and tool 
dependencies are satisfied by either by CoreGen or 
by the wxWidgets GUI interface library.  wxWidgets 
comes packages as various different libraries depending 
upon the target build target (and the respective platform GUI). 
Generally speaking, the CoreGen Portal package is built 
using the following steps: 

1. Clone the CoreGenPortal repository.  There are currently no git dependencies
2. Create a ``build`` directory within the CoreGen source tree (and change to that directory)
3. Execute cmake to generate the target-specific makefiles
4. Execute the build

### Ubuntu

We currently support building and installing on LTS versions of Ubuntu.  While it
is certainly possible to build/install on intermediate release and/or development
versions, we do not currently test these scenarios.

#### Ubuntu 16.04
1. Install the system dependencies
```
sudo apt-get install -y software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt-get install libwxgtk3.0-0v5 libwxgtk3.0-dev libwxgtk3.0-gtk3-0v5 libwxgtk3.0-gtk3-dev libgraphviz-dev g++-9
```
2. [Optional] Update the default GCC/G++ pointer to default to GCC/G++-9
```
sudo update-alternatives --remove-all gcc
sudo update-alternatives --remove-all g++
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 20
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 20
sudo update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30
sudo update-alternatives --set cc /usr/bin/gcc
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30
sudo update-alternatives --set c++ /usr/bin/g++
sudo update-alternatives --config gcc
sudo update-alternatives --config g++
```
3. Clone the CoreGenPortal repository
```
git clone https://github.com/opensocsysarch/CoreGenPortal.git
```
4. Setup your build tree
```
cd CoreGenPortal
mkdir build
cd build
```
5. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib/llvm-8/cmake -DCOREGEN_INSTALL_PATH=/path/to/CoreGen/install ../
```
6. Execute the build
```
make
```

#### Ubuntu 18.04
1. Install the system dependencies
```
sudo apt-get install libwxgtk3.0-0v5 libwxgtk3.0-dev libwxgtk3.0-gtk3-0v5 libwxgtk3.0-gtk3-dev libgraphviz-dev
```
2. Clone the CoreGenPortal repository
```
git clone https://github.com/opensocsysarch/CoreGenPortal.git
```
3. Setup your build tree
```
cd CoreGenPortal
mkdir build
cd build
```
4. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib/llvm-8/cmake -DCOREGEN_INSTALL_PATH=/path/to/CoreGen/install ../
```
5. Execute the build
```
make
```

#### Ubuntu 20.04
1. Install the system dependencies
```
sudo apt-get install libwxgtk3.0-gtk3-dev libwxgtk3.0-gtk3-0v5 libgraphviz-dev
```
2. Clone the CoreGenPortal repository
```
git clone https://github.com/opensocsysarch/CoreGenPortal.git
```
3. Setup your build tree
```
cd CoreGenPortal
mkdir build
cd build
```
4. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib/llvm-8/cmake -DCOREGEN_INSTALL_PATH=/path/to/CoreGen/install ../
```
5. Execute the build
```
make
```

#### Ubuntu 20.04
1. Install the system dependencies
```
sudo apt-get install libwxgtk3.0-gtk3-dev libwxgtk3.0-gtk3-0v5
```
2. Clone the CoreGenPortal repository
```
git clone https://github.com/opensocsysarch/CoreGenPortal.git
```
3. Setup your build tree
```
cd CoreGenPortal
mkdir build
cd build
```
4. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib/llvm-8/cmake -DCOREGEN_INSTALL_PATH=/path/to/CoreGen/install ../
```
5. Execute the build
```
make
```

### Centos7

Centos7 (CentOS 7.4, 1704) has somewhat of an older toolchain.  As a result,
the build requires some rather unique system packages.  Utilize the following
steps on vanilla CentOS 7.4 systems.

1. Install the EPEL repository (required to pull the wxGTK3 RPMs).
```
wget https://dl.fedoraproject.org/pub/epel/7/x86_64/Packages/e/epel-release-7-11.noarch.rpm
sudo rpm -Uvh epel-release-7-11.noarch.rpm
```
2. Install the system dependencies:
```
sudo yum -y install wxGTK3 wxGTK3-devel graphviz-devel
```
3. [Optional] For those wishing to build CentOS7 RPMS, you also need the following packages.
```
sudo yum install rpm-build rpmdevtools
```
4. Enable the SCL LLVM environment (this will create a fresh bash shell with the correct paths)
```
scl enable llvm-toolset-7 bash
```
5. Clone the CoreGenPortal repository
```
git clone https://github.com/opensocsysarch/CoreGenPortal.git
```
6. Setup your build tree
```
cd CoreGenPortal
mkdir build
cd build
```
7. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/opt/rh/llvm-toolset-7.0/root/usr/lib64/cmake/llvm -DCOREGEN_INSTALL_PATH=/path/to/CoreGen/install ../
```
8. Execute the build
```
make
```

### Centos8
Centos8 has a modern toolchain, much more so than Centos7.  However, the build requires 
packages from the EPEL repository.  Utilize the following steps on the CentOS 8 systems.

1. Install the EPEL repository (required to pull the wxGTK3 RPMs).
```
wget https://dl.fedoraproject.org/pub/epel/8/Everything/x86_64/Packages/e/epel-release-8-8.el8.noarch.rpm
sudo rpm -Uvh epel-release-8-8.el8.noarch.rpm
```
2. Install the system dependencies:
```
sudo yum -y install wxGTK3 wxGTK3-devel graphviz-devel
```
3. [Optional] For those wishing to build CentOS7 RPMS, you also need the following packages.
```
sudo yum install rpm-build rpmdevtools
```
4. Clone the CoreGenPortal repository
```
git clone https://github.com/opensocsysarch/CoreGenPortal.git
```
5. Setup your build tree
```
cd CoreGenPortal
mkdir build
cd build
```
6. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib64/cmake/llvm -DCOREGEN_INSTALL_PATH=/path/to/CoreGen/install ../
```
7. Execute the build
```
make
```

### Darwin (OSX)

#### Darwin (OSX) 10.13
1. Install the homebrew package: https://brew.sh/
2. Install the ``llvm@7``, ``llvm@8``, ``wxmac`` and ``graphviz`` packages.  It is very important on 10.13 to install both versions of LLVM.  
```
brew install llvm@7 llvm@8 wxmac graphviz
```
3. Clone the CoreGenPortal repository
```
git clone https://github.com/opensocsysarch/CoreGenPortal.git
```
4. Setup your build tree
```
cd CoreGenPortal
mkdir build
cd build
```
5. Execute CMake to generate the makefiles.  Note the use of LLVM@7 as the base compiler and LLVM@8 for the runtime target.  This is important as OSX 10.13 does not support using C++17 features and LLVM@8 for compilation.
```
CC=/usr/local/opt/llvm\@7/bin/clang CXX=/usr/local/opt/llvm\@7/bin/clang++ cmake -DLLVM_DIR=/usr/local/opt/llvm\@8/lib/cmake/llvm -DCOREGEN_INSTALL_PATH=/path/to/CoreGen/install ../
```
6. Execute the build
```
make
```


#### Darwin (OSX) 10.15
1. Install the homebrew package: https://brew.sh/
2. Install the ``llvm@8``, ``wxmac`` and ``graphviz`` packages.
```
brew install llvm@7 llvm@8 wxmac graphviz
```
3. Clone the CoreGenPortal repository
```
git clone https://github.com/opensocsysarch/CoreGenPortal.git
```
4. Setup your build tree
```
cd CoreGenPortal
mkdir build
cd build
```
5. Execute CMake to generate the makefiles.  Note the use of LLVM@8 for the actual compilation.  This provides sufficient C++17 support.
```
CC=/usr/local/opt/llvm\@7/bin/clang CXX=/usr/local/opt/llvm\@7/bin/clang++ cmake -DLLVM_DIR=/usr/local/opt/llvm\@8/lib/cmake/llvm -DCOREGEN_INSTALL_PATH=/path/to/CoreGen/install ../
```
6. Execute the build
```
make
```


## CMake Build Options

### Standard Build Options
* -DCMAKE\_BUILD\_TYPE=Release : Builds a release build with optimization (Default)
* -DCMAKE\_BUILD\_TYPE=Debug : Builds with debugging options (-O0 -g, etc)
* -DCMAKE\_INSTALL\_PREFIX=/path/to/install : Forces alternate install locations
* -DCOREGEN\_INSTALL\_PATH=/path/to/coregen/install : The default is /opt/coregen
* -DLLVM\_DIR=/path/to/llvm/cmake/scripts : Sets the path for the LLVM cmake scripts.  This must be set for the build to be successful.  This is generally difficult to derive automatically.
* -DBUILD\_ALL\_TESTING=ON : Enables the top-level GUI test harness

### Documentation Options
* -DBUILD\_DOCUMENTATION=ON  : Enables Doxygenized documentation.  ``make doc`` will build the documentation (requires Doxygen to be installed)

### Developer Build Options
* -DYAML\_PATH=/path/to/alternate/yaml-cpp : Set an alternate yaml-cpp library path
* -DCOREGEN\_BUILD\_RPM=ON : Enable RPM builds (CentOS)
* -DCOREGEN\_BUILD\_DEB=ON : Enable DEB builds (Ubuntu)
* -DCOREGEN\_BUILD\_DMG=ON : Enable DMG builds (Darwin)
* -DCOREGEN\_BUILD\_TGZ=ON : Enable TGZ builds (generic)

### Debugging Options
* -DBUILD\_COREGEN\_MEMORY\_SANITY=ON : Enables Clang's memory sanitizer testing.  You MUST be uses Clang as your target C++ compiler!!

## Contributing

## Testing
* -DBUILD\_PORTAL\_GUI\_TESTS=ON : Enables the Python3 GUI test harness

## License
CoreGenPortal is licensed under an Apache-style license - see the [LICENSE](LICENSE) file for details

## External Packages
* None at this time

## Authors
* *John Leidel* - *Chief Scientist* - [Tactical Computing Labs](http://www.tactcomplabs.com)
* *David Donofrio* - *Chief Hardware Architect* - [Tactical Computing Labs](http://www.tactcomplabs.com)
* *Ryan Kabrick* - *Research Engineer* - [Tactical Computing Labs](http://www.tactcomplabs.com)

## Acknowledgements
* None at this time
