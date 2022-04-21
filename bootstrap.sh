#!/bin/sh

srcdir=${PWD}
builddir=${1:-${PWD}/build.$(uname -s)}
# I prefer Ninja.
which ninja > /dev/null && cmakegen="-G Ninja"


mkdir -p "${builddir}"
cmake ${cmakegen} -S "${srcdir}" -B "${builddir}"
cmake --build "${builddir}" --target all
