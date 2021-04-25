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


# Example usage:
```bash
curl --location --request POST 'http://localhost:8080/volume' \
--form 'n="2"' \
--form 'image0=@"/home/hyperledger/Downloads/on_top.jpg"' \
--form 'image1=@"/home/hyperledger/Downloads/portrait.jpg"' \
--form 'imagePoints0="750 6950
4972 6412
4912 1572
688 2140"' \
--form 'imagePoints1="102 6109
5956 6005
5873 2133
102 2209"' \
--form 'objectPoints0="0 0 28
40 0 28
40 40 28
0 40 28"' \
--form 'objectPoints1="0 0 0
40 0 0
40 0 28
0 0 28"' \
--form 'cameraMatrix="5575.0 0 3000
0 5575.0 4000
0 0 1"' \
--form 'distCoefficients="0
0
0
0
0"'
```