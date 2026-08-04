# Local Fedora RPM

This fork publishes a locally built Fedora RPM for the MatePad streaming
stack. A build is accepted only when the working tree is clean and `HEAD`
matches the requested version tag.

```sh
git tag v2026.804.1
packaging/linux/local/build-fedora-rpm 2026.804.1
```

The helper configures the complete Linux backend with GCC 15, CUDA 13.3,
Vulkan, KWin, Portal, Wayland, and X11 support. Native compilation runs in a
bounded systemd user cgroup. The helper builds Sunshine and its tests, runs
the Linux input-mode tests, creates the RPM, and verifies package identity,
publisher metadata, and Qt 6 dependencies.

The verified artifact is written to:

```text
artifacts/2026.804.1/Sunshine-2026.804.1-1.kkkzbh.x86_64.rpm
```

The integration repository at
`/home/kkkzbh/code/tool/kwin-virtual-output` owns installation of this RPM,
Sunshine configuration, virtual-output helpers, and user units through its
`install-stack.sh` entry point.
