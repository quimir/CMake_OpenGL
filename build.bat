cd /d  "%~dp0"
if not exist build (  # Check if the build folder exists.If it doesn't, create it
    mkdir build
) else (
    del /S /Q /F build
    rd /S /Q build
    mkdir build
)

pushd build
cd build
cmake ../

cd ..

pause