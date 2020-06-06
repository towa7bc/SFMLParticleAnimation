# Generic test that uses conan libs
add_executable(
        SFMLTest
        main.cpp
        Particle.cpp
        Particle.hpp
        ParticleSystem.cpp
        ParticleSystem.hpp)

target_link_libraries(
        SFMLTest
        PRIVATE project_options
        project_warnings
        CONAN_PKG::catch2
        CONAN_PKG::spdlog
        CONAN_PKG::sfml
        CONAN_PKG::imgui-sfml
)
