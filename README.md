# RoadOfSalvation

[![CI](https://github.com/mastworp/RoadOfSalvation/actions/workflows/ci.yml/badge.svg)](https://github.com/mastworp/RoadOfSalvation/actions/workflows/ci.yml)

A small TUI RPG game written in C++23.

The game is in its infancy really: it creates a hero, loads the hero's profession starting stats from...
`professions.json` and prints them. The TUI itself hasn't been implemented yet.

## Installing

The Linux packages contain a statically linked binary, so they have no dependencies and work on any x86_64
distribution. Tagged versions are published on the [Releases page](https://github.com/mastworp/RoadOfSalvation/releases),
and every CI run that passes its checks uploads them as the `RoadOfSalvation-linux-x86_64` artifact.

| Package                                         | Use it on                                   |
|-------------------------------------------------|---------------------------------------------|
| `RoadOfSalvation-<version>-Linux-x86_64.sh`     | Any distribution, self-extracting installer |
| `RoadOfSalvation-<version>-Linux-x86_64.tar.gz` | Any distribution, runs without installing   |
| `roadofsalvation_<version>_amd64.deb`           | Debian, Ubuntu and derivatives              |
| `roadofsalvation-<version>-1.x86_64.rpm`        | Fedora, RHEL and its rebuilds, openSUSE     |

CI installs every build on Ubuntu 24.04, Debian 13, Fedora 45, openSUSE Tumbleweed, Arch Linux, Alpine and AlmaLinux 8
and runs it there as an unprivileged user.

### Any distribution

```sh
sudo sh RoadOfSalvation-1.0.0-Linux-x86_64.sh --prefix=/usr/local --exclude-subdir --skip-license
RoadOfSalvation
```

Without options the installer asks where to install. To uninstall, remove `/usr/local/bin/RoadOfSalvation` and
`/usr/local/share/RoadOfSalvation`.

To play without installing anything at all, just extract the archive and run the game from there. The extracted folder can of course be moved. anywhere, because the game finds its data relative to its own executable.

```sh
tar -xzf RoadOfSalvation-1.0.0-Linux-x86_64.tar.gz
./RoadOfSalvation-1.0.0-Linux-x86_64/bin/RoadOfSalvation
```

### Debian and Ubuntu

```sh
sudo dpkg -i roadofsalvation_1.0.0_amd64.deb
```

Uninstall with udo dpkg -r roadofsalvation

### Fedora, RHEL and its rebuilds

```sh
sudo dnf install ./roadofsalvation-1.0.0-1.x86_64.rpm
```

Uninstall with `sudo dnf remove roadofsalvation`. The RPM isn't signed: dnf installs it with a warning, but RPM 6 (Fedora 45 instance) will refuse a plain 'rpm -i' unless you add '--nosignature'

### openSUSE

```sh
sudo zypper install --allow-unsigned-rpm ./roadofsalvation-1.0.0-1.x86_64.rpm
```

Uninstall with sudo zypper remove roadofsalvation.

### Checking a download

Every package comes with a `.sha256` file:

```sh
sha256sum -c roadofsalvation_1.0.0_amd64.deb.sha256
```

## Building from source

### Requirements

| Tool         | Version                                                                     |
|--------------|-----------------------------------------------------------------------------|
| CMake        | 4.3 or newer                                                                |
| Ninja        | Any recent version; every preset uses it                                    |
| C++ compiler | C++23, including `<print>`. Tested with GCC 16, Clang 23 and Apple clang 21 |
| Git          | Needed to download GoogleTest                                               |
| Python       | 3.14 or older, only for the developer tools below                           |
| Docker       | Optional, provides all of the above on Fedora 45                            |

[nlohmann/json](https://github.com/nlohmann/json) and [GoogleTest](https://github.com/google/googletest) are downloaded
during configuration. An installed GoogleTest is used instead when CMake can find one.

clang-format, cmake-format and gcovr are pinned in `requirements-dev.txt` so that local results match CI. Install them
into a virtual environment and activate it before configuring, so CMake picks up these exact versions:

```sh
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements-dev.txt
```

cmake-format doesn't run on Python 3.15 or newer. If that's your default, create the environment with an older
interpreter, for example `python3.14 -m venv .venv`.

### Quick start

```sh
cmake --workflow --preset clang-debug
./build/clang-debug/stage/bin/RoadOfSalvation
```

The workflow configures, builds and runs the tests. Every build stages the game in `build/<preset>/stage` with the same
layout as an installation, so it runs straight from there.

### Presets

| Configure preset | Compiler  | Build type | Notes                                                     |
|------------------|-----------|------------|-----------------------------------------------------------|
| `gcc-debug`      | `g++`     | Debug      | ASan and UBSan                                            |
| `gcc-release`    | `g++`     | Release    |                                                           |
| `clang-debug`    | `clang++` | Debug      | ASan and UBSan                                            |
| `clang-release`  | `clang++` | Release    |                                                           |
| `coverage`       | `g++`     | Debug      | gcov instrumentation, see [Code coverage](#code-coverage) |
| `dist`           | `g++`     | Release    | Static binary for the packages, Linux only                |
| `format`         | default   |            | Only provides the formatting targets                      |

Every preset builds with Ninja into `build/<preset>`, installs into `install/<preset>` and treats warnings as errors. On
macOS `g++` is Apple clang, so the `gcc-*` presets build with Apple clang there.

Each configure preset has a workflow preset with the same name that runs all of its steps. You can also run the steps
one by one:

```sh
cmake --preset gcc-release
cmake --build --preset gcc-release
ctest --preset gcc-release
```

### Options

| Option                  | Default | Effect                                                        |
|-------------------------|---------|---------------------------------------------------------------|
| `BUILD_TESTS`           | `ON`    | Build the unit tests                                          |
| `ENABLE_SANITIZERS`     | `ON`    | Build with ASan and UBSan; only the `*-debug` presets keep it |
| `ENABLE_COVERAGE`       | `OFF`   | Instrument the build for gcov and add the `coverage` target   |
| `ENABLE_STATIC_LINKING` | `OFF`   | Link the game fully statically, Linux only                    |
| `USE_CCACHE`            | `ON`    | Use ccache when it's installed                                |
| `ENABLE_TUI`            | `OFF`   | Build the TUI layer, which isn't implemented yet              |

Override them when configuring, for example `cmake --preset clang-debug -DENABLE_SANITIZERS=OFF`.

### Installing a build

```sh
cmake --workflow --preset clang-release
cmake --build --preset clang-release-install
```

This installs into `install/clang-release`. The presets `gcc-debug-install`, `gcc-release-install`,
`clang-debug-install`, `clang-release-install` and `dist-install` do the same for their builds. To install somewhere
else:

```sh
cmake --install build/clang-release --prefix ~/.local
```

The installation is relocatable: the game looks for its data in `../share/RoadOfSalvation`, relative to its executable.

## Tests

The unit tests use GoogleTest and GoogleMock. They cover the core library and check the shipped `professions.json`
a smoke test runs the game itself.

```sh
ctest --preset clang-debug
ctest --preset clang-debug -R JsonParserTest
```

The test executable, `core_tests`, is built into `build/<preset>/stage/bin` next to the game, so relative data paths
resolve exactly as they do in an installation.

`JsonParserTest.LoadsReadOnlyFiles` is skipped when it runs as root, because root ignores file permissions. CI covers
that case by running the installed game as an unprivileged user.

## Formatting

C++ code follows `.clang-format` (Google style) and CMake code follows `.cmake-format.yaml`. CI fails on unformatted
code.

```sh
cmake --workflow --preset format
cmake --workflow --preset format-check
```

The first command rewrites files in place. The second only checks them, and is what CI runs. Every configured build
tree also has the targets `format`, `format-check`, `clang-format`, `clang-format-check`, `cmake-format` and
`cmake-format-check`. When a tool is missing, its targets fail instead of passing silently.

Other clang-format versions can format code differently from CI, so use the version pinned in `requirements-dev.txt`.

## Code coverage

```sh
cmake --workflow --preset coverage
```

This builds with gcov instrumentation, runs the tests and then runs gcovr over `src/`. The reports go to
`build/coverage/coverage/`:

| File            | Contents                                   |
|-----------------|--------------------------------------------|
| `index.html`    | Browsable report with per-line coverage    |
| `cobertura.xml` | Cobertura XML for other tools              |
| `summary.json`  | Totals as JSON                             |
| `summary.md`    | Markdown summary, shown on the CI job page |

GCC builds are measured with `gcov` and LLVM clang builds with `llvm-cov gcov`. On macOS the preset builds with Apple
clang and uses Apple's `gcov`.

### Coverage guard

The last step of the workflow fails when coverage drops below these minimums:

| Metric    | Minimum | Cache variable           |
|-----------|---------|--------------------------|
| Lines     | 95 %    | `COVERAGE_MIN_LINES`     |
| Functions | 95 %    | `COVERAGE_MIN_FUNCTIONS` |
| Branches  | 50 %    | `COVERAGE_MIN_BRANCHES`  |

Each compiler counts branches differently, which is why the branch minimum is lower. The defaults live in
`cmake/Coverage.cmake`. To try other values locally:

```sh
cmake --preset coverage -DCOVERAGE_MIN_LINES=98
cmake --build --preset coverage-report
```

## Packaging

The `dist` preset links the game statically, runs the tests and builds the packages listed under
[Installing](#installing), together with their `.sha256` files, into `dist/`:

```sh
cmake --workflow --preset dist
```

This preset only exists on Linux. It needs the static glibc and libstdc++ libraries and `rpmbuild` (`glibc-static`,
`libstdc++-static` and `rpm-build` on Fedora). On macOS, or without those, build the packages in Docker instead:

```sh
docker compose run --rm package
```

Docker on Apple silicon produces aarch64 packages.

## Docker

The Docker image provides the complete toolchain on Fedora 45: GCC, Clang, CMake, Ninja, rpmbuild and the pinned Python
tools. All compose services except `game` mount the repository at `/src`:

| Service        | Runs                                                                     |
|----------------|--------------------------------------------------------------------------|
| `dev`          | An interactive shell                                                     |
| `test`         | The `gcc-debug` and `clang-debug` workflows                              |
| `format-check` | The formatting check                                                     |
| `coverage`     | The coverage workflow, including the guard                               |
| `package`      | The `dist` workflow, writing the packages to `dist/`                     |
| `game`         | The game, installed from its RPM into Fedora minimal and run as `nobody` |

```sh
docker compose run --rm test
docker compose run --rm game
```

Builds in the container go to `build/docker/<preset>`, so they don't clash with builds on the host. The coverage report
for example, ends up in `build/docker/coverage/coverage/index.html`.

The Dockerfile can also be used on its own:

| Stage       | Purpose                                                                            |
|-------------|------------------------------------------------------------------------------------|
| `toolchain` | Fedora with the build tools; the image the compose services use                    |
| `build`     | Runs the workflow preset given with `--build-arg PRESET=…`, `gcc-debug` by default |
| `package`   | Runs the `dist` workflow                                                           |
| `dist`      | Only the packages, for exporting                                                   |
| `runtime`   | The default target: the game, installed from its RPM                               |

```sh
docker build --target build --build-arg PRESET=clang-release .
docker build --target dist --output dist .
docker build -t roadofsalvation.
docker run --rm -it roadofsalvation
```

Pass `--build-arg FEDORA_VERSION=…` to build on another Fedora release.

## Continuous integration

[`.github/workflows/ci.yml`](.github/workflows/ci.yml) runs on pushes to `main`, on pull requests, on `v*` tags and on
demand. The build jobs run in Fedora 45 containers.

| Job            | What it does                                                                                     |
|----------------|--------------------------------------------------------------------------------------------------|
| Format         | Runs the `format-check` workflow                                                                 |
| Build and test | Runs the `gcc-debug`, `gcc-release`, `clang-debug` and `clang-release` workflows                 |
| Coverage       | Runs the `coverage` workflow with its guard, shows the summary and uploads `coverage-report`     |
| Package        | Runs the `dist` workflow once the jobs above pass and uploads `RoadOfSalvation-linux-x86_64`     |
| Install on …   | Installs the packages on seven distributions as root, then runs the game as an unprivileged user |
| Release        | On `v*` tags only: publishes a GitHub release with the packages                                  |

### Releasing

1. Update `VERSION` in the `project()` call in `CMakeLists.txt` and commit it.
2. Tag the commit and push the tag:

   ```sh
   git tag v1.1.0
   git push origin v1.1.0
   ```

The release job stops if the tag doesn't match the project version.

## Project layout

```text
.
├── CMakeLists.txt              Top-level build
├── CMakePresets.json           Configure, build, test, package and workflow presets
├── cmake/                      Warnings, sanitizers, coverage, formatting, packaging and other modules
├── src/
│   ├── main.cpp                Entry point
│   ├── app/                    Core library: characters, professions and JSON loading
│   │   └── inc/RoadOfSalvation Public headers
│   ├── data/                   Game data
│   └── tui/                    TUI layer, not implemented yet
├── tests/                      Unit tests
├── Dockerfile
├── docker-compose.yml
├── requirements-dev.txt        Pinned clang-format, cmake-format and gcovr
└── .github/workflows/ci.yml    CI pipeline
```

### Game data

Data files live in `src/data` and are registered with `add_game_data()` in `src/CMakeLists.txt`. The build copies them
to `build/<preset>/stage/share/RoadOfSalvation`, and installing puts them in `share/RoadOfSalvation`. `json_parser`
resolves relative paths against that directory.

`professions.json` holds the starting stats of each profession (`crusader`, `alchemist`, `scout` and `none`) for all
eleven attributes.
