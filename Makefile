all:
	cmake -B build
	cmake --build build --config Release

tidy:
	clang-tidy source/projects/*/*.{cpp,hpp} -- std=c++17 -Ibuild/_deps/catch2-src/single_include

format:
	clang-format -i source/projects/*/*.{hpp,cpp}

test:
	cd build && ctest -C Release --output-on-failure
