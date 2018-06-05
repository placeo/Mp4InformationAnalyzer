find ./ -name "CMakeFiles" -exec rm -rf {} \;
find ./ -name "target" -exec rm -rf {} \;
find ./ -name "CMakeCache.txt" -exec rm -f {} \;
find ./ -name "*.cmake" -exec rm -f {} \;
find ./ -name "Makefile" -exec rm -f {} \;
find ./ -type f -name "mp4Analyzer" -exec rm -f {} \;
find ./ -type f -name "*.mp4" -exec rm -f {} \;
rm -rf install_manifest.txt log *.mp4
