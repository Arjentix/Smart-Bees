# CMake generated Testfile for 
# Source directory: /home/batman/prj/Smart-Bees/Micro-services/libs/json/test/cmake_import_minver
# Build directory: /home/batman/prj/Smart-Bees/Micro-services/test_folder/json_test/build/json/test/cmake_import_minver
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cmake_import_minver_configure "/usr/bin/cmake" "-G" "Unix Makefiles" "-Dnlohmann_json_DIR=/home/batman/prj/Smart-Bees/Micro-services/test_folder/json_test/build/json" "/home/batman/prj/Smart-Bees/Micro-services/libs/json/test/cmake_import_minver/project")
set_tests_properties(cmake_import_minver_configure PROPERTIES  FIXTURES_SETUP "cmake_import_minver")
add_test(cmake_import_minver_build "/usr/bin/cmake" "--build" ".")
set_tests_properties(cmake_import_minver_build PROPERTIES  FIXTURES_REQUIRED "cmake_import_minver")
