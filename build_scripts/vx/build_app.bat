
rem # ./build_lib.sh <name> <src> <path>

echo "Building: %1"

pushd %3

if EXIST build (
    rd /s /q build
)

md build

pushd build

IF "%4"=="RELEASE" (

  cmake -DCMAKE_BUILD_TYPE=Release .. -G"Visual Studio 12 Win64"
  if %errorlevel% neq 0 exit -1

  cmake --build . --config Release
  if %errorlevel% neq 0 exit -1

) ELSE (

  cmake .. -G"Visual Studio 12 Win64"
  if %errorlevel% neq 0 exit -1

  cmake --build .
  if %errorlevel% neq 0 exit -1

)

popd

popd
