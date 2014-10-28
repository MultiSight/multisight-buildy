
rem ./build_lib.bat <name> <src> <path>

echo "Building: %1"

pushd %3

if EXIST build (
   rd /s /q build
)

md build

pushd build

cmake .. -G"Visual Studio 12 Win64"
if %errorlevel% neq 0 exit -1

cmake --build .
if %errorlevel% neq 0 exit -1

cmake --build . --target install
if %errorlevel% neq 0 exit -1

popd

popd
