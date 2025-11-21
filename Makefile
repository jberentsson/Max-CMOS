all:
	cmake -B build -G "Unix Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM="3.5" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake --build build --config Release -j8 --verbose

tidy:
	@echo "Running clang-tidy (all checks)..."
	@find source/projects source/thulr/source -name "*.cpp" -o -name "*.hpp" | \
	    xargs -r clang-tidy \
	    -- -std=c++17 \
	    -I source/thulr/source \
	    -isystem source/min-api \
	    -isystem source/min-api/include \
	    -isystem source/min-api/max-sdk-base/c74support \
	    -isystem source/min-api/max-sdk-base/c74support/max-includes \
	    -isystem build/_deps/catch2-src/single_include \
	    -isystem source/min-api/test \
	    -Wno-everything

tidy-ci:
	@echo "Running clang-tidy (CI checks only)..."
	@find source/projects source/thulr/source -name "*.cpp" -o -name "*.hpp" | \
	    xargs -r clang-tidy \
	    -checks='readability-*,modernize-*,performance-*,bugprone-*,-modernize-avoid-c-arrays,-readability-identifier-naming,-bugprone-chained-comparison,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-use-enum-class' \
	    -- -std=c++17 \
	    -I source/thulr/source \
	    -isystem source/min-api \
	    -isystem source/min-api/include \
	    -isystem source/min-api/max-sdk-base/c74support \
	    -isystem source/min-api/max-sdk-base/c74support/max-includes \
	    -isystem build/_deps/catch2-src/single_include \
	    -isystem source/min-api/test \
	    -Wno-everything

format:
	# TODO: Fix the header file linting.
	clang-format -i -style=file source/projects/*/*.cpp

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

line-count:
	wc -l source/thulr/source/*/*.{cpp,hpp} source/projects/*/*.{cpp,hpp} | sort -n
