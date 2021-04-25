You need C++17 compiler.

# Build essential

```bash
$ sudo apt update && sudo apt install -y cmake g++ wget unzip
```

# Install opencv

```bash
$ cd ~/Downloads/
$ wget -O opencv.zip https://github.com/opencv/opencv/archive/master.zip
$ unzip opencv.zip
$ mkdir -p build && cd build
$ cmake  ../opencv-master
$ cmake --build .
$ sudo make install
```

Return to the project

# Install Pistache

```bash
$ sudo add-apt-repository ppa:pistache+team/unstable
$ sudo apt update
$ sudo apt install libpistache-dev
```

# Install spdlog

```bash
$ cd ~/Downloads/
$ git clone https://github.com/gabime/spdlog.git
$ cd spdlog && mkdir build && cd build
$ cmake .. && make -j
$ sudo make install
```

Return to the project

# Install boost

```bash
$ sudo apt-get install libboost-all-dev
```