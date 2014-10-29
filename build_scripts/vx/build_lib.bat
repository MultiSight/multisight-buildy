
rem ./build_lib.bat <name> <src> <path>

echo "Building: %1"

pushd %3

if EXIST build (
   rd /s /q build
)

md build

pushd build

IF DEFINED RELEASE (
  cmake -DCMAKE_BUILD_TYPE=Release .. -G"Visual Studio 12 Win64"
) ELSE (
  cmake .. -G"Visual Studio 12 Win64"
)
if %errorlevel% neq 0 exit -1

IF DEFINED RELEASE (
  cmake --build . --config Release
) ELSE (
  cmake --build .
)
if %errorlevel% neq 0 exit -1

IF DEFINED RELEASE (
  cmake --build . --config Release --target install
) ELSE (
  cmake --build . --target install
)
if %errorlevel% neq 0 exit -1

popd

popd
