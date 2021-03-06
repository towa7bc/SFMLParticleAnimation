macro(run_conan)
    # Download automatically, you can also just copy the conan.cmake file
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(
                STATUS
                "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD
                "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_add_remote(NAME bincrafters URL
            https://api.bintray.com/conan/bincrafters/public-conan)

    conan_add_remote(NAME manu343726 URL
            https://api.bintray.com/conan/manu343726/conan-packages)

    conan_add_remote(NAME conan-mpusz URL
            https://api.bintray.com/conan/mpusz/conan-mpusz)

    conan_cmake_run(
            REQUIRES
            ${CONAN_EXTRA_REQUIRES}
            catch2/2.12.0
            spdlog/1.6.1
            sfml/2.5.1@bincrafters/stable
            imgui-sfml/2.1@bincrafters/stable
            #irrlicht/1.8.4@mpusz/testing
            OPTIONS
            ${CONAN_EXTRA_OPTIONS}
            BASIC_SETUP
            CMAKE_TARGETS # individual targets to link to
            BUILD
            missing)
endmacro()
