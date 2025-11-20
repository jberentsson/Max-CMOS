all:
	cmake -B build
	cmake --build build --config Release -j8

tidy:
	@find source/projects/seidr.* source/thulr/source/* \( -name "*.cpp" -o -name "*.hpp" \) -exec clang-tidy {} \
	    -- -std=c++17 \
	    -Isource/thulr/source \
	    -isystem source/min-api \
	    -isystem source/min-api/include \
	    -isystem source/min-api/max-sdk-base/c74support \
	    -isystem source/min-api/source/c74_min_unittest \
	    -isystem source/min-api/test \
	    -Wno-everything \;

tidy-ci:
	@find source/projects/seidr.* source/thulr/source/* \( -name "*.cpp" -o -name "*.hpp" \) -exec clang-tidy {} \
	    -checks='readability-*,modernize-*,performance-*,bugprone-*,-modernize-avoid-c-arrays,-readability-identifier-naming,-bugprone-chained-comparison,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-use-enum-class' \
	    -- -std=c++17 \
	    -Isource/thulr/source \
	    -isystem source/min-api \
	    -isystem source/min-api/include \
	    -isystem source/min-api/max-sdk-base/c74support \
	    -isystem source/min-api/source/c74_min_unittest \
	    -isystem source/min-api/test \
	    -Wno-everything \;

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
