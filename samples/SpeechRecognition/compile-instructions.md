if there's a build folder, remove it (cmake will try to read the cache and the new build won't work)
```bash
rm -r build
```
now we can build the speech recognition example using the Toolchain (this tells cmake to compile a binary that will run in the Bela) and the ArmNN library path. arguments start with -D..
```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=Toolchain.cmake -DARMNN_LIB_DIR=/opt/armnn-dist/armnn/lib/
```
once it's finished we can build
```bash
cmake --build build
```
hopefully that was errorless