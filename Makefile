all:
	cmake -B build
	cmake --build build --config Release -j8

tidy:
	clang-tidy source/projects/seidr.*/*.{cpp,hpp} -- -std=c++17 \
    -I. -Isource -Isource/thulr/source \
    -Wno-everything

format:
	clang-format -i -style=file source/projects/*/*.{hpp,cpp}

test:
	cd build && ctest -C Release --output-on-failure

check:
	cppcheck --enable=warning,style,performance,portability \
		-I source/thulr/source \
		--std=c++17 \
		--check-level=exhaustive \
		source/projects/seidr.*/

clean:
	rm -rf build
