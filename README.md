```
git clone https://github.com/OPSphystech420/OpenGL-3D-Train.git
```

### MacOS

#### Requirements
GCC c++17 or later
```
brew install glfw3
brew install glew
brew install openal-soft
brew install libsndfile 
```
#### Run
```bash
sed -i '' 's/\r$//' run.sh
chmod +x run.sh
./run.sh
```
#### 🛠️ Don't use freeglut!
It will suggest « _failed to open display ''_ »‎ which means that the freeglut lib can't find an X server to connect to, while MacOS uses a different windowing system.

![White blank editor](/images/img1.png)

![White blank editor](/images/img2.png)

![White blank editor](/images/img3.png)
