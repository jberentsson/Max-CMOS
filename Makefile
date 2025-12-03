all:
	mkdir -p temp
	cmake -B build --debug-output --trace-expand --trace-source=CMakeLists.txt -G "Unix Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM="3.5" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 2>&1> temp/CMakeSetup.log
	cmake --build build --config Release -j8 --verbose 2>&1> temp/CMakeBuild.log

tidy:
	@echo "Running clang-tidy (all checks)..."
	@find source/projects -name "*.cpp" -exec clang-tidy --system-headers=0  -p build {} \
	           -- -std=c++17 \
	           -I source/thulr/source \
	           -isystem source/min-api \
	           -isystem source/min-api/include \
	           -isystem source/min-api/max-sdk-base/c74support \
	           -isystem source/min-api/max-sdk-base/c74support/max-includes \
	           -isystem build/_deps/catch2-src/single_include \
	           -isystem source/min-api/test \
	           -Wno-everything \;

tidy-ci:
	@echo "Running clang-tidy (CI checks only)..."
	@find source/projects -name "*.cpp" -exec clang-tidy --system-headers=0 -p build {} \
	           -checks='readability-*,modernize-*,performance-*,bugprone-*,-modernize-avoid-c-arrays,-readability-identifier-naming,-bugprone-chained-comparison,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-use-enum-class' \
	           -- -std=c++17 \
	           -I source/thulr/source \
	           -isystem source/min-api \
	           -isystem source/min-api/include \
	           -isystem source/min-api/max-sdk-base/c74support \
	           -isystem source/min-api/max-sdk-base/c74support/max-includes \
	           -isystem build/_deps/catch2-src/single_include \
	           -isystem source/min-api/test \
	           -Wno-everything \;

format:
	@echo "Formatting C++ files..."
	@find source/projects source/thulr/source -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" -exec clang-format -i -style=file {} \;

format-check:
	@echo "Checking code formatting..."
	@find source/projects source/thulr/source -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" -exec clang-format --dry-run --Werror -style=file {} \;

test:
	cd build && ctest -C Release --output-on-failure

check:
	@echo "Running cppcheck..."
	@cppcheck --enable=warning,style,performance,portability \
		-I source/thulr/source \
		--std=c++17 \
		--check-level=exhaustive \
		source/projects/seidr.*/ \
		source/thulr/source/

clean:
	rm -rf build

line-count:
	@echo "Counting lines of code..."
	@wc -l source/thulr/source/*/*.{cpp,hpp} source/thulr/source/*/*/*.{cpp,hpp} source/projects/*/*.{cpp,hpp} source/projects/*/*/*.{cpp,hpp} | sort -n

deps:
	@echo "Checking dependencies..."
	@cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON > /dev/null 2>&1
	@echo "Dependencies configured"

.PHONY: all tidy tidy-ci tidy-fast format format-check test check clean line-count deps
