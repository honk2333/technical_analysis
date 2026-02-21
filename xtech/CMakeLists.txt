cmake_minimum_required(VERSION 3.10)
project(xtechnical_analysis)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件搜索路径
include_directories(include)

# 检查是否存在Eigen库（用于ssa.cpp和其他依赖它的文件）
find_package(Eigen3 QUIET)
if(Eigen3_FOUND)
    include_directories(${EIGEN3_INCLUDE_DIR})
    message(STATUS "Found Eigen3: ${EIGEN3_INCLUDE_DIR}")
else()
    message(WARNING "Eigen3 not found. Some tests may fail to compile.")
    # 定义一个宏，用于条件编译
    add_definitions(-DNO_EIGEN)
endif()

# 测试文件列表
set(TEST_FILES
    tests/body_filter.cpp
    tests/trend_direction_force_index.cpp
    tests/super_trend.cpp
    tests/period_stats.cpp
    tests/cluster_shaper.cpp
    tests/cci.cpp
    tests/atr.cpp
    tests/fractals.cpp
    tests/ssa.cpp
    tests/check-lrma/check-lrma.cpp
    tests/check-rshillma/check-rshillma.cpp
    tests/check-td/check-td.cpp
    tests/check_bb/check_bb.cpp
    tests/check_crsi/check_crsi.cpp
    tests/check_delay_line/check_delay_line.cpp
    tests/check_fft/check_fft.cpp
    tests/check_indicators/check_indicators.cpp
    tests/check_maz/check_maz.cpp
    tests/check_min_max/check_min_max.cpp
    tests/check_min_max_difference/check_min_max_difference.cpp
    tests/check_pri/check_pri.cpp
    tests/check_sma/check_sma.cpp
    tests/check_stochastics/check_stochastics.cpp
    tests/check_sum/check_sum.cpp
    tests/check_zscore/check_zscore.cpp
    tests/checking_circular_buffer/checking_circular_buffer.cpp
    tests/checking_delay_measurement/checking_delay_measurement.cpp
    tests/checking_least_square_method/checking_least_square_method.cpp
    tests/checking_normalization/checking_normalization.cpp
    tests/checking_statistical_indicators/checking_statistical_indicators.cpp
    tests/checking_statistics/checking_statistics.cpp
    tests/cmp_min_max_witch_streaming_min_max/cmp_min_max_witch_streaming_min_max.cpp
)

# 为每个测试文件创建可执行目标
foreach(TEST_FILE ${TEST_FILES})
    # 从文件路径中提取目标名称
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    
    # 创建可执行目标
    add_executable(${TEST_NAME} ${TEST_FILE})
    
    # 对于需要特殊处理的测试文件，可以在这里添加额外的配置
    if(${TEST_NAME} STREQUAL "ssa")
        if(NOT Eigen3_FOUND)
            # 如果没有找到Eigen库，禁用ssa测试
            set_target_properties(${TEST_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        endif()
    elseif(${TEST_NAME} STREQUAL "cluster_shaper")
        # 使用vcpkg安装的OpenCV
        set(OpenCV_DIR /home/wanghk/code/vcpkg/installed/x64-linux/share/opencv4)
        find_package(OpenCV REQUIRED)
        if(OpenCV_FOUND)
            # 添加包含路径和链接库
            include_directories(${OpenCV_INCLUDE_DIRS})
            # 添加libjpeg-turbo的包含路径
            include_directories(/home/wanghk/code/vcpkg/installed/x64-linux/include)
            # 添加libpng的包含路径
            include_directories(/home/wanghk/code/vcpkg/installed/x64-linux/include)
            # 链接OpenCV库和所有需要的依赖库
            target_link_libraries(${TEST_NAME} 
                ${OpenCV_LIBS} 
                /home/wanghk/code/vcpkg/installed/x64-linux/lib/libjpeg.a
                /home/wanghk/code/vcpkg/installed/x64-linux/lib/libpng16.a
                /home/wanghk/code/vcpkg/installed/x64-linux/lib/libz.a
            )
            message(STATUS "Found OpenCV: ${OpenCV_VERSION}")
        else()
            # 如果找不到OpenCV，禁用cluster_shaper测试
            set_target_properties(${TEST_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
            message(WARNING "OpenCV not found. cluster_shaper test will be disabled.")
        endif()
    endif()
endforeach()

# 添加一个目标来编译所有测试
add_custom_target(all_tests)
foreach(TEST_FILE ${TEST_FILES})
    # 从文件路径中提取目标名称
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_dependencies(all_tests ${TEST_NAME})
endforeach()

# 添加CTest支持
enable_testing()
foreach(TEST_FILE ${TEST_FILES})
    # 从文件路径中提取目标名称
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    # 添加测试
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

message(STATUS "CMake configuration completed successfully!")
message(STATUS "To compile all tests, run: cmake --build .")
message(STATUS "To run all tests, run: ctest")
message(STATUS "To run all tests with verbose output, run: ctest -v")
message(STATUS "To run a specific test, run: ./<test_name>")
message(STATUS "To run a specific test with ctest, run: ctest -R <test_name>")
