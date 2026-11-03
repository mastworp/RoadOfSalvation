ARG FEDORA_VERSION=45

FROM registry.fedoraproject.org/fedora:${FEDORA_VERSION} AS toolchain
RUN dnf -y install --setopt=install_weak_deps=False \
        clang \
        cmake \
        compiler-rt \
        gcc-c++ \
        git-core \
        glibc-static \
        libasan \
        libstdc++-static \
        libubsan \
        ninja-build \
        python3.14 \
        rpm-build \
    && dnf clean all
COPY requirements-dev.txt /tmp/requirements-dev.txt
RUN python3.14 -m venv /opt/venv \
    && /opt/venv/bin/pip install --no-cache-dir --disable-pip-version-check -r /tmp/requirements-dev.txt \
    && rm /tmp/requirements-dev.txt
ENV PATH=/opt/venv/bin:${PATH}
WORKDIR /src

FROM toolchain AS build
ARG PRESET=gcc-debug
COPY . .
RUN cmake --workflow --preset "${PRESET}"

FROM toolchain AS package
COPY . .
RUN cmake --workflow --preset dist

FROM scratch AS dist
COPY --from=package /src/dist/ /

FROM registry.fedoraproject.org/fedora-minimal:${FEDORA_VERSION} AS runtime
RUN --mount=type=bind,from=dist,target=/dist rpm -i --nosignature /dist/*.rpm
USER nobody
ENTRYPOINT ["RoadOfSalvation"]
