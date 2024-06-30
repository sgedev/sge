#
#
#

from conan import ConanFile
from conan.tools.cmake import cmake_layout

class SimpleGameEngine(ConanFile):
    name = "sge"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("libuv/1.48.0")
        self.requires("physfs/3.2.0")
        self.requires("sdl/2.30.4")

    def layout(self):
        cmake_layout(self)
