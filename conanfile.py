from conans import ConanFile, CMake, tools


class samna(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = [("cppzmq/4.8.1"),
                ("stduuid/1.2.2"),
                ("concurrentqueue/1.0.3")]

    default_options = {}

    generators = "cmake", "cmake_find_package", "virtualrunenv"

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="lib", src="lib")
        self.copy("*.so*", dst="lib", src="lib")

    def configure(self):
        self.options["zeromq"].with_draft_api = True
