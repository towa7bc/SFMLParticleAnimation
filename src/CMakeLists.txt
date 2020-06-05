# Generic test that uses conan libs
add_executable(
        SFMLTest
        main.cpp
        Particle.cpp Particle.hpp ParticleSystem.cpp ParticleSystem.hpp)

#set(CMAKE_PREFIX_PATH
#        /usr/local/Cellar/qt/5.14.2;C:\\Qt\\5.14.2\\msvc2017_64\\;/usr/local/include/stlab
#        )

target_link_libraries(
        SFMLTest
        PRIVATE project_options
        project_warnings
        CONAN_PKG::catch2
        CONAN_PKG::spdlog
        CONAN_PKG::sfml
        CONAN_PKG::imgui-sfml
)
